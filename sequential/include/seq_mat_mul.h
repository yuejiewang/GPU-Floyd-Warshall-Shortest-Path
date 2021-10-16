#ifndef __SEQ_MAT_MUL_H
#define __SEQ_MAT_MUL_H
#include<string.h>
#include"../../include/utils.h"

inline float* get_mat_space(index_t len);
inline float* get_mat_space_sq(axis_t d);
inline float** get_mat_space_2d(axis_t row, axis_t col);
inline float** get_mat_space_sq_2d(axis_t d);
inline void free_mat_space(float* m);
inline void free_mat_space(float** m);

inline float* get_mat_space(index_t len) {
    float* mat = new float [len];
    memset(mat, 0, sizeof(mat));
    return mat;
}

inline float* get_mat_space_sq(axis_t d) {
    return get_mat_space((index_t)d * (index_t)d);
}

inline float** get_mat_space_2d(axis_t row, axis_t col) {
    float** mat = new float* [row];
    for (size_t i = 0; i < row; i++) {
        mat[i] = new float [col];
        memset(mat[i], 0, sizeof(mat[i]));
    }
    return mat;
}

inline float** get_mat_space_sq_2d(axis_t d) {
    return get_mat_space_2d(d, d);
}

inline void free_mat_space(float* m) {
    if (m) delete m;
}

inline void free_mat_space(float** m) {
    if (!m) return;
    size_t rowcnt = sizeof(m) / sizeof(m[0]);
    for (int i = rowcnt - 1; i >= 0; i--) {
        delete m[i];
    }
    delete m;
}

#endif
