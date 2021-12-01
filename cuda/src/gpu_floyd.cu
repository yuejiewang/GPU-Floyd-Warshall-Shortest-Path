#include <cuda.h>
#include "../include/gpu_floyd.cuh"

__global__ void floyd_kernel(float* D, unsigned int V, unsigned int k) {
  if (blockDim.x * blockIdx.x + threadIdx.x < V && blockDim.y * blockIdx.y + threadIdx.y < V &&
      D[get_index((blockDim.x * blockIdx.x + threadIdx.x), (blockDim.y * blockIdx.y + threadIdx.y), V)] >
      D[get_index((blockDim.x * blockIdx.x + threadIdx.x), k, V)] +
      D[get_index(k, (blockDim.y * blockIdx.y + threadIdx.y), V)]) {
    D[get_index((blockDim.x * blockIdx.x + threadIdx.x), (blockDim.y * blockIdx.y + threadIdx.y), V)] =
        D[get_index((blockDim.x * blockIdx.x + threadIdx.x), k, V)] +
        D[get_index(k, (blockDim.y * blockIdx.y + threadIdx.y), V)];
  }
}

__global__ void floyd_shared_kernel(float* D, unsigned int V, unsigned int k) {
  // smem load
  __shared__ float k_x[n_thread];
  __shared__ float y_k[n_thread];
  if (threadIdx.y == 0) {
    if (blockDim.x * blockIdx.x + threadIdx.x < V) {
      k_x[threadIdx.x] = D[get_index((blockDim.x * blockIdx.x + threadIdx.x), k, V)];
    }
  }
  if (threadIdx.y == 1) {
    if (blockDim.y * blockIdx.y + threadIdx.x < V) {
      y_k[threadIdx.x] = D[get_index(k, (blockDim.y * blockIdx.y + threadIdx.x), V)];
    }
  }

  __syncthreads();
  if (blockDim.x * blockIdx.x + threadIdx.x < V && blockDim.y * blockIdx.y + threadIdx.y < V &&
      D[get_index((blockDim.x * blockIdx.x + threadIdx.x), (blockDim.y * blockIdx.y + threadIdx.y), V)] >
      k_x[threadIdx.x] + y_k[threadIdx.y]) {
    D[get_index((blockDim.x * blockIdx.x + threadIdx.x), (blockDim.y * blockIdx.y + threadIdx.y), V)] =
        k_x[threadIdx.x] + y_k[threadIdx.y];
  }
}
/*
__global__ void floyd_shared_kernel(float* D, unsigned int V, unsigned int k) {
  // smem load
  __shared__ float k_x[n_thread * n_unroll];
  __shared__ float y_k[n_thread * n_unroll];
  unsigned int tid = blockDim.x * threadIdx.y + threadIdx.x;
  if (threadIdx.y < (n_thread / 2)) {
    if (blockIdx.x * n_thread * n_unroll + tid < V)
      k_x[tid] = D[get_index((blockIdx.x * n_thread * n_unroll + tid), k, V)];
  } else {
    tid -= n_thread * n_unroll;
    if (blockIdx.y * n_thread * n_unroll + tid < V)
      y_k[tid] = D[get_index(k, (blockIdx.y * n_thread * n_unroll + tid), V)];
  }

  __syncthreads();
#pragma unroll
  for (unsigned int y = 0; y < n_unroll; y++) {
#pragma unroll
    for (unsigned int x = 0; x < n_unroll; x++) {
      if (blockDim.x * blockIdx.x * n_unroll + x * n_thread + threadIdx.x < V &&
          blockDim.y * blockIdx.y * n_unroll + y * n_thread + threadIdx.y < V &&
          D[get_index(
              (blockDim.x * blockIdx.x * n_unroll + x * n_thread + threadIdx.x),
              (blockDim.y * blockIdx.y * n_unroll + y * n_thread + threadIdx.y),
              V)] > k_x[threadIdx.x + n_thread * x] +
                        y_k[threadIdx.y + n_thread * y]) {
        D[get_index(
            (blockDim.x * blockIdx.x * n_unroll + x * n_thread + threadIdx.x),
            (blockDim.y * blockIdx.y * n_unroll + y * n_thread + threadIdx.y),
            V)] =
            k_x[threadIdx.x + n_thread * x] + y_k[threadIdx.y + n_thread * y];
      }
    }
  }
}
*/
__global__ void init_kernel(float* D, unsigned int V, float inf) {
  D[get_index(
      (blockIdx.x * blockDim.x + threadIdx.x),
      (blockIdx.y * blockDim.y + threadIdx.y), V)] =
      inf;
}

__global__ void init_kernel_unroll(float* D, unsigned int V, float inf) {
#pragma unroll
  for (unsigned int y = 0; y < n_unroll; y++) {
#pragma unroll
    for (unsigned int x = 0; x < n_unroll; x++) {
      D[get_index(
          (blockIdx.x * blockDim.x * n_unroll + x * n_thread + threadIdx.x),
          (blockIdx.y * blockDim.y * n_unroll + y * n_thread + threadIdx.y), V)] =
          inf;
    }
  }
}

