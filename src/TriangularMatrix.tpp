// Внутренние методы (Упаковка индексов)
template <class T>
int TriangularMatrix<T>::Get1DIndex(int row, int col) const {
    if (isLower) {
        if (row < col) {
            return -1;
        }
        return (row * (row + 1)) / 2 + col;
    } else {
        if (row > col) {
            return -1;
        }
        return (col * (col + 1)) / 2 + row;
    }
}

// Конструкторы и деструктор
template <class T>
TriangularMatrix<T>::TriangularMatrix(int n, bool lower) : size(n), isLower(lower), zeroElement(T()) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }
    int elementsCount = (n * (n + 1)) / 2;
    data = new MutableArraySequence<T>();
    for (int i = 0; i < elementsCount; ++i) {
        data->Append(T());
    }
}

template <class T>
TriangularMatrix<T>::TriangularMatrix(const TriangularMatrix<T>& other) : size(other.size), isLower(other.isLower), zeroElement(T()) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
TriangularMatrix<T>::TriangularMatrix(TriangularMatrix<T>&& other) noexcept : size(other.size), isLower(other.isLower), zeroElement(T()) {
    data = other.data;
    other.data = nullptr;
    other.size = 0;
}

template <class T>
TriangularMatrix<T>::~TriangularMatrix() {
    delete data;
}

// Операторы присваивания
template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator=(const TriangularMatrix<T>& other) {
    if (this != &other) {
        delete data;
        size = other.size;
        isLower = other.isLower;
        data = new MutableArraySequence<T>(*(other.data));
    }
    return *this;
}

template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator=(TriangularMatrix<T>&& other) noexcept {
    if (this != &other) {
        delete data;
        data = other.data;
        size = other.size;
        isLower = other.isLower;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}

// Доступ к элементам (с защитой инварианта)
template <class T>
T& TriangularMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    int index = Get1DIndex(row, col);
    if (index == -1) {
        throw InvalidArgument("Write access outside the triangle is forbidden");
    }
    return (*data)[index];
}

template <class T>
const T& TriangularMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    int index = Get1DIndex(row, col);
    if (index == -1) {
        return zeroElement;
    }
    return (*data)[index];
}

template <class T>
void TriangularMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    int index = Get1DIndex(row, col);
    if (index == -1) {
        if (value != zeroElement) {
            throw InvalidArgument("Cannot set non-zero value outside the triangle");
        }
        return;
    }
    (*data)[index] = value;
}

// Операторы составного присваивания
template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator+=(const TriangularMatrix<T>& other) {
    if (size != other.size || isLower != other.isLower) {
        throw InvalidArgument("Mismatch type or size");
    }
    int elementsCount = (size * (size + 1)) / 2;
    for (int i = 0; i < elementsCount; ++i) {
        (*data)[i] = (*data)[i] + (*other.data)[i];
    }
    return *this;
}

template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator-=(const TriangularMatrix<T>& other) {
    if (size != other.size || isLower != other.isLower) {
        throw InvalidArgument("Mismatch type or size");
    }
    int elementsCount = (size * (size + 1)) / 2;
    for (int i = 0; i < elementsCount; ++i) {
        (*data)[i] = (*data)[i] - (*other.data)[i];
    }
    return *this;
}

template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator*=(const T& scalar) {
    int elementsCount = (size * (size + 1)) / 2;
    for (int i = 0; i < elementsCount; ++i) {
        (*data)[i] = (*data)[i] * scalar;
    }
    return *this;
}

// Арифметические операторы
template <class T>
TriangularMatrix<T> TriangularMatrix<T>::operator+(const TriangularMatrix<T>& other) const {
    TriangularMatrix<T> res(*this);
    res += other;
    return res;
}

template <class T>
TriangularMatrix<T> TriangularMatrix<T>::operator-(const TriangularMatrix<T>& other) const {
    TriangularMatrix<T> res(*this);
    res -= other;
    return res;
}

template <class T>
TriangularMatrix<T> TriangularMatrix<T>::operator*(const T& scalar) const {
    TriangularMatrix<T> res(*this);
    res *= scalar;
    return res;
}

// Умножение матриц
template <class T>
TriangularMatrix<T> TriangularMatrix<T>::operator*(const TriangularMatrix<T>& other) const {
    if (size != other.size || isLower != other.isLower) {
        throw InvalidArgument("Matrix multiplication mismatch");
    }
    TriangularMatrix<T> res(size, isLower);

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // Вычисляем только внутри нужного треугольника
            if ((isLower && i >= j) || (!isLower && i <= j)) {
                T sum = T();
                for (int k = 0; k < size; ++k) {
                    sum = sum + (*this)(i, k) * other(k, j);
                }
                res.Set(i, j, sum);
            }
        }
    }
    return res;
}

// Умножение матрицы на вектор
template <class T>
Vector<T> TriangularMatrix<T>::operator*(const Vector<T>& v) const {
    if (size != v.GetSize()) {
        throw InvalidArgument("Size mismatch");
    }
    Vector<T> res(size);

    for (int i = 0; i < size; ++i) {
        T sum = T();
        for (int j = 0; j < size; ++j) {
            sum = sum + (*this)(i, j) * v[j];
        }
        res[i] = sum;
    }
    return res;
}