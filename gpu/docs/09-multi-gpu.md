---
title:  Multi-GPU programming
event:  CSC Summer School in High-Performance Computing 2025
lang:   en
---

# Anatomy of a supercomputer

<div class="column" style=width:58%>
- Supercomputers consist of nodes connected by a high-speed network
- A node can contain several multicore CPUs and several GPUs
    - 8 GPUs per node in LUMI, 4 GPUs per node in Mahti and Roihu
- All CPU memory within a node is shared
- GPU memories within a node are distinct
</div>
<div class="column" style=width:38%>
![](img/lumi.png){.center width=80%}
<small>Lumi - Pre-exascale system in Finland</small>
</div>

# Using multiple GPUs

- Why to use multiple GPUs?
    - Application requires more memory than a single GPU has
    - Solve the problem faster than with single GPU
- Using multiple GPUs requires:
    - Coordinating the work between GPUs
    - Moving data between GPUs
- HIP/CUDA has functionality for intranode peer-to-peer data movement
- MPI and RCCL/NCCL can be use both for intra- and internode data movement


# Multi-GPU Programming Models


*Model - example API*

| | One GPU per process | Many GPUs per process | One GPU per thread |
|--|--|--|--|
| Communication | MPI | HIP | HIP  |
| Synchronization | MPI/HIP | HIP (streams) | OpenMP/HIP (streams) |
| | ![](img/single_proc_mpi_gpu2.png){width=100%} | ![](img/single_proc_multi_gpu.png){width=100%} | ![](img/single_proc_thread_gpu.png){width=100%} | 


# One GPU per Process

- Simple porting:
  - Each process assumes one GPU
  - No GPU device selection within program
-  Communication between GPUs with MPI or RCCL/NCCL
-  Works with arbitrary number of GPUs
  - Same programming approach for inter- and intranode data movement
- Very similar MPI programming as with CPUs

# Assuming one GPU per process: LUMI

