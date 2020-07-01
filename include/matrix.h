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
// 5. adj, determinant methods for computing inverse of matrices
// 6. inverse, inverse_transpose of 4x4 matrices
// 7. transpose for 4x4 matrix

// multiplication of MxN matrix with Nx1 array
template <int M, int N, typename T>
Vector<M, T> operator *(const Vector<M, Vector<N, T> >& lhs, const Vector<N, T>& rhs) {
    Vector<M, T> tmp;
    for (int i = 0; i < M; i++) {
        tmp[i] = dot(lhs[i], rhs);
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

template <int M, int N, typename T>
Vector<M, Vector<N, T> > operator *(const Vector<M, Vector<N, T> >& lhs, const T& rhs) {
    Vector<M, Vector<N, T> > ret;
    for (int i = 0; i < M; i++) ret[i] = lhs[i] * rhs;
    return ret;
}

template <int N, typename T>
Vector<N, Vector<N, T> > operator *(const Vector<N, Vector<N, T> >& lhs, const T& rhs) {
    Vector<N, Vector<N, T> > ret;
    for (int i = 0; i < N; i++) ret[i] = lhs[i] * rhs;
    return ret;
}

template <int M, int N, typename T>
Vector<M, Vector<N, T> > operator /(const Vector<M, Vector<N, T> >& lhs, const T& rhs) {
    Vector<M, Vector<N, T> > ret;
    for (int i = 0; i < M; i++) ret[i] = lhs[i]/rhs;
    return ret;
}

template <int N, typename T>
Vector<N, Vector<N, T> > operator /(const Vector<N, Vector<N, T> >& lhs, const T& rhs) {
    Vector<N, Vector<N, T> > ret;
    for (int i = 0; i < N; i++) ret[i] = lhs[i]/rhs;
    return ret;
}

template <int N, typename T>
Vector<N, Vector<N, T> > identity(Vector<N, Vector<N, T>>& mat) {
    Vector< N, Vector<N, T> > ret;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            ret[i][j] = (i==j);
        }
    }
    return ret;
}

template <int M, int N, typename T>
Vector<N, Vector<M, T> > transpose(const Vector<M, Vector<N, T> >& mat) {
    Vector<N, Vector<M, T> > ret;
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            ret[j][i] = mat[i][j];
        }
    }
    return ret;
}

template <int N, typename T>
T getMinor(const Vector<N, Vector<N, T> >& mat, int row, int col) {
    Vector<N-1, Vector<N-1, T> > tmp;
    int drow, dcol;
    drow = 0;
    for (int i = 0; i < N; i++) {
        if (i == row) continue;         // excluding the rows and col which include given indices
        dcol = 0;
        for (int j = 0; j < N; j++) {
            if (j == col) continue;
            tmp[drow][dcol] = mat[i][j];
            dcol++;
        }
        drow++;
    }

    return determinant(tmp);
}

template <int N, typename T>
T determinant(const Vector<N,Vector<N, T> >& mat) {
    T sum = 0;
    
    for (int i = 0; i < N; i++) {
        T minor = getMinor(mat, i, 0);
        // std::cout << i << " " << minor << std::endl;
        sum += mat[i][0] * minor * ((i & 1) ? -1 : 1);        // check if odd or even
        
    }

    return sum;
}

template <typename T>
T determinant(const Vector<1, Vector<1, T> >& mat) {
    return mat[0][0];
}

template <typename T>
T determinant(const Vector<2, Vector<2, T> >& mat) {
    return mat[0][0]* mat[1][1]- mat[1][0]* mat[0][1];
}

template <typename T>
T determinant(const Vector<3, Vector<3, T> >& mat) {
    return
        mat[0][0] * (mat[1][1] * mat[2][2] - mat[2][1] * mat[1][2]) -
        mat[1][0] * (mat[0][1] * mat[2][2] - mat[2][1] * mat[0][2]) +
        mat[2][0] * (mat[0][1] * mat[1][2] - mat[1][1] * mat[0][2]);
}

template <int N, typename T>
Vector<N, Vector<N, T> > adj(const Vector<N, Vector<N, T> >& mat) {
    Vector<N, Vector<N, T> > ret;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            T minor = getMinor(mat, i, j);
            T cofactor = ((i + j) & 1) ? -minor : minor;
            ret[j][i] = cofactor;
        }
    }

    return ret;
}

template <int N, typename T>
Vector<N, Vector<N, T> > inverse(const Vector<N, Vector<N, T> >& mat) {
    T d = determinant(mat);

    // linearly dependent basis, should never happen in program.
    assert (d != 0);

    return adj(mat)/d;
}


// multiplication for 4x4 matrix and 3x1 vector by emdedding w-component as 1 in the vector
// helpful in numerous transformations involving multiplication of transformation matrices with 3d vectors
template <typename T>
Vector<3, T> multMatrixVec(const Vector<4, Vector<4, T> >& mat, const Vector<3, T>& vec) {
    // this function takes into consideration the w coordinate and performs
    // homogenous divide in the resulting vector
    Vector3f ret;
    T a, b, c, w;
    a = vec[0] * mat[0][0] + vec[1] * mat[0][1] + vec[2] * mat[0][2] + mat[0][3];
    b = vec[0] * mat[1][0] + vec[1] * mat[1][1] + vec[2] * mat[1][2] + mat[1][3];
    c = vec[0] * mat[2][0] + vec[1] * mat[2][1] + vec[2] * mat[2][2] + mat[2][3];
    w = vec[0] * mat[3][0] + vec[1] * mat[3][1] + vec[2] * mat[3][2] + mat[3][3];

    ret.x = a/w;
    ret.y = b/w;
    ret.z = c/w;

    return ret;
}

template <typename T>
Vector<3, T> multMatrixDir(const Vector<4, Vector<4, T> >& mat, const Vector<3, T>& vec) {
    // this function simply ignores the w dimension i.e. doesn't take into account the translation
    
    Vector3f ret;
    
    ret.x = vec[0] * mat[0][0] + vec[1] * mat[0][1] + vec[2] * mat[0][2];
    ret.y = vec[0] * mat[1][0] + vec[1] * mat[1][1] + vec[2] * mat[1][2];
    ret.z = vec[0] * mat[2][0] + vec[1] * mat[2][1] + vec[2] * mat[2][2];

    return ret;
}

typedef Vector<3, Vector<3, int> > Matrix3i;
typedef Vector<4, Vector<4, int> > Matrix4i;
typedef Vector<3, Vector<3, float> > Matrix3f;
typedef Vector<4, Vector<4, float> > Matrix4f;

#endif      // MATRIX_H