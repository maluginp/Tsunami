#ifndef MATRIX_H
#define MATRIX_H

// TODO Need testing
// double a = double& A::f();
// double& A::f() = double& B::f()

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
        MATRIX_COPY,
        MATRIX_TRANSPOSE,
        MATRIX_INVERSE
    };

    Matrix();
    Matrix( int rows, int columns, TypeMatrix type = MATRIX_ZERO );
    Matrix( int square, TypeMatrix type = MATRIX_ZERO );
    Matrix( const Matrix<T>& other, TypeMatrix type = MATRIX_COPY );
    Matrix( const Vector<T>& vector, TypeMatrix type = MATRIX_COPY  );
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
    T& at(int row, int column);

    void inverse();
    void transpose();

    bool isEmpty();

private:
    bool isScalar();
    bool isVector();


    void initialize(TypeMatrix type);

    int rows_;
    int columns_;
    T* matrix_;
};

}
}


#endif // MATRIX_H
