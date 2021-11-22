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
  unsigned int src[7] = {0, 1, 1, 1, 3, 4, 4};
  unsigned int dst[7] = {4, 0, 2, 3, 2, 1, 3};
  float w[7] = {0.5, 2.2, 3.1, -1, 4.2, 1.5, -2};
  unsigned int V = 5;
  unsigned int E = 7;
  float** D = new float*[V];
  for (unsigned i = 0; i < V; i++) {
    D[i] = new float[V];
  }
  init_coo(src, dst, w, D, V, E);
  seq_floyd(D, V);
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0 ; j < V; j++) {
      if (D[i][j] == INF) printf("   INF  ");
      else printf("%6.3f  ", D[i][j]);
    }
    printf("\n");
  }
  for (unsigned i = 0; i < V; i++) {
    delete D[i];
  }
  delete D;
  return 0;
}
