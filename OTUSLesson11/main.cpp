#include <iostream>

#include "dok_sparse_matrix.h"
#include "lil_sparse_matrix.h"
#include "multidimensional_sparse_matrix.h"

using namespace std;

int main(int argc, char* argv[]) {
    SparseMatrixDOK<int, 0> m;
    //SparseMatrixLIL<int, 0> m;
    //MultiSparseMatrixDOK<int, 0, 2> m;
    constexpr int N = 10;

    // fill two diagonal values
    for (int i = 0; i < N; ++i) {
        m[i][i] = m[N-1-i][i] = i; // N = 10, i = [0, N)
    }

    // fill zeroes in chess-like order
    // 30 operations: 10 + (10*10/2 - 10)/2
    for (int i = 0; i < N; ++i) {
        for (int j = i; j < N; j += 2) {
            m[i][j] = 0;
            if (i != j) {
                m[j][i] = 0;
            }
        }
    }

    // output [1,1] -- [8,8]
    for (int i = 1; i < 9; ++i) {
        for (int j = 1; j < 9; ++j) {
            cout << m[i][j] << ' ';
        }
        cout << '\n';
    }
    cout << m.size() << endl;

    // output all values
    for (auto p : m) {
        int row, col, val;
        tie(row, col, val) = p;
        cout << row << ' ' << col << ' ' << val << '\n';
    }

    return 0;
}
