#ifndef MATRIX_H
#define MATRIX_H
namespace tsunami{
namespace core{
template<class T> class Vector;
template<class T>
class Matrix
{
public:
    enum TypeMatrix{
        MATRIX_ZERO,
        MATRIX_IDENTITY,
        MATRIX_TRANSPOSE,
        MATRIX_INVERSE
    };

    Matrix();
    Matrix( int rows, int columns, TypeMatrix type = MATRIX_ZERO );
    Matrix( int square, TypeMatrix type = MATRIX_ZERO );
    Matrix( const Matrix<T>& matrix, TypeMatrix type = MATRIX_ZERO );
    Matrix( const Vector<T>& vector );
    Matrix<T>& operator=(const Vector<T>& vector);
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
    T& at(int row, int column);

    void inverse();
    void transpose();

private:
    bool isScalar();
    bool isVector();

    int rows_;
    int columns_;
    T* matrix_;
};

}
}


#endif // MATRIX_H
