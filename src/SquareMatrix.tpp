// Математика квадратных матриц
template <class T>
Vector<T> SquareMatrix<T>::operator*(const Vector<T>& x) const {
    if (this->cols != x.GetSize()) {
        throw InvalidArgument("Size mismatch");
    }
    Vector<T> result(this->rows);

    for (int i = 0; i < this->rows; ++i) {
        T sum = T();
        for (int j = 0; j < this->cols; ++j) {
            sum = sum + (*this)(i, j) * x[j];
        }
        result[i] = sum;
    }
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const SquareMatrix<T>& other) const {
    if (this->cols != other.rows) {
        throw InvalidArgument("Matrix multiplication mismatch");
    }
    SquareMatrix<T> result(this->rows);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            T sum = T();
            for (int k = 0; k < this->cols; ++k) {
                sum = sum + (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

template <class T>
T SquareMatrix<T>::Trace() const {
    T sum = T();
    for (int i = 0; i < this->rows; ++i) {
        sum = sum + (*this)(i, i);
    }
    return sum;
}

// Фабричный метод
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