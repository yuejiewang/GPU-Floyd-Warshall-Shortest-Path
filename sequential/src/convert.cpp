#include<iostream>
#include<fstream>
#include<unistd.h>
#include"../../include/utils.h"
#include"../include/utils.h"
#include"../include/seq_mat_mul.h"
using namespace std;

int main(int argc, char** argv)
{
    int c;
    char* pstr;
    char* fstr;
    while ((c = getopt(argc, argv, "p:m:")) != -1)
        switch (c)
        {
        case 'p':
            pstr = optarg;
            break;
        case 'm':
            fstr = optarg;
            break;
        default:
            abort();
        }
    axis_t pow;
    sscanf(pstr, "%lu", &pow);
    axis_t N = (1 << pow);
    ifstream fileM;
    fileM.open(fstr, ios::in);
    float* hilbert_matrix = get_mat_space_sq(N);
    for (axis_t i = 0; i < N; i++) {
        for (axis_t j = 0; j < N; j++) {
            fileM >> hilbert_matrix[index2hilbert_2d(i, j, pow)];
        }
    }
    for (index_t i = 0; i < N * N; i++) cout << hilbert_matrix[i] << " ";
    fileM.close();
    return(0);
}
