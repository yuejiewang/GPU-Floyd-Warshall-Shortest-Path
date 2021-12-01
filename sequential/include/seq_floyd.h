#ifndef __SEQ_FLOYD_H
#define __SEQ_FLOYD_H

#include <limits>
#define INF std::numeric_limits<float>::infinity()
void seq_floyd(float** D, unsigned int V);
void init_coo(unsigned int* src, unsigned int* dst, float* w, float** D,
              unsigned int V, unsigned int E);

#endif
