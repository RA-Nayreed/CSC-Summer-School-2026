<!--
SPDX-FileCopyrightText: 2010 CSC - IT Center for Science Ltd. <www.csc.fi>

SPDX-License-Identifier: CC-BY-4.0
-->

# General instructions for Mahti

## Logging in via ssh

You can access the [Mahti](https://docs.csc.fi/computing/systems-mahti/) supercomputer with ssh:

    ssh <username>@mahti.csc.fi

If you don't have the ssh key in the default location, you need to give a path to the key file:

    ssh -i <path-to-private-key> <username>@mahti.csc.fi

See [wiki](../../wiki/Setting-up-CSC-account-and-SSH) for further details.

## First-time setup

All the exercises should be carried out in the scratch disk area.
This scratch area is shared between all the project members, so create a personal working directory there:

    mkdir -p /scratch/project_2019219/$USER
    cd /scratch/project_2019219/$USER

and clone the summer school git repository there:

    git clone https://github.com/csc-training/summerschool.git /scratch/project_2019219/$USER/summerschool

Now, `/scratch/project_2019219/$USER/summerschool` is your own clone of the summer school repository on Mahti
and you can modify files there without causing conflicts with other summer school participants.

After this initial setup, the files remain on the system and in the subsequent terminal sessions you can go
directly to your summershool clone:

    cd /scratch/project_2019219/$USER/summerschool

<details>
<summary>Optional: Working with your own git fork</summary>

**We don't have time to teach git during the summer school, so this is recommended only if you are already somewhat familiar with git.**

It is recommended to fork the summer school repository on github and clone your own fork to Mahti instead.
This allows you to push your exercise solutions to a branch on your own fork.

In order to push commits to your own fork on Mahti, you can use your existing key on your laptop by enabling ssh agent forwarding (`ssh -A <username>@mahti.csc.fi`) *or* to add an SSH public key generated on Mahti to your github account.

Note that the default editor for commit messages is *vim*, if you prefer something else you can add, e.g.,

    export EDITOR=nano

to the file `$HOME/.bashrc`.

</details>

## Editors

For editing program source files you can use e.g. the *nano* editor:

    nano test.F90

(`^` in nano's shortcuts refer to **Ctrl** key, *i.e.* in order to save the file and exit the editor press `Ctrl+X`)
Also other popular editors such as *emacs* and *vim* are available.


## Web interface

- Mahti web interface can be accessed through <https://www.mahti.csc.fi>


## Compiling

### CPU programming

Mahti has several programming environments.

For CPU programming use (this is the default environment activated without loading any modules):
```bash
module load gcc/11.2.0 openmpi/4.1.2
```

#### MPI

Compilation of MPI programs in C, C++, and Fortran:
```bash
mpicc -O3 -Wall test.c -o test.x
mpicxx -O3 -Wall test.cpp -o test.x
mpif90 -O3 test.F90 -o test.x
```

The wrapper commands include automatically all the flags needed for building MPI programs.

#### MPI+OpenMP and pure OpenMP (threading with CPUs)

Both pure OpenMP and hybrid MPI+OpenMP programs can be compiled with the same wrappers
by including `-fopenmp` flag:
```bash
mpicc -fopenmp -O3 -Wall test.c -o test.x
mpicxx -fopenmp -O3 -Wall test.cpp -o test.x
mpif90 -fopenmp -O3 test.F90 -o test.x
```

#### HDF5

In order to use HDF5 in CSC supercomputers, you need the load the HDF5 module with MPI I/O support.
The appropriate module in Mahti:
```bash
module load hdf5/1.10.7-mpi
```

When building programs, `-lhdf5` (C/C++) or `-lhdf5_fortran` (Fortran) needs to be added to linker flags, e.g.
```bash
mpicxx -O3 -Wall test.cpp -lhdf5 -o test.x
mpif90 -O3 test.F90 -lhdf5_fortran -o test.x
```


### GPU programming

Mahti has several programming environments and we recommend
using different environments for CUDA and OpenMP offload.

#### CUDA and MPI+CUDA

For GPU programming with CUDA use:
```bash
module load gcc/11.2.0 openmpi/4.1.2-cuda cuda/11.5.0
```

Compilation of CUDA programs:
```bash
nvcc -O3 -gencode arch=compute_80,code=sm_80 test.cu -o test.x
```

Compilation of MPI+CUDA programs is a bit more complex as we need to include the MPI flags.
This can be automated by following lines (only the last line needs to be edited):
```bash
# Parse MPI options for compiler
Xcompiler="-Xcompiler $(mpicxx --showme | tr ' ' '\n' | sed '/^-Wl,/d;1d' | paste -sd, -)"

# Parse MPI options for linker
Xlinker="-Xlinker $(mpicxx --showme | tr ' ' '\n' | sed -n 's/^-Wl,//p' | paste -sd, -)"

# Compile MPI code using nvcc
nvcc -O3 -gencode arch=compute_80,code=sm_80 $Xcompiler $Xlinker test.cu -o test.x
```

### OpenMP offload and MPI+OpenMP offload

For GPU programming with OpenMP offload use:
```bash
module purge
module use /appl/opt/nvhpc/modulefiles
module load nvhpc-hpcx-cuda12/25.1
```

**Note!** This modification has implications on the consistency of the module tree,
see [CSC documentation](https://docs.csc.fi/computing/compiling-mahti/#openacc-and-openmp-offloading) for more information.


Compilation of OpenMP offload programs:
```bash
nvc -mp=gpu -O3 -gpu=cc80 test.c -o test.x
nvc++ -mp=gpu -O3 -gpu=cc80 test.cpp -o test.x
nvfortran -mp=gpu -O3 -gpu=cc80 test.F90 -o test.x
```

Compilation of MPI + OpenMP offload programs works with the usual wrappers in this environment:
```bash
mpicc -mp=gpu -O3 -gpu=cc80 test.c -o test.x
mpicxx -mp=gpu -O3 -gpu=cc80 test.cpp -o test.x
mpif90 -mp=gpu -O3 -gpu=cc80 test.F90 -o test.x
```


## Running

Programs need to be executed via the batch job system:
```bash
sbatch job.sh
```
The output of the job will be in the file `slurm-JOBID.out`. You can check the status of your jobs with `squeue --me` and kill possible hanging applications with `scancel JOBID`.

The file job script `job.sh` contains both the resource request (comment header lines starting with `#SBATCH`)
and the file is executed as a bash script in the allocation (lines starting with `#` are comments and ignored
during bash execution). Examples are provided below.

Note that you can override any of the options define in the `#SBATCH` header as command line options, for example:
```bash
sbatch --time=00:10:00 job.sh
```

### Slurm reservations

There is no dedicated slurm reservations on Mahti.
See LUMI for available reservations.


### CPU jobs

Example `job.sh` for running MPI+OpenMP program reserving 1 node, 4 tasks per node, and 2 CPU core per task, as well as 1 GB RAM per core, i.e., 8 CPU cores and 8 GB RAM within one node in total:

```bash
#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_2019219
#SBATCH --partition=small
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=4
#SBATCH --cpus-per-task=2
#SBATCH --mem-per-cpu=1G
#SBATCH --time=00:05:00

# Set the number of threads based on cpus-per-task
export OMP_NUM_THREADS=${SLURM_CPUS_PER_TASK:-1}

# Place and bind threads to single hardware threads
# Comment the following lines if binding is not desired
export OMP_PLACES=cores
export OMP_PROC_BIND=spread

# Run the program
srun ./prog.x
```

Rules of thumb for choosing the resources based on the parallelization type:
- MPI only: `--ntasks-per-node=<number_of_mpi_tasks>` and `--cpus-per-task=1`
- OpenMP only: `--ntasks-per-node=1` and `--cpus-per-task=<number_of_threads>`
- MPI+OpenMP: `--ntasks-per-node=<number_of_mpi_tasks>` and `--cpus-per-task=<number_of_threads_per_mpi_task>`

Note that other ways might be reasonable in some cases too.
Some of such cases will be discussed in the exercises.


### GPU jobs

Example `job.sh` for running a GPU program reserving 1 NVIDIA A100 GPU:

```bash
#!/bin/bash
#SBATCH --job-name=test
#SBATCH --account=project_2019219
#SBATCH --partition=gpusmall
#SBATCH --nodes=1
#SBATCH --ntasks-per-node=1
#SBATCH --cpus-per-task=2
#SBATCH --gres=gpu:a100:1
#SBATCH --time=00:05:00

# Run the program
srun ./prog.x
```

Note that this script allocates also 2 CPU cores per task.
These extra CPU cores are especially useful for OpenMP runtime.

For multi-GPU jobs using MPI:
- Change the number of MPI tasks and GPUs per node: `--ntasks-per-node=<number_of_mpi_tasks>` and `--gres=gpu:a100:<number_of_gpus>`


### Interactive jobs

Requesting an allocation:

```bash
salloc --account=project_2019219 --partition=small --nodes=1 --ntasks-per-node=1 --cpus-per-task=4 --time=00:30:00
```

Once the allocation is ready, you'll get a new shell on the login node.
In this shell, `srun` will launch jobs within the allocation:
```bash
srun --ntasks-per-node=2 --cpus-per-task=2 ./prog.x
```


### Interactive shell on a compute node

Create an interactive shell on a compute node:

```bash
srun --job-name=cn-shell --account=project_2019219 --partition=small --nodes=1 --ntasks-per-node=1 --cpus-per-task=2 --time=01:00:00 --pty ./tools/launch-cn-shell.sh
```

The script [launch-cn-shell.sh](tools/launch-cn-shell.sh) launches a bash shell
with the environment cleaned so that submitting slurm jobs works from the shell
in the same way as from the login node.


## Resources

- [Mahti documentation](https://docs.csc.fi/computing/systems-mahti/)
