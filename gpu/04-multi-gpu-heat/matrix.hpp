#pragma once
#include <vector>
#include <cassert>
#include <hip/hip_runtime.h>
#include "../error_checking.hpp"

// Generic 2D matrix array class.
//
// Internally data is stored in 1D vector but is
// accessed using index function that maps i and j
// indices to an element in the flat data vector.
// Row major storage is used
// For easier usage, we overload parentheses () operator
// for accessing matrix elements in the usual (i,j)
// format.

enum storage_spec {
  DEVICE_ONLY,
  HOST_ONLY,
  HOST_AND_DEVICE
};

template<typename T>
struct MatrixView {
  T* _data;
  const size_t nrows, ncols;
  const storage_spec mem_location;

  __host__ __device__ T& operator()(size_t i, size_t j) {
    return data[i*ncols+j]; 
  }
}

template<typename T>
class Matrix
{

private:

    // Internal storage
    T* _data;

    // Internal 1D indexing (row major)
    __host__ __device__ int indx(int i, int j) const {
      return i * ncols + j;
    }

    void allocate(){
      switch(mem_location) {
        case DEVICE_ONLY:
          HIP_ERRCHK(hipMalloc(&_data, sizeof(T)*nrows*ncols));
          break;
        case HOST_ONLY:
          _data = new T[nrows*ncols];
          break;
        case HOST_AND_DEVICE:
          HIP_ERRCHK(hipMallocManaged(&_data, sizeof(T)*nrows*ncols));
          break;
      }
    }

public:

    const size_t nrows, ncols;
    const storage_spec mem_location;

    MatrixView<T> view() const { return MatrixView<T>(_data, nrows, ncols, mem_location); }

    Matrix(const Matrix&) = delete; // Delete copy constructor

    // Make a deep copy from a view
    Matrix(MatrixView<T> view, storage_spec mem_location) : mem_location(mem_location), nrows(view.nrows), ncols(view.ncols) {
      allocate();
      HIP_ERRCHK(hipMemCpy(_data, view.data, sizeof(T)*view.nrows*view.ncols, hipMemCpyDefault));
    }
    
    // Allocate at the time of construction
    Matrix(size_t nrows, size_t ncols, storage_spec mem_location) : nrows(nrows), ncols(ncols), mem_location(mem_location) {
      allocate();
    }

    ~Matrix() {
      if (_data) {
        if (mem_location == HOST_ONLY) { 
          delete[] _data; 
        }
        else {
          hipFree(_data);
        }
      }
    }

    // standard (i,j) syntax for setting elements
    __host__ __device__ inline T& operator()(int i, int j) {
        return _data[ indx(i, j) ];
    }

    // standard (i,j) syntax for getting elements
    __host__ __device__ inline const T& operator()(int i, int j) const {
        return _data[ indx(i, j) ];
    }

    // provide possibility to get raw pointer for data at index (i,j) (needed for MPI)
    __host__ __device__ T* data(int i=0, int j=0) {return _data + indx(i,j);}
};
