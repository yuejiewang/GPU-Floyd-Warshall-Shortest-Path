#include<stdlib.h>
#include<stdio.h>
#include<string>
#include<cstring>
#include<string.h>
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
  fscanf(ifile, "%lu %lu\n", &V, &E);
  unsigned int* src = (unsigned int*)malloc(E * sizeof(unsigned int));
  unsigned int* dst = (unsigned int*)malloc(E * sizeof(unsigned int));
  float* w = (float*)malloc(E * sizeof(float));
  for (unsigned int i = 0; i < E; i++) {
    fscanf(ifile, "%lu", &src[i]);
  }
  for (unsigned int i = 0; i < E; i++) {
    fscanf(ifile, "%lu", &dst[i]);
  }
  for (unsigned int i = 0; i < E; i++) {
    fscanf(ifile, "%f", &w[i]);
  }
  float* D = gpu_floyd(src, dst, w, V, E);
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0; j < V; j++) {
      if (D[get_index(i, j, V)] == INF) fprintf(ofile, "     INF ");
      else fprintf(ofile, "%8.3f ", D[get_index(i, j, V)]);
    }
    fprintf(ofile, "\n");
  }
  fclose(ifile);
  fclose(ofile);
  free(src);
  free(dst);
  free(w);
  free(D);
  return 0;
}
