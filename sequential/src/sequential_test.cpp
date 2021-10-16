#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<iostream>
#include<fstream>
#include<string>
#include<cstring>
#include<string.h>
#include"../../include/utils.h"
#include"../include/utils.h"
#include"../include/seq_mat_mul.h"
using namespace std;

int main(int argc, char** argv)
{
    char* pstr = NULL;
    char* astr = NULL;
    char* bstr = NULL;
    int c;
    while ((c = getopt(argc, argv, "p:A:B:C:")) != -1)
        switch (c)
        {
        case 'p':
            pstr = optarg;
            break;
        case 'A':
            astr = optarg;
            break;
        case 'B':
            bstr = optarg;
            break;
        default:
            abort();
        }

    if (!pstr or !astr or !bstr) abort();

    axis_t pow;
    sscanf(pstr, "%lu", &pow);
    axis_t N = (1 << pow);
    ifstream fileA, fileB;
    ofstream fileC;
    fileA.open(string(astr), ios::in);
    fileB.open(string(bstr), ios::in);
    float** mat_A = get_mat_space_sq_2d(N);
    float** mat_B = get_mat_space_sq_2d(N);
    float** mat_C = get_mat_space_sq_2d(N);
    
    for (axis_t x = 0; x < N; x++) {
        for (axis_t y = 0; y < N; y++) {
            index_t hidx = index2hilbert_2d(x, y, pow);
            printf("%5llu ", hidx);
            index_2d idx = hilbert2index_2d(hidx, pow);
            if (idx.row != x || idx.col != y) {
                printf("reverse convert failed at %llu, %lu, %lu\n", hidx, idx.row, idx.col);
                abort();
            }
        }
        printf("\n");
    }

    for (axis_t x = 0; x < N; x++) {
        for (axis_t y = 0; y < N; y++) {
            fileA >> mat_A[x][y];
            fileB >> mat_B[x][y];
        }
    }

    basic_matmul(mat_A, mat_B, mat_C, pow);
    for (axis_t x = 0; x < N; x++) {
        for (axis_t y = 0; y < N; y++) {
            printf("%3.4f ", mat_C[x][y]);
        }
        printf("\n");
    }

    free_mat_space(mat_A);
    free_mat_space(mat_B);
    free_mat_space(mat_C);
    fileA.close();
    fileB.close();
    return(0);
}
