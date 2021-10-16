#include"../include/utils.h"
using namespace std;

index_2d hilbert2index_2d(index_t hidx, axis_t pow) {
    if (pow == 0) return index_2d();
    pow -= 1;
    byte loc = hidx >> (pow << 1);
    index_t mask = (1 << (pow << 1)) - 1;
    index_2d subidx = hilbert2index_2d(hidx & mask, pow);
    axis_t row = subidx.row, col = subidx.col;
    switch (loc)
    {
    case 0x0:
        swap(row, col);
        break;
    case 0x1:
        col |= (1 << pow);
        break;
    case 0x2:
        row |= (1 << pow);
        col |= (1 << pow);
        break;
    case 0x3:
        row = (1 << pow) - 1 - row;
        col = (1 << pow) - 1 - col;
        swap(row, col);
        row |= (1 << pow);
        break;
    default:
        break;
    }
    return index_2d(row, col);
}
