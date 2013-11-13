#ifndef MATRIX_H
#define MATRIX_H

// TODO Need testing
// double a = double& A::f();
// double& A::f() = double& B::f()

// TODO !UnitTest for this class!
namespace tsunami{
namespace core{

template<class T> class Vector;
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
    T& at(int row, int column);

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

}
}


#endif // MATRIX_H
