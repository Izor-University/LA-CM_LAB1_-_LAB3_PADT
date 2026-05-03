// Конструкторы и деструктор
template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n) : size(n), zeroElement(T()) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) {
        data->Append(T());
    }
}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n, const T& defaultValue) : size(n), zeroElement(T()) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) {
        data->Append(defaultValue);
    }
}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(const DiagonalMatrix<T>& other) : size(other.size), zeroElement(T()) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(DiagonalMatrix<T>&& other) noexcept : size(other.size), zeroElement(T()) {
    data = other.data;
    other.data = nullptr;
    other.size = 0;
}

template <class T>
DiagonalMatrix<T>::~DiagonalMatrix() {
    delete data;
}

// Операторы присваивания
template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator=(const DiagonalMatrix<T>& other) {
    if (this != &other) {
        delete data;
        size = other.size;
        data = new MutableArraySequence<T>(*(other.data));
    }
    return *this;
}

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator=(DiagonalMatrix<T>&& other) noexcept {
    if (this != &other) {
        delete data;
        data = other.data;
        size = other.size;
        other.data = nullptr;
        other.size = 0;
    }
    return *this;
}

// Доступ к элементам (с защитой диагонали)
template <class T>
T& DiagonalMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    if (row != col) {
        throw InvalidArgument("Write access outside the main diagonal is forbidden");
    }
    return (*data)[row];
}

template <class T>
const T& DiagonalMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    if (row != col) {
        return zeroElement;
    }
    return (*data)[row];
}

template <class T>
void DiagonalMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange();
    }
    if (row != col) {
        if (value != zeroElement) {
            throw InvalidArgument("Cannot set non-zero value outside diagonal");
        }
        return;
    }
    (*data)[row] = value;
}

// Операторы составного присваивания (O(N))
template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator+=(const DiagonalMatrix<T>& other) {
    if (this->size != other.size) throw InvalidArgument("Mismatch size");
    for (int i = 0; i < size; ++i) {
        (*data)[i] = (*data)[i] + (*other.data)[i];
    }
    return *this;
}

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator-=(const DiagonalMatrix<T>& other) {
    if (this->size != other.size) throw InvalidArgument("Mismatch size");
    for (int i = 0; i < size; ++i) {
        (*data)[i] = (*data)[i] - (*other.data)[i];
    }
    return *this;
}

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator*=(const DiagonalMatrix<T>& other) {
    if (this->size != other.size) throw InvalidArgument("Mismatch size");
    for (int i = 0; i < size; ++i) {
        (*data)[i] = (*data)[i] * (*other.data)[i];
    }
    return *this;
}

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator*=(const T& scalar) {
    for (int i = 0; i < size; ++i) {
        (*data)[i] = (*data)[i] * scalar;
    }
    return *this;
}

// Арифметические операторы
template <class T> DiagonalMatrix<T> DiagonalMatrix<T>::operator+(const DiagonalMatrix<T>& other) const { DiagonalMatrix<T> res(*this); res += other; return res; }
template <class T> DiagonalMatrix<T> DiagonalMatrix<T>::operator-(const DiagonalMatrix<T>& other) const { DiagonalMatrix<T> res(*this); res -= other; return res; }
template <class T> DiagonalMatrix<T> DiagonalMatrix<T>::operator*(const DiagonalMatrix<T>& other) const { DiagonalMatrix<T> res(*this); res *= other; return res; }
template <class T> DiagonalMatrix<T> DiagonalMatrix<T>::operator*(const T& scalar) const { DiagonalMatrix<T> res(*this); res *= scalar; return res; }

// Оптимизированное умножение на вектор (O(N))
template <class T>
Vector<T> DiagonalMatrix<T>::operator*(const Vector<T>& v) const {
    if (size != v.GetSize()) {
        throw InvalidArgument("Mismatch size");
    }
    Vector<T> result(size);
    for (int i = 0; i < size; ++i) {
        result[i] = (*data)[i] * v[i];
    }
    return result;
}

// Норма
template <class T>
double DiagonalMatrix<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < size; ++i) {
        double currentAbs = MathUtils::Abs((*data)[i]);
        if (currentAbs > maxNorm) {
            maxNorm = currentAbs;
        }
    }
    return maxNorm;
}