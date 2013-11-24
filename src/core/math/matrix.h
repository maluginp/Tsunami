#ifndef MATRIX_H
#define MATRIX_H
#include <assert.h>
#include <float.h>

// TODO Need testing
// double a = double& A::f();
// double& A::f() = double& B::f()

// TODO !UnitTest for this class!
namespace tsunami{
namespace core{

export template<class T> class Vector;

template<class T>
class Matrix
{
public:
    enum TypeMatrix{
        MATRIX_ZERO,
        MATRIX_IDENTITY
    };
    enum OperationMatrix{
        MATRIX_COPY,
        MATRIX_TRANSPOSE,
        MATRIX_INVERSE
    };

    Matrix();
    Matrix( int rows, int columns, TypeMatrix type );
    Matrix( int square, TypeMatrix type );
    Matrix( const Matrix<T>& other, OperationMatrix type = MATRIX_COPY );
    Matrix( const Vector<T>& vector, OperationMatrix type = MATRIX_COPY  );
    Matrix( T scalar );

    Matrix<T>& operator=( const Matrix<T>& other );
    Matrix<T>& operator=( const Vector<T>& vector);
    ~Matrix();

    int rows();
    int columns();

    Matrix<T>& operator*( const Matrix<T>& other);
    Matrix<T>& operator*( const Vector<T>& vector);
    Matrix<T>& operator*( T scalar );
    Matrix<T>& operator/( const Matrix<T>& other);
    Matrix<T>& operator/( T scalar );
    Matrix<T>& operator+( const Matrix<T>& other);
    Matrix<T>& operator-( const Matrix<T>& other);

    T& operator()(int row, int column);
    const T& operator()(int row, int column) const;

    T& at(int row, int column);
    const T& at(int row, int column) const;
    bool inverse();
    void transpose();

    bool isEmpty();
    bool isScalar();
    bool isVector();
private:

    void swap( int row1, int row2 );

    void initialize(TypeMatrix type,int rows=-1, int columns=-1);

    int rows_;
    int columns_;
    T* matrix_;
};

template<class T>
Matrix<T>::Matrix()
    : rows_(0)
    , columns_(0), matrix_(0) {
    //
}
template<class T>
Matrix<T>::Matrix(int rows, int columns,TypeMatrix type)
    : rows_(rows)
    , columns_(columns),matrix_(0) {

    initialize(type);

}
template<class T>
Matrix<T>::Matrix(int square,TypeMatrix type)
    : rows_(square)
    , columns_(square),matrix_(0) {

    initialize(type);


}
template<class T>
Matrix<T>::Matrix(const Matrix<T> &other, OperationMatrix type):matrix_(0) {

    rows_ = other.rows_;
    columns_ = other.columns_;

    initialize( MATRIX_ZERO );

    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++j){
            matrix_[i*columns_+j] = other(i,j);
        }
    }


    if(type == MATRIX_TRANSPOSE){
        transpose();
    }else if(type == MATRIX_INVERSE){
        inverse();
    }

}
template<class T>
Matrix<T>::Matrix(const Vector<T> &vector, OperationMatrix type):matrix_(0) {
    rows_ = vector.rows();
    columns_ = 1;
    initialize( MATRIX_ZERO );

    for(int i=0; i < rows_; ++i){
        matrix_[i] = vector[i];
    }

    if( type == MATRIX_TRANSPOSE ){
        transpose();
    }

}
template<class T>
Matrix<T>::Matrix(T scalar):matrix_(0) {
    rows_ = 1;
    columns_ = 1;
    initialize( MATRIX_ZERO );

    matrix_[0] = scalar;

}
template<class T>
Matrix<T> &Matrix<T>::operator =(const Matrix<T> &other) {
    rows_ = other.rows_;
    columns_ = other.columns_;
    initialize(MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++j){
            matrix_[i*columns_+j] = other.at(i,j);
        }
    }
    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator =(const Vector<T> &vector) {
    rows_ = vector.rows();
    columns_ = 1;
    initialize(MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        matrix_[i] = vector[i];
    }
    return *this;
}

template<class T>
T &Matrix<T>::operator ()(int row, int column) {
    return at(row,column);
}

template<class T>
const T &Matrix<T>::operator()(int row, int column) const {
    return at(row,column);

}

//template<class T>
//const T& operator()(int row, int column) const{
//    return at
//}

template<class T>
T &Matrix<T>::at(int row, int column) {
    Q_ASSERT( row >= 0 && row < rows_ && column >=  0 && column < columns_ );

    return matrix_[ row*columns_ + column ];
}

