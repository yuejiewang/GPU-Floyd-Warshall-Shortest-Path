#include<stdlib.h>
#include<stdio.h>
#include"../utils.hpp"
#include"./utils.hpp"
using namespace std;

int main()
{
    axis_t pow = 4;
    axis_t N = (1 << pow);
    index_t** mat = new index_t* [N];
    for (index_t i = 0; i < N; i++) {
        mat[N] = new index_t [N];
    }

    for (axis_t x = 0; x < N; x++) {
        for (axis_t y = 0; y < N; y++) {
            printf("%5llu ", index2hilbert_2d(x, y, pow));
        }
        printf("\n");
    }

    return(0);
}
