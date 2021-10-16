#include"../include/seq_mat_mul.h"

void basic_matmul(float** A, float** B, float** C, axis_t p) {
    axis_t N = (1 << p);
    for (axis_t i = 0; i < N; i++) {
        for (axis_t j = 0; j < N; j++) {
            float sum = 0.0;
            for (axis_t k = 0; k < N; k++)
                sum += A[i][k] * B[k][j];
            C[i][j] = sum;
        }
    }
}

void hilbert_matmul(float* A, float* B, float* C, axis_t p) {
    return;
}