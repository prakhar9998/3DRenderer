#ifndef VECTOR3D_H
#define VECTOR3D_H

#include <iostream>
#include <cassert>

template <int N, typename T>
class Vector {
public:
    T data[N];

    Vector();
    Vector(const T d[]);

    T& operator[] (size_t index);
    const T& operator[] (size_t index) const;
};

template<typename T>
class Vector<2, T> {
public:
    T x, y;

    Vector();
    Vector(T x, T y);

    T& operator[] (size_t index);
    const T& operator[] (size_t index) const;
};

template <typename T>
class Vector<3, T> {
public:
    T x, y, z;

    Vector();
    Vector(T x, T y, T z);

    T& operator[] (size_t index);
    const T& operator[] (size_t index) const;
};

template<int N, typename T>
Vector<N, T> operator +(const Vector<N, T>& left, const Vector<N, T>& right);

template <int N, typename T>
Vector<N, T> operator -(const Vector<N, T>& left, const Vector<N, T>& right);

// Element-wise product of two vectors
template <int N, typename T>
Vector<N, T> operator *(const Vector<N, T>& left, const Vector<N, T>& right);

// output the vector values, for debugging purposes
template <int N, typename T>
std::ostream& operator <<(std::ostream& out, Vector<N, T>& vector);

// Dot product of two vectors
template <int N, typename T>
T dot(const Vector<N, T>& left, const Vector<N, T>& right);

// Cross product of two Vector3T vectors
template <typename T>
Vector<3, T> cross(Vector<3, T>& left, Vector<3, T>& right);

////////////////////////////////////////////////////////////////////////////////

template <int N, typename T>
Vector<N, T>::Vector()
: x(0), y(0) {}

template <int N, typename T>
Vector<N, T>::Vector(const T d[]) {
    for (int i = 0; i < N; i++) {
        data[i] = d[i];
    }
}

template <int N, typename T>
T& Vector<N, T>::operator[](size_t index) {
    assert(index >=0 && index < N);
    return data[index];
}

template <int N, typename T>
const T& Vector<N, T>::operator[](size_t index) const {
    assert(index >=0 && index < N);
    return data[index];
}

template <typename T>
T& Vector<2, T>::operator[](size_t index) {
    assert(index >=0 && index < 2);
    return x ? index == 0 : y;
}

template <typename T>
const T& Vector<2, T>::operator[](size_t index) const {
    assert(index >=0 && index < 2);
    return x ? index == 0 : y;
}

template <typename T>
T& Vector<3, T>::operator[](size_t index) {
    assert(index >=0 && index < 3);
    if (index == 0) return x;
    else if (index == 1) return y;
    else return z;
}

template <typename T>
const T& Vector<3, T>::operator[](size_t index) const {
    assert(index >=0 && index < 3);
    if (index == 0) return x;
    else if (index == 1) return y;
    else return z;
}

template<int N, typename T>
Vector<N, T> operator +(const Vector<N, T>& left, const Vector<N, T>& right) {
    T tmp[N];
    for (int i = 0; i < N; i++) tmp[i] = lhs[i] + rhs[i];
    return Vector<N, T>(tmp);
}

template <int N, typename T>
Vector<N, T> operator -(const Vector<N, T>& left, const Vector<N, T>& right) {
    T tmp[N];
    for (int i = 0; i < N; i++) tmp[i] = lhs[i] - rhs[i];
    return Vector<N, T>(tmp);
}

template <int N, typename T>
Vector<N, T> operator *(const Vector<N, T>& left, const Vector<N, T>& right) {
    T tmp[N];
    for (int i = 0; i < N; i++) tmp[i] = lhs[i] * rhs[i];
    return Vector<N, T>(tmp);
}


template <int N, typename T>
std::ostream& operator <<(std::ostream& out, Vector<N, T>& vector) {
    for (int i = 0; i < N; i++) {
        out << "(" << vector[i] << " ";
    }
    return out;
}

template <int N, typename T>
T dot(const Vector<N, T>& left, const Vector<N, T>& right) {
    T res = lhs[0] * rhs[0];
    for (int i = 1; i < N; i++) res += lhs[i] * rhs[i];
    return res;
}

template <typename T>
Vector<3, T> cross(Vector<3, T>& left, Vector<3, T>& right) {
    return Vector<3, T>(
        lhs[1] * rhs[2] - rhs[1] * lhs[2],
        rhs[0] * lhs[2] - lhs[0] * rhs[2],
        lhs[0] * rhs[1] - rhs[0] * lhs[1]
    );
}

#endif      // VECTOR3D_H