#include<stdlib.h>
#include<stdio.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<string.h>
#include"../../include/utils.h"
#include"../include/utils.h"
#include"../include/seq_floyd.h"
using namespace std;

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
  unsigned int V;
  unsigned int E;
  fscanf(ifile, "%u\n", &V);
  fscanf(ifile, "%u\n", &E);
  unsigned int* src = (unsigned int*)malloc(E * sizeof(unsigned int));
  unsigned int* dst = (unsigned int*)malloc(E * sizeof(unsigned int));
  unsigned int* row = (unsigned int*)malloc(V * sizeof(unsigned int));
  float* w = (float*)malloc(E * sizeof(float));
  readcsr(ifile, w, dst, row, V, E);
  csr2coo(src, row, V);
  
  /*
  printf("row: \n");
  for (int i = 0; i < V; i++) {
    printf("%d ", row[i]);
  }
  printf("\n"); */
  float** D = new float*[V];
  for (unsigned i = 0; i < V; i++) {
    D[i] = new float[V];
  }

  /*
  printf("src: \n");
  for (int i = 0; i < E; i++) {
    printf("%d ", src[i]);
  }
  printf("\n");
  printf("dst: \n");
  for (int i = 0; i < E; i++) {
    printf("%d ", dst[i]);
  }
  printf("\n");
  printf("dist: \n");
  for (int i = 0; i < E; i++) {
    printf("%5.1f ", w[i]);
  }
  printf("\n\n"); */

  init_coo(src, dst, w, D, V, E);
  /*
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0; j < V; j++) {
      printf("%4.1f ", D[i][j]);
    }
    printf("\n");
  }
  printf("\n"); */

  seq_floyd(D, V);
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0 ; j < V; j++) {
      if (D[i][j] == INF) {
        // printf("   INF ");
        fprintf(ofile, "   INF ");
      }
      else {
        // printf("%6.2f ", D[i][j]);
        fprintf(ofile, "%6.2f ", D[i][j]);
      }
    }
    // printf("\n");
    fprintf(ofile, "\n");
  }

  for (unsigned i = 0; i < V; i++) {
    delete D[i];
  }
  delete D;
  free(src);
  free(dst);
  free(row);
  free(w);
  fclose(ifile);
  fclose(ofile);
  return 0;
}
