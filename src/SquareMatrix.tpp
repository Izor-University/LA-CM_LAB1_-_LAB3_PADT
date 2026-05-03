// Операторы составного присваивания
template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator+=(const IMatrix<T>& other) {
    Matrix<T>::operator+=(other);
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator-=(const IMatrix<T>& other) {
    Matrix<T>::operator-=(other);
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator*=(const T& scalar) {
    Matrix<T>::operator*=(scalar);
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator*=(const SquareMatrix<T>& other) {
    // Матричное умножение не может быть выполнено in-place без создания буфера,
    // поэтому мы используем обычный оператор умножения и перезаписываем текущий объект.
    *this = (*this) * other;
    return *this;
}

// Арифметические операторы
template <class T>
SquareMatrix<T> SquareMatrix<T>::operator+(const IMatrix<T>& other) const {
    SquareMatrix<T> result(*this);
    result += other;
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator-(const IMatrix<T>& other) const {
    SquareMatrix<T> result(*this);
    result -= other;
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const T& scalar) const {
    SquareMatrix<T> result(*this);
    result *= scalar;
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const SquareMatrix<T>& other) const {
    if (this->cols != other.GetRows()) {
        throw InvalidArgument("Matrix multiplication mismatch");
    }

    SquareMatrix<T> result(this->rows);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < other.GetCols(); ++j) {
            T sum = T();
            for (int k = 0; k < this->cols; ++k) {
                sum = sum + (*this)(i, k) * other.Get(k, j);
            }
            result(i, j) = sum;
        }
    }

    return result;
}

// Специфические методы квадратных матриц
template <class T>
T SquareMatrix<T>::Trace() const {
    T sum = T();

    for (int i = 0; i < this->rows; ++i) {
        sum = sum + (*this)(i, i);
    }

    return sum;
}

// Фабричные методы
template <class T>
SquareMatrix<T> SquareMatrix<T>::Identity(int n) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }

    SquareMatrix<T> result(n);
    T one = static_cast<T>(1);

    for (int i = 0; i < n; ++i) {
        result(i, i) = one;
    }

    return result;
}