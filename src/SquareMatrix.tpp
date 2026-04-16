// =========================================================================
// 1. Умножение квадратной матрицы на вектор
// =========================================================================
template <class T>
Vector<T> SquareMatrix<T>::operator*(const Vector<T>& x) const {
    if (this->cols != x.GetSize()) throw InvalidArgument("Size mismatch");
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

// =========================================================================
// 2. Умножение квадратной матрицы на квадратную матрицу
// =========================================================================
template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const SquareMatrix<T>& other) const {
    if (this->cols != other.rows) throw InvalidArgument("Matrix multiplication mismatch");

    SquareMatrix<T> result(this->rows);

    for (int i = 0; i < this->rows; ++i) {
        for (int j = 0; j < other.cols; ++j) {
            T sum = T();
            for (int k = 0; k < this->cols; ++k) {
                // i-тая строка первой матрицы умножается на j-тый столбец второй
                sum = sum + (*this)(i, k) * other(k, j);
            }
            result(i, j) = sum;
        }
    }
    return result;
}

// =========================================================================
// 3. След матрицы (Trace)
// =========================================================================
template <class T>
T SquareMatrix<T>::Trace() const {
    T sum = T();
    for (int i = 0; i < this->rows; ++i) {
        sum = sum + (*this)(i, i); // Складываем элементы (0,0), (1,1) и т.д.
    }
    return sum;
}

// =========================================================================
// 4. Генерация единичной матрицы (Identity)
// =========================================================================
template <class T>
SquareMatrix<T> SquareMatrix<T>::Identity(int n) {
    if (n <= 0) throw IndexOutOfRange("Size must be positive");

    SquareMatrix<T> result(n); // Создается заполненная нулями

    // В C++ нет универсальной "единицы" для шаблонов, как T(), который дает ноль.
    // Поэтому мы кастуем обычную единицу к типу T.
    // Для double это 1.0, для int это 1, для Complex это (1, 0).
    T one = static_cast<T>(1);

    for (int i = 0; i < n; ++i) {
        result(i, i) = one;
    }

    return result;
}