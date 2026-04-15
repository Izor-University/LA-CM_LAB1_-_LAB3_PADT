#ifndef VECTOR_HPP
#define VECTOR_HPP
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "MathUtils.hpp"

template <class T>
class Vector {
private:
    MutableArraySequence<T>* data;
    int size;

public:
    explicit Vector(int size);
    Vector(int size, const T& defaultValue);
    Vector(const Vector<T>& other);      // Copy constructor
    Vector(Vector<T>&& other) noexcept;  // Move constructor
    ~Vector();

    Vector<T>& operator=(const Vector<T>& other);      // Copy assignment
    Vector<T>& operator=(Vector<T>&& other) noexcept;  // Move assignment

    int GetSize() const;
    T& operator[](int index);
    const T& operator[](int index) const;

    Vector<T> operator+(const Vector<T>& other) const;
    Vector<T> operator-(const Vector<T>& other) const;
    Vector<T> operator*(const T& scalar) const;
    T operator*(const Vector<T>& other) const; // Dot product

    double Norm() const;
};
#include "Vector.tpp"
#endif