template<class T>
const T &Matrix<T>::at(int row, int column) const{
    Q_ASSERT( row >= 0 && row < rows_ && column >=  0 && column < columns_ );

    return matrix_[ row*columns_ + column ];
}
template<class T>
bool Matrix<T>::inverse() {
    Q_ASSERT( rows_ == columns_ );
    Matrix<T> E(rows_,columns_,MATRIX_IDENTITY);
    Matrix<T> matrix(*this);

    int N = rows_;
    T tmp;

    for(int k=0; k < N; ++k){
        tmp = matrix(k,k);
        if( fabs(tmp) < DBL_MIN ){
            bool swaped = false;
            for(int i=k+1; i < N; ++i){
                if(fabs(matrix(i,k)) > DBL_MIN){
                    matrix.swap(k,i);
                    E.swap(k,i);
                    swaped = true;
                    break;
                }
            }
            assert(swaped);
            return false;
        }
        tmp = matrix(k,k);
        for(int i=0;i < N; ++i){
            matrix(k,i) /= tmp;
            E(k,i) /= tmp;
        }
        for(int i=k+1;i < N; ++i){
            tmp = matrix(i,k);
            for(int j=0; j < N; ++j){
                E(i,j) -= E(k,j)*tmp;
                matrix(i,j) -= matrix(k,j)*tmp;
            }
        }
    }

    for(int i=N-1;i >0; --i){
        for(int j=i-1; j >=0; --j){
            tmp = at(j,i);
            for(int k=0; k < N; ++k){
                E(j,k) -= E(i,k)*tmp;
                matrix(j,k) -= matrix(i,k)*tmp;
            }
        }
    }

    // Copy E matrix
    initialize(MATRIX_ZERO);
    for(int i=0; i < N; ++i){
        for(int j=0; j < N; ++j){
            at(i,j) = E(i,j);
        }
    }

    return true;
}
template<class T>
void Matrix<T>::transpose() {
    Matrix<T> matrix(*this);

    columns_ = matrix.rows_;
    rows_    = matrix.columns_;

    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++j){
            at(i,j) = matrix(j,i);
        }
    }

}

template<class T>
Matrix<T>::~Matrix() {
    delete[] matrix_;
}
template<class T>
int Matrix<T>::rows() {
    return rows_;
}
template<class T>
int Matrix<T>::columns() {
    return columns_;
}
template<class T>
Matrix<T> &Matrix<T>::operator*(const Matrix<T> &other){
    if(other.isScalar()){
        *this * other.at(0,0);
    }
    if( isScalar() ){
        T scalar = at(0,0);
        initialize(MATRIX_ZERO,other.rows_,other.columns_);

        Matrix<T> temp(other);
        temp = temp*scalar;

        for( int i=0; i < temp.rows_; ++i ){
            for(int j=0; j < temp.columns_; ++j){
                at(i,j) = temp(i,j);
            }
        }

    }

    assert( columns_ == other.rows_ );

    int M = rows_, N = other.columns_, R = other.rows_;

    T* matrix = new T[M*N];

    for(int i=0; i < M; ++i){
        for(int j=0; j < N; ++i){
            T c = static_cast<T>(0.0);
            for(int r=0; r<R; r++){
                c += at(i,r)*other.at(r,j);
            }
            matrix[i*M+j] = c;
        }
    }

    rows_ = M;
    columns_ = N;

    delete[] matrix_;
    matrix_ = matrix;

    return *this;

}
template<class T>
Matrix<T> &Matrix<T>::operator*(const Vector<T> &vector) {
    Matrix<T> matrix(vector);
    *this*matrix;
    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator*(T scalar) {
    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++j){
            at(i,j) *= scalar;
        }
    }
    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator/(const Matrix<T> &other) {
    Matrix<T> matrix(other);
    matrix.inverse();

    *this * matrix;

    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator/(T scalar) {
    T val = 1 / scalar;
    *this * val;

    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator+(const Matrix<T> &other) {
    assert( other.rows_ == rows_ && other.columns_ == columns_ );

    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++i){
            at(i,j) += other.at(i,j);
        }
    }
    return *this;
}
template<class T>
Matrix<T> &Matrix<T>::operator-(const Matrix<T> &other) {
    assert( other.rows_ == rows_ && other.columns_ == columns_ );

    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++i){
            at(i,j) -= other.at(i,j);
        }
    }
    return *this;
}
template<class T>
void Matrix<T>::initialize(TypeMatrix type, int rows, int columns) {
    if(rows > 0)   { rows_ = rows;       }
    if(columns > 0){ columns_ = columns; }

    if(matrix_ != 0){
        delete[] matrix_;
    }

    matrix_ = new T[rows_*columns_];
    if(type == MATRIX_ZERO){
        for(int i=0; i < rows_; ++i){
            for(int j=0; j < columns_; ++j){
                at( i, j ) = static_cast<T>(0.0);
            }
        }
    }else if(type == MATRIX_IDENTITY){
        for(int i=0; i < rows_; ++i){
            for(int j=0; j < columns_; ++j){
                if(i == j){
                    at( i, j ) = static_cast<T>(1.0);
                }else{
                    at( i, j ) = static_cast<T>(0.0);
                }
            }
        }

    }
}
template<class T>
bool Matrix<T>::isScalar() {
    return (rows_ == 1 && columns_ == 1);
}
template<class T>
bool Matrix<T>::isVector() {
    return (rows_ > 1 && columns_ == 1);
}
template<class T>
void Matrix<T>::swap(int row1, int row2) {
    int N = columns_;
    T temp; // = new T[N];
    for(int i=0; i < N; ++i){
        temp = at(row1,i);
        at(row1,i) = at(row2,i);
        at(row2,i) = temp;
    }

    return;
}
template<class T>
bool Matrix<T>::isEmpty() {
    return (rows_ == 0 && columns_ == 0);
}


}
}


#endif // MATRIX_H
