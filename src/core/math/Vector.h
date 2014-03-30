#ifndef VECTOR_H
#define VECTOR_H
namespace tsunami{
namespace core{
#include "Matrix.h"
template<class T>
class Vector
{
    template<class U>
    friend Vector<U> operator+(const Vector<U>& vector1,
                               const Vector<U> &vector2);
    template<class U>
    friend Vector<U> operator-(const Vector<U>& vector1,
                               const Vector<U> &vector2);
    template<class U>
    friend Vector<U> operator*(const Vector<U> &vector,
                               U scalar);
    template<class U>
    friend Vector<U> operator/(const Vector<U> &vector,
                               U scalar);


public:
    Vector();
    Vector(int rows);
    Vector(const Vector<T>& other);
    Vector(const Matrix<T>& other);
    Vector<T>& operator=(const Vector<T>& other);
    ~Vector();

    T& operator[](int row);
    const T& operator[](int row) const;

    const int &rows() const;
private:
    int rows_;
    T* vector_;
};

template<class T>
Vector<T>::Vector() : rows_(0), vector_(0) {

}

template<class T>
Vector<T>::Vector(int rows)
    : rows_(rows), vector_(0) {

    vector_ = new T[rows_];

    for(int i=0; i < rows_; ++i){
        vector_[i] = .0;
    }
}
template<class T>
Vector<T>::Vector(const Vector<T> &other)
    : rows_(other.rows_), vector_(0){

    vector_ = new T[rows_];
    for(int i=0; i < rows_; ++i){
        vector_[i] = other[i];
    }
}
template<class T>
Vector<T>::Vector(const Matrix<T> &other) :
    rows_(0),vector_(0)
{
    if (other.rows() > 0 && other.columns() == 1) {
        rows_ =   other.rows();
        vector_ = new T[rows_];
        for(int i=0; i < rows_; i++){
            vector_[i] = other.at(i,0);
        }

    }
}
template<class T>
Vector<T> &Vector<T>::operator=(const Vector<T> &other) {
    rows_ = other.rows_;
    delete[] vector_;

    vector_ = new T[rows_];

    for(int i=0; i < rows_; ++i){
        vector_[i] = other[i];
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
const T &Vector<T>::operator[](int row) const{
    assert( row >= 0 && row < rows_ );
    return vector_[row];
}
template<class T>
const int &Vector<T>::rows() const{
    return rows_;
}

// Friends

template<class T>
Vector<T> operator+(const Vector<T>& vector1, const Vector<T> &vector2) {
    Q_ASSERT( vector1.rows_ == vector2.rows_ );
    Vector<T> newVector(vector1);

    int N = vector1.rows_;
    for(int i=0; i < N; ++i){
        newVector[i] += vector2[i];
    }

    return newVector;
}
template<class T>
Vector<T> operator-(const Vector<T> &vector1,const Vector<T>& vector2) {
    Q_ASSERT( vector1.rows_ == vector2.rows_ );
    Vector<T> newVector(vector1);

    int N = vector1.rows_;
    for(int i=0; i < N; ++i){
        newVector[i] -= vector2[i];
    }

    return newVector;
}
template<class T>
Vector<T> operator*(const Vector<T> &vector,T scalar) {
    Vector<T> newVector(vector);

    int N = vector.rows_;
    for(int i=0; i < N; ++i){
        newVector[i] *= scalar;
    }

    return newVector;
}
template<class T>
Vector<T> operator/(const Vector<T> &vector,T scalar) {
    Vector<T> newVector(vector);

    int N = vector.rows_;
    for(int i=0; i < N; ++i){
        newVector[i] /= scalar;
    }

    return newVector;

}


}
}

#endif // VECTOR_H