__global__ void const_kernel(unsigned int* src, unsigned int* dst, float* w,
                             float* D, unsigned int V) {
  D[get_index(dst[blockIdx.x * blockDim.x + threadIdx.x],
              src[blockIdx.x * blockDim.x + threadIdx.x], V)] =
      w[blockIdx.x * blockDim.x + threadIdx.x];
}

__global__ void zero_kernel(float* D, unsigned int V) {
  D[get_index((blockIdx.x * blockDim.x + threadIdx.x),
              (blockIdx.x * blockDim.x + threadIdx.x), V)] = 0.0;
}

__host__ float* gpu_floyd(unsigned int* src, unsigned int* dst, float* w,
                        unsigned int V, unsigned int E) {
  float* hmem = (float*)malloc(V * V * sizeof(float));
  float* D;
  unsigned int* d_src;
  unsigned int* d_dst;
  float* d_w;
  CHK(cudaMalloc(&D, V * V * sizeof(float)), "cudaMalloc");
  CHK(cudaMalloc(&d_src, E * sizeof(unsigned int)), "cudaMalloc_src");
  CHK(cudaMalloc(&d_dst, E * sizeof(unsigned int)), "cudaMalloc_dst");
  CHK(cudaMalloc(&d_w, E * sizeof(float)), "cudaMalloc_w");
  CHK(cudaMemset(D, 0, V * V * sizeof(float)), "cudaMemset");
  CHK(cudaMemcpy(d_src, src, E * sizeof(unsigned int), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_src");
  CHK(cudaMemcpy(d_dst, dst, E * sizeof(unsigned int), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_dst");
  CHK(cudaMemcpy(d_w, w, E * sizeof(float), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_w");

  // configuration
  dim3 b_conf(n_thread, n_thread);
  unsigned int n_blocks = div_up(V, n_thread);
  dim3 g_conf(n_blocks, n_blocks);

  // initialize D (construct from src, dst, w) on GPU
  CHK((init_kernel<<<g_conf, b_conf>>>(D, V, INFINITY)), "initialization");
  CHK((const_kernel<<<div_up(E, 1024), 1024>>>(d_src, d_dst, d_w, D, V)),
      "construct");
  unsigned int v_block = V < 1024 ? V : 1024;
  CHK((zero_kernel<<<div_up(V, v_block), v_block>>>(D, V)), "zero");

  for (unsigned int i = 0; i < V; i++) {
    // one kernel for each interation of k
    CHK((floyd_kernel<<<g_conf, b_conf>>>(D, V, i)), "kernel");
  }
  CHK(cudaMemcpy(hmem, D, V * V * sizeof(float), cudaMemcpyDeviceToHost),
      "cudaMemcpyD2H");
  CHK(cudaFree(D), "freeD");
  CHK(cudaFree(d_src), "freesrc");
  CHK(cudaFree(d_dst), "freedst");
  CHK(cudaFree(d_w), "freedist");
  return hmem;
}

__host__ float* gpu_floyd_shared(unsigned int* src, unsigned int* dst, float* w,
                        unsigned int V, unsigned int E) {
  float* hmem = (float*)malloc(V * V * sizeof(float));
  float* D;
  unsigned int* d_src;
  unsigned int* d_dst;
  float* d_w;
  CHK(cudaMalloc(&D, V * V * sizeof(float)), "cudaMalloc");
  CHK(cudaMalloc(&d_src, E * sizeof(unsigned int)), "cudaMalloc_src");
  CHK(cudaMalloc(&d_dst, E * sizeof(unsigned int)), "cudaMalloc_dst");
  CHK(cudaMalloc(&d_w, E * sizeof(float)), "cudaMalloc_w");
  CHK(cudaMemset(D, 0, V * V * sizeof(float)), "cudaMemset");
  CHK(cudaMemcpy(d_src, src, E * sizeof(unsigned int), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_src");
  CHK(cudaMemcpy(d_dst, dst, E * sizeof(unsigned int), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_dst");
  CHK(cudaMemcpy(d_w, w, E * sizeof(float), cudaMemcpyHostToDevice),
      "cudaMemcpyH2D_w");

  // configuration
  dim3 b_conf(n_thread, n_thread);
  // unsigned int n_blocks = div_up(V, (n_thread * n_unroll));
  unsigned int n_blocks = div_up(V, n_thread);
  dim3 g_conf(n_blocks, n_blocks);

  // initialize D (construct from src, dst, w) on GPU
  CHK((init_kernel<<<g_conf, b_conf>>>(D, V, INFINITY)), "initialization");
  CHK((const_kernel<<<div_up(E, 1024), 1024>>>(d_src, d_dst, d_w, D, V)),
      "construct");
  unsigned int v_block = V < 1024 ? V : 1024;
  CHK((zero_kernel<<<div_up(V, v_block), v_block>>>(D, V)), "zero");

  for (unsigned int i = 0; i < V; i++) {
    // one kernel for each interation of k
    CHK((floyd_shared_kernel<<<g_conf, b_conf>>>(D, V, i)), "kernel");
  }
  CHK(cudaMemcpy(hmem, D, V * V * sizeof(float), cudaMemcpyDeviceToHost),
      "cudaMemcpyD2H");
  CHK(cudaFree(D), "freeD");
  CHK(cudaFree(d_src), "freesrc");
  CHK(cudaFree(d_dst), "freedst");
  CHK(cudaFree(d_w), "freedist");
  return hmem;
}
