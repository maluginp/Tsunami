#include "vector.h"
#include <assert.h>
namespace tsunami{
namespace core{
template<class T>
Vector<T>::Vector(int rows)
    : rows_(rows) {

    T* vector_ = new T[rows_];
}
template<class T>
Vector<T>::Vector(const Vector<T> &other)
    : rows_(other.rows_){

    T* vector_ = new T[rows_];
    for(int i=0; i < rows_; ++i){
        vector_[i] = other[i];
    }
}
template<class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    rows_ = other.rows_;
    delete[] vector_;
    for(int i=0; i < rows_; ++i){
        vector_[i] = other[i];
    }

    return *this;
}
template<class T>
Vector<T> &Vector<T>::operator+(const Vector<T> &other) {
    Q_ASSERT( rows_ == other.rows_ );

    for(int i=0; i < rows_; ++i){
        vector_[i] += other[i];
    }

    return *this;
}
template<class T>
Vector<T> &Vector<T>::operator-(const Vector<T> &other) {
    Q_ASSERT( rows_ == other.rows_ );

    for(int i=0; i < rows_; ++i){
        vector_[i] -= other[i];
    }

    return *this;
}
template<class T>
Vector<T> &Vector<T>::operator*(T scalar) {
    for(int i=0; i < rows_; ++i){
        vector_[i] *= scalar;
    }

    return *this;
}
template<class T>
Vector<T> &Vector<T>::operator/(T scalar) {
    for(int i=0; i < rows_; ++i){
        vector_[i] /= scalar;
    }

    return *this;
}

template<class T>
Vector<T>::~Vector() {
    delete[] vector_;
}
template<class T>
T &Vector<T>::operator[](int row) {
    assert( row >= 0 && row < rows_ );
    return vector_[row];
}
template<class T>
const int &Vector<T>::rows() {
    return rows_;
}


}
}
