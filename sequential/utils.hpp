#include"../utils.hpp"

struct index_2d {
    axis_t row;
    axis_t col;
    index_2d() {
        row = 0;
        col = 0;
    }
    index_2d(axis_t _r, axis_t _c) {
        row = _r;
        col = _c;
    }
};

template<class T>
inline void swap(T &a, T &b) {
    a += b;
    b = a - b;
    a -= b;
}

// hilbert index of a [(2 ^ pow), (2 ^ pow)] matrix has (2 * pow) bits
// row(col) < 2 ^ 32, pow <= 32
inline index_t index2hilbert_2d(axis_t row, axis_t col, axis_t pow) {
    index_t hidx = 0;
    axis_t subrow = row, subcol = col, mask = (1 << pow) - 1;
    for (int i = pow - 1; i >= 0; i--) {
        // the left most bit of row and col together decides which subpart the node goes in
        // each subpart decides 2 left most bits of the hilbert index
        // subrow(subcol) = row(col) % (2 ^ i)
        mask >>= 1;  // lower i bits set to 1, other bits 0
        byte loc = ((byte)(subrow >> i << 1) | (byte)(subcol >> i));
        subrow &= mask;  // lower i bits of row
        subcol &= mask;  // lower i bits of col
        // printf("i = %lu , ", i);
        // return 1;

        switch (loc)
        {
        case 0x0:
            // subrow += subcol;
            // subcol = subrow - subcol;
            // subrow -= subcol;
            swap<axis_t>(subrow, subcol);
            break;
        case 0x1:
            hidx |= ((index_t)1 << (i << 1));  // the right (i * 2 + 1)th and (i * 2)th bits are 01, 2nd to visit
            break;
        case 0x2:
            hidx |= ((index_t)3 << (i << 1));  // the bits are 10, 4th to visit
            subrow = (1 << i) - 1 - subrow;
            subcol = (1 << i) - 1 - subcol;
            swap<axis_t>(subrow, subcol);
            // printf("|| subrow = %lu, subcol = %lu,", subrow, subcol);
            break;
        case 0x3:
            hidx |= ((index_t)2 << (i << 1));  // the bits are 11, 3rd to visit
            break;
        default:
            exit(1);
        }
    }
    return hidx;
}

inline index_t index2hilbert_2d(index_2d idx, axis_t pow) {
    return index2hilbert_2d(idx.row, idx.col, pow);
}

inline index_t index2hilbert_2d(index_t index, axis_t pow) {
    return index2hilbert_2d((index >> pow), index & ((1 << pow) - 1), pow);
}

// any loop version?
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
        exit(1);
    }
    return index_2d(row, col);
}