- Set environment variables prior to executing binary
  - `export ROCR_VISIBLE_DEVICES=$SLURM_LOCALID` on [LUMI-G](https://docs.lumi-supercomputer.eu/runjobs/scheduled-jobs/lumig-job/)
  - `select_gpu` script:
```shell
#!/bin/bash
export ROCR_VISIBLE_DEVICES=$SLURM_LOCALID
exec $*
```
  - `srun ... ./select_gpu <my_binary>`
- Enable GPU support in MPI
  - `export MPICH_GPU_SUPPORT_ENABLED=1`

# Sharing GPUs among processes with MPI

**Effectively one gpu per process**

- Idea
  1. Split the global communicator to one per shared memory space
  2. Get rank within those communicator and pick a GPU based on that number

```c++
int deviceCount, nodeRank;
MPI_Comm commNode;
MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &commNode);
MPI_Comm_rank(commNode, &nodeRank);
hipGetDeviceCount(&deviceCount);
hipSetDevice(nodeRank % deviceCount);
```

- Still on LUMI: Enable GPU support in MPI
  - `export MPICH_GPU_SUPPORT_ENABLED=1`

# Many GPUs per Process

* HIP
  * Process selects GPU with `hipSetDevice()`
  * Default stream uses the selected device
  * `hipStreamCreate()` will assign the created stream to selected GPU
  * `hipMemcpy()` with `hipMemcpyDefault` will perform P2P copies with unified virtual addressing

* OpenMP
  * Select device with `omp_set_default_device()`
  * Asynchronous function calls `nowait` (OpenMP) for overlapping work

# Many GPUs per Process: Code Example 

::::::{.columns}
:::{.column}
*HIP*
<small>
```cpp
//Create streams
for (size_t n = 0;  n < num_devices; n++) {
  hipSetDevice(n);
  hipStreamCreate(&stream[n]);
}
// Launch kernels 
for(size_t n = 0; n < num_devices; n++) 
  kernel<<<blocks[n],threads[n], 0, stream[n]>>>(args[n], size[n]);

//Synchronize all kernels with host 
for(size_t n = 0; n < num_devices; n++) 
  hipStreamSynchronize(stream[n]);
```
</small>
:::
:::{.column}
*OpenMP offload*
<small>
```cpp
// Launch kernels (OpenMP)
for(int n = 0; n < num_devices; n++) {
  omp_set_default_device(n);
  #pragma omp target teams distribute parallel for nowait
  for (unsigned i = 0; i < size[n]; i++)
    // Do something
}
#pragma omp taskwait //Synchronize all kernels with host (OpenMP)
```

```cpp
// Launch and synchronize kernels
// from parallel CPU threads using OpenMP
#pragma omp parallel num_threads(num_devices)
{
  unsigned n = omp_get_thread_num();
  #pragma omp target teams distribute parallel for device(n)
  for (unsigned i = 0; i < size[n]; i++)
    // Do something
}
```
</small>
:::
::::::


# Sidetrack: OpenMP `device` clause

- Defines which device the directive should target
- It is available to following directives: `target`, `target data`, `target enter data`, `target exit data`, and `target update`
  - (Also: `dispatch` and `interop`)
- [Specification documentation](https://www.openmp.org/spec-html/5.2/openmpse79.html)


# GPU-GPU Communication through MPI

* GPU aware MPI libraries support direct GPU-GPU transfers
  * Can take a pointer to device buffer (avoids host/device data copies)
* Sending custom MPI datatypes falls back to communication via host
  * Data packing/unpacking must be implemented application-side on GPU

# Device management: HIP

| Api call | Description
|-:|:-|
| `hipErrot_t hipGetDeviceCount(&count)` | Query the number of available GPUS within a node |
| `hipErrot_t hipSetDevice(device)` | Set `device` as the current device for the calling host thread (device numbering starts from 0) |
| `hipErrot_t hipGetDevice(&device)` | Query the current device for the calling host thread |
| `hipErrot_t hipDeviceReset(void)` |Reset and destroy all current device resources|

# Device management: OpenMP

| Api call | Description
|-:|:-|
| `int omp_get_num_devices()` | Query the number of devices within a node |
| `void omp_set_default_device(device)` | Set `device` as the current device for the calling host thread
| `int omp_get_default_device()` | Query the current device for the calling host thread |

# Selecting the Correct GPU

* typically all processes on the node can access all GPUs of that node
* implementation for using 1 GPU per 1 MPI process

```cpp
int deviceCount, nodeRank;
MPI_Comm commNode;
MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &commNode);
MPI_Comm_rank(commNode, &nodeRank);
hipGetDeviceCount(&deviceCount);
hipSetDevice(nodeRank % deviceCount);
```
::: notes
* Can be done from slurm using `ROCR_VISIBLE_DEVICES` or `CUDA_VISIBLE_DEVICES`
:::

# Compiling MPI+GPU Code

- Trying to compile code with HIP calls with other than the `hipcc`
  compiler can result in errors
- Either set MPI compiler to use `hipcc`, eg for OpenMPI:
```bash
OMPI_CXXFLAGS='' OMPI_CXX='hipcc'
```
- or separate HIP and MPI code in different compilation units compiled with
  `mpicxx` and `hipcc`
    * Link object files in a separate step using `mpicxx` or `hipcc`
- **on LUMI, `cc` and `CC` wrappers know about both MPI and HIP**


# Example: HIP + MPI program

```cpp
hipMalloc((void **) &dA, sizeof(double) * N);
hipMalloc((void **) &dB, sizeof(double) * N);
...
hipSetDevice(nodeRank % deviceCount);
...
MPI_Send(dA, ...)
MPI_Recv(dB, ...)
gpu_kernel<<<gridsize, blocksize>>> (dB, N);
```

# Overlapping communication and computation

- Non-blocking MPI operations make it possible to start and complete communication in separate steps
    - A CPU may still be needed for the message progress $\Rightarrow$ overlapping
      CPU computation with communication not necessarily possible
- GPU is capable of concurrent computation and memory copies
- Host CPU is available for message progress $\Rightarrow$ more potential for 
  overlapping

# Overlapping communication and computation

<div class="column">
![](img/g2g-trace-no-overlap.png){width=80%}
</div>
<div class="column">
![](img/g2g-trace-overlap.png){width=80%}
</div>

<br>

```cpp
MPI_Isend(boundary_data, ...)
MPI_Irecv(boundary_data, ...)
compute_interior<<<gridsize, blocksize>>> (interior_data, ...);
MPI_Waitall(...)
compute_boundaries<<<gridsize, blocksize>>> (boundary_data, ...);
```


# Summary

- there are various options to write a multi-GPU program
- in HIP/OpenMP a device is set, and the subsequent calls operate on that device
- in SYCL each device has a separate queue
- often best to use one GPU per process + MPI for communications
- use direct peer to peer transfers when available in multithreaded cases
- GPU-aware MPI is required when passing device pointers to MPI

     * Using host pointers does not require any GPU awareness

- on LUMI GPU binding is important
  
