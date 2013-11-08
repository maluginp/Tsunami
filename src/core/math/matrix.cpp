#include "matrix.h"
#include "vector.h"
namespace tsunami{
namespace core{

template<class T>
Matrix::Matrix(int rows, int columns,TypeMatrix type)
    : rows_(rows)
    , columns_(columns) {

    initialize(type);

}
template<class T>
Matrix::Matrix(int square,TypeMatrix type) : Matrix(square,square,type) {

}
template<class T>
Matrix::Matrix(const Matrix<T> &other, Matrix::TypeMatrix type) {

    rows_ = other.rows_;
    columns_ = other.columns_;

    if(type == MATRIX_IDENTITY ){
        initialize( MATRIX_IDENTITY );
    }else{
        initialize( MATRIX_ZERO );
    }

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
Matrix::Matrix(const Vector<T> &vector,TypeMatrix type) {
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
Matrix::Matrix(T scalar) {
    rows_ = 1;
    columns_ = 1;
    initialize( MATRIX_ZERO );

    matrix_[0] = scalar;

}
template<class T>
Matrix<T> &Matrix::operator =(const Matrix<T> &other) {
    rows_ = other.rows_;
    columns_ = other.columns_;
    initialize(MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        for(int j=0; j < columns_; ++j){
            matrix_[i*columns_+j] = other(i,j);
        }
    }
    return *this;
}
template<class T>
Matrix<T> &Matrix::operator =(const Vector<T> &vector) {
    rows_ = vector.rows();
    columns_ = 1;
    initialize(MATRIX_ZERO);
    for(int i=0; i < rows_; ++i){
        matrix_[i] = other[i];
    }
    return *this;
}

template<class T>
T &Matrix::operator ()(int row, int column) {
    return at(row,column);
}

template<class T>
T &Matrix::at(int row, int column) {
    Q_ASSERT( row >= rows_ || row < 0 || column > columns_ || column < 0 );

    return matrix_[ row*columns_ + column ];
}

void Matrix::inverse() {

}

template<class T>
Matrix::~Matrix() {
    delete[] matrix_;
}
template<class T>
int Matrix::rows() {
    return rows_;
}
template<class T>
int Matrix::columns() {
    return columns_;
}
template<class T>
Matrix<T> &Matrix::operator*(const Matrix<T> &other){
    if(other.isScalar()){
        *this * other.at(0,0);
    }
    if( isScalar() ){
        T scalar = at(0,0);
        Matrix



    }



}
template<class T>
void Matrix::initialize(TypeMatrix type) {
    if(matrix_ != NULL){
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
bool Matrix::isScalar() {
    return (rows_ == 1 && columns_ == 1);
}
template<class T>
bool Matrix::isVector() {
    return (rows_ > 1 && columns_ == 1);
}
template<class T>
bool Matrix::isEmpty() {
    return (rows_ == 0 && columns_ == 0);
}



}
}
