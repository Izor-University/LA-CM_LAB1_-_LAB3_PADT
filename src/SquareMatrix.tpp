template <class T>
SquareMatrix<T>::SquareMatrix(int n) : size(n) {
    if (n <= 0) throw IndexOutOfRange();
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n * n; ++i) data->Append(T());
}

template <class T>
SquareMatrix<T>::SquareMatrix(int n, const T& defaultValue) : size(n) {
    if (n <= 0) throw IndexOutOfRange();
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n * n; ++i) data->Append(defaultValue);
}

template <class T>
SquareMatrix<T>::SquareMatrix(const SquareMatrix<T>& other) : size(other.size) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
SquareMatrix<T>::SquareMatrix(SquareMatrix<T>&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

template <class T>
SquareMatrix<T>::~SquareMatrix() { delete data; }

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator=(const SquareMatrix<T>& other) {
    if (this != &other) { delete data; size = other.size; data = new MutableArraySequence<T>(*(other.data)); }
    return *this;
}

template <class T>
SquareMatrix<T>& SquareMatrix<T>::operator=(SquareMatrix<T>&& other) noexcept {
    if (this != &other) { delete data; data = other.data; size = other.size; other.data = nullptr; }
    return *this;
}

template <class T>
int SquareMatrix<T>::GetRows() const { return size; }
template <class T>
int SquareMatrix<T>::GetCols() const { return size; }

template <class T>
T& SquareMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
const T& SquareMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator+(const SquareMatrix<T>& other) const {
    if (this->size != other.size) throw InvalidArgument("Mismatch");
    SquareMatrix<T> result(size);
    for (int i = 0; i < size * size; ++i) result.data->operator[](i) = (*data)[i] + (*other.data)[i];
    return result;
}

template <class T>
SquareMatrix<T> SquareMatrix<T>::operator*(const T& scalar) const {
    SquareMatrix<T> result(size);
    for (int i = 0; i < size * size; ++i) result.data->operator[](i) = (*data)[i] * scalar;
    return result;
}

template <class T>
Vector<T> SquareMatrix<T>::operator*(const Vector<T>& x) const {
    if (size != x.GetSize()) throw InvalidArgument("Mismatch");
    Vector<T> result(size);
    for (int i = 0; i < size; ++i) {
        T sum = T();
        for (int j = 0; j < size; ++j) sum = sum + (*this)(i, j) * x[j];
        result[i] = sum;
    }
    return result;
}

template <class T>
double SquareMatrix<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < size; ++i) {
        double currentSum = 0.0;
        for (int j = 0; j < size; ++j) currentSum += MathUtils::Abs((*this)(i, j));
        if (currentSum > maxNorm) maxNorm = currentSum;
    }
    return maxNorm;
}

template <class T>
void SquareMatrix<T>::SwapRows(int row1, int row2) {
    if (row1 == row2) return;
    for (int j = 0; j < size; ++j) {
        T temp = (*this)(row1, j);
        (*this)(row1, j) = (*this)(row2, j);
        (*this)(row2, j) = temp;
    }
}