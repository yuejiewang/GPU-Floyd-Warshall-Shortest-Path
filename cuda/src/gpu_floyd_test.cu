#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<cstring>
#include<string.h>
#include"../../include/utils.h"
#include"../include/gpu_floyd.cuh"

int main(int argc, char** argv)
{
  if (argc != 3)
    exit(1);
  char* ifile_name = argv[1];
  char* ofile_name = argv[2];
  FILE* ifile = fopen(ifile_name, "r");
  FILE* ofile = fopen(ofile_name, "w+");
  if (!ifile || !ofile)
    exit(1);
  unsigned int V, E;
  fscanf(ifile, "%u\n", &V);
  fscanf(ifile, "%u\n", &E);
  unsigned int* src = (unsigned int*)malloc(E * sizeof(unsigned int));
  unsigned int* dst = (unsigned int*)malloc(E * sizeof(unsigned int));
  unsigned int* row = (unsigned int*)malloc(V * sizeof(unsigned int));
  float* w = (float*)malloc(E * sizeof(float));
  readcsr(ifile, w, dst, row, V, E);
  csr2coo(src, row, V);
  // float* D = gpu_floyd(src, dst, w, V, E);
  float* D = gpu_floyd_shared(src, dst, w, V, E);
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0; j < V; j++) {
      if (D[get_index(j, i, V)] == INFINITY) {
        // printf("   INF ");
        fprintf(ofile, "   INF ");
      }
      else {
        // printf("%6.2f ", D[get_index(i, j, V)]);
        fprintf(ofile, "%6.2f ", D[get_index(j, i, V)]);
      }
    }
    // printf("\n");
    fprintf(ofile, "\n");
  }
  fclose(ifile);
  fclose(ofile);
  free(src);
  free(dst);
  free(row);
  free(w);
  free(D);
  return 0;
}
