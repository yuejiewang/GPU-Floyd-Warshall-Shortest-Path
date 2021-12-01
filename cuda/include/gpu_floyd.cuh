#ifndef __GPU_FLOYD_CUH
#define __GPU_FLOYD_CUH

#include <cuda.h>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#define n_thread 32
#define n_unroll 16
#define get_index(x, y, s) (y * s + x)
#define div_up(x, y) ((x + y - 1) / y)
#define CHK(x, info)                                                \
  {                                                                 \
    x;                                                              \
    cudaDeviceSynchronize();                                        \
    cudaError_t err = cudaGetLastError();                           \
    if (err != cudaSuccess) {                                       \
      printf("cuda error %s: %s\n", info, cudaGetErrorString(err)); \
      exit(1);                                                      \
    }                                                               \
  }

__global__ void floyd_kernel(float* D, unsigned int V, unsigned int k);
__global__ void floyd_shared_kernel(float* D, signed int V, unsigned int k);
__global__ void init_kernel(float* D, unsigned int V, float inf);
__global__ void init_kernel_unroll(float* D, unsigned int V, float inf);
__global__ void const_kernel(unsigned int* src, unsigned int* dst, float* w,
                             float* D, unsigned int V);
__host__ float* gpu_floyd(unsigned int* src, unsigned int* dst, float* w,
                        unsigned int V, unsigned int E);
__host__ float* gpu_floyd_shared(unsigned int* src, unsigned int* dst, float* w,
                                unsigned int V, unsigned int E);

#endif
