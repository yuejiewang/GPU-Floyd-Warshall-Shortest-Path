#ifndef __UTIL_H
#define __UTIL_H

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cstring>
#include <string.h>

void readcsr(FILE* ifile, float* v, unsigned int* col, unsigned int* row,
             unsigned int n_nodes, unsigned int n_edges) {
  for (unsigned int i = 0; i < n_edges; i++) {
    fscanf(ifile, "%f\n", &v[i]);
  }
  for (unsigned int i = 0; i < n_edges; i++) {
    fscanf(ifile, "%d\n", &col[i]);
  }
  for (unsigned int i = 0; i <= n_nodes; i++) {
    fscanf(ifile, "%d\n", &row[i]);
  }
}

void csr2coo(unsigned int* row, unsigned int* csr_row, unsigned int n_node) {
  for (unsigned int i = 0; i < n_node; i++) {
    for (unsigned int j = csr_row[i]; j < csr_row[i + 1]; j++) {
      row[j] = i;
    }
  }
}

#endif
