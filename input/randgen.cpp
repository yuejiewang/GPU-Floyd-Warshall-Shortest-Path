#include<iostream>
#include<unistd.h>
#include<fstream>
#include<random>
using namespace std;

int main(int argc, char** argv)
{
    int c;
    char* pstr = NULL;
    char* fstr = NULL;
    float upper = 0.0, lower = 0.0;
    while ((c = getopt(argc, argv, "s:p:f:u:l:")) != -1)
        switch (c)
        {
        case 's':
            unsigned int seed;
            sscanf(optarg, "%u", &seed);
            srand(seed);
            printf("seed=%u\n", seed);
            break;
        case 'p':
            pstr = optarg;
            break;
        case 'f':
            fstr = optarg;
            break;
        case 'u':
            sscanf(optarg, "%f", &upper);
            break;
        case 'l':
            sscanf(optarg, "%f", &lower);
            break;
        default:
            abort();
        }
    if (!pstr || !fstr) abort();
    unsigned long pow;
    unsigned long long N;
    sscanf(pstr, "%lu", &pow);
    N = 1 << pow;
    N *= N;
    printf("N=%llu\n", N);
    string filename(fstr);
    filename = "./p" + string(pstr) + "/" + filename + ".txt";
    ofstream wfile(filename);
    float range = upper > lower ? static_cast<float>(RAND_MAX) / (upper - lower) : 0.0;
    if (range == 0.0) {
        while (N > 0) {
            wfile << rand() << " ";
            N--;
        }
    }  // rand uint
    else {
        while (N > 0) {
            wfile << lower + (static_cast<float>(rand()) / range) << " ";
            N--;
        }
    }  // rand float in [lower, upper]
    return(0);
}
