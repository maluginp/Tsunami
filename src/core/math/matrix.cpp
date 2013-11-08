#include "matrix.h"
namespace tsunami{
namespace core{

template<class T>
Matrix::Matrix(int rows, int columns,TypeMatrix type)
    : rows_(rows)
    , columns_(columns) {

    matrix_ = new T[rows*columns];
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
Matrix::Matrix(int square,TypeMatrix type) : Matrix(square,square,type) {

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

Matrix::~Matrix() {
    delete[] matrix_;
}



}
}
