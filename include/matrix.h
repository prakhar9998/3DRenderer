#ifndef MATRIX_H
#define MATRIX_H

#include "vector3d.h"

/* 
 * Design: Consider vector3d as building blocks of matrices.
 * and define every operation in terms of vectors
 * No need of overload [] operators
 * not need to check dimension mismatch
 * rather the compiler will throw error for the templates not being defined
 * This features the most used and required operations and 
 * does not contain every matrix operation known.
*/
// 1. Define matrix in terms of vectors
// 2. Multiplication of MxN with Nx1 matrices
// 3. Specializations for common methods in 3x3 and 4x4 matrices
// 4. set NxN matrix to be identity
// 5. adj, det methods for computing inverse of matrices
// 6. inverse, inverse_transpose of 4x4 matrices
// 7. transpose for 4x4 matrix

// multiplication of MxN matrix with Nx1 array
template <int M, int N, typename T>
Vector<M, T> operator *(const Vector<M, Vector<N, T> >& lhs, const Vector<N, T>& rhs) {
    Vector<M, T> tmp;
    for (int i = 0; i < M; i++) {
        tmp[i] = dot(lhs[m], rhs);
    }
}

// specialization for 3x3 matrix and 3x1 vector
template <typename T>
Vector<3, T> operator *(const Vector<3, Vector<3, T> >& lhs, const Vector<3, T>& rhs) {
    return Vector<3, T>(dot(lhs[0], rhs), dot(lhs[1], rhs), dot(lhs[2], rhs));
}

template <typename T>
Vector<4, T> operator *(const Vector<4, Vector<4, T> >& lhs, const Vector<4, T>& rhs) {
    return Vector<4, T>(
        dot(lhs[0], rhs),
        dot(lhs[1], rhs),
        dot(lhs[2], rhs),
        dot(lhs[3], rhs)
    );
}

// generalized matrix multiplication
// multiplication of MxN with NxK
template <int M, int N, int K, typename T>
Vector<M, Vector<K, T> > operator *(const Vector<M, Vector<N, T> >& lhs, const Vector<N, Vector<K, T> >& rhs) {
    Vector<M, Vector<K, T> > ret;
    T c;
    for (int i = 0; i < M; i++) {
        for (int k = 0; k < K; k++) {
            c = 0;
            for (int j = 0; j < N; j++) {
                c += lhs[i][j] * rhs[j][k];
            }
            ret[i][k] = c;
        }
    }

    return ret;
}

template <int N, typename T>
Vector<N, Vector<N, T> > identity(Vector<N, Vector<N, T>>& mat) {
    Vector< N, Vector<N, T> > ret;
    for (int i = 0; i < N; i++) {
        for (int i = 0; i < N; i++) {
            ret[i][j] = (i==j);
        }
    }
    return ret;
}

template <int N, typename T>
Vector<N, Vector<M, T> > transpose(const Vector<M, Vector<N, T> >& mat) {
    Vector<N, Vector<M, T> > ret;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            ret[j][i] = mat[i][j];
        }
    }
    return ret;
}

// template <int N, typename T>
// T minor(const Vector<N, Vector<N, T> >& mat, int row, int col) {
//     Vector<N-1, Vector<N-1, T> > tmp;
    
// }

// template <int N, typename T>
// T det(const Vector<N,Vector<N, T> >& mat) {
//     T sum = 0;
//     for (int i = 0; i < N; i++) {
//         T minor = minor(mat, n, 0);
//     }
// }

typedef Vector<3, Vector<3, int> > Matrix3i;
typedef Vector<4, Vector<4, int> > Matrix4i;
typedef Vector<3, Vector<3, float> > Matrix3i;
typedef Vector<4, Vector<4, float> > Matrix4i;

#endif      // MATRIX_H