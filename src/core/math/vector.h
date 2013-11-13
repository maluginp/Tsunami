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

}
}

#endif // VECTOR_H
