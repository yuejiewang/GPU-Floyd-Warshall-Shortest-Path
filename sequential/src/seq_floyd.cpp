#include "../include/seq_floyd.h"
#include <limits>

void seq_floyd(float** D, unsigned int V) {
  for (unsigned int k = 0; k < V; k++) {
    for (unsigned int i = 0; i < V; i++) {
      for (unsigned int j = 0; j < V; j++) {
        if (D[i][j] > D[i][k] + D[k][j]) D[i][j] = D[i][k] + D[k][j];
      }
    }
  }
}

void init_coo(unsigned int* src, unsigned int* dst, float* w, float** D,
              unsigned int V, unsigned int E) {
  for (unsigned int i = 0; i < V; i++) {
    for (unsigned int j = 0; j < V; j++) {
      D[i][j] = INF;
    }
  }
  for (unsigned int i = 0; i < V; i++) {
    D[i][i] = 0;
  }
  for (unsigned int j = 0; j < E; j++) {
    D[src[j]][dst[j]] = w[j];
  }
}
