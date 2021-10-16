#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include"../utils.hpp"
#include"./utils.hpp"
using namespace std;

int main(int argc, char** argv)
{
    char* pstr = NULL;
    int c;
    while ((c = getopt(argc, argv, "p:")) != -1)
        switch (c)
        {
        case 'p':
            pstr = optarg;
            break;
        default:
            abort();
        }
    
    axis_t pow;
    sscanf(pstr, "%lu", &pow);
    axis_t N = (1 << pow);
    index_t** mat = new index_t* [N];
    for (index_t i = 0; i < N; i++) {
        mat[N] = new index_t [N];
    }
    
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

    return(0);
}
