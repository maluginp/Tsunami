#ifndef VECTOR_H
#define VECTOR_H
namespace tsunami{
namespace core{
template<class T> class Matrix;
template<class T>
class Vector
{
public:
    Vector(int rows);
    Vector(const Vector<T>& other);
    Vector<T>& operator=(const Vector<T>& other);
    Vector<T>& operator+(const Vector<T>& other);
    Vector<T>& operator-(const Vector<T>& other);
    Vector<T>& operator*(T scalar);
    Vector<T>& operator/(T scalar);
    ~Vector();

    T& operator[](int row);

    const int &rows();
private:
    int rows_;
    T* vector_;
};

template<class T>
Vector<T>::Vector(int rows)
    : rows_(rows) {

    vector_ = new T[rows_];

    for(int i=0; i < rows_; ++i){
        vector_[i] = .0;
    }
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

#endif // VECTOR_H
