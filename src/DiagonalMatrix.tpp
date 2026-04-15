template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n) : size(n), zeroElement(T()) {
    if (n <= 0) throw IndexOutOfRange();
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) data->Append(T());
}

template <class T>
DiagonalMatrix<T>::DiagonalMatrix(int n, const T& defaultValue) : size(n), zeroElement(T()) {
    if (n <= 0) throw IndexOutOfRange();
    data = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) data->Append(defaultValue);
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
DiagonalMatrix<T>::~DiagonalMatrix() { delete data; }

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator=(const DiagonalMatrix<T>& other) {
    if (this != &other) { delete data; size = other.size; data = new MutableArraySequence<T>(*(other.data)); }
    return *this;
}

template <class T>
DiagonalMatrix<T>& DiagonalMatrix<T>::operator=(DiagonalMatrix<T>&& other) noexcept {
    if (this != &other) { delete data; data = other.data; size = other.size; other.data = nullptr; }
    return *this;
}

template <class T>
T& DiagonalMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    if (row != col) throw InvalidArgument("Write access outside the main diagonal is forbidden");
    return (*data)[row];
}

template <class T>
const T& DiagonalMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    if (row != col) return zeroElement;
    return (*data)[row];
}

template <class T>
void DiagonalMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    if (row != col) {
        if (value != zeroElement) throw InvalidArgument("Cannot set non-zero outside diagonal.");
        return;
    }
    (*data)[row] = value;
}

template <class T>
DiagonalMatrix<T> DiagonalMatrix<T>::operator+(const DiagonalMatrix<T>& other) const {
    if (this->size != other.size) throw InvalidArgument("Mismatch");
    DiagonalMatrix<T> result(this->size);
    for (int i = 0; i < size; ++i) result.data->operator[](i) = (*data)[i] + (*other.data)[i];
    return result;
}

template <class T>
DiagonalMatrix<T> DiagonalMatrix<T>::operator*(const T& scalar) const {
    DiagonalMatrix<T> result(this->size);
    for (int i = 0; i < size; ++i) result.data->operator[](i) = (*data)[i] * scalar;
    return result;
}

template <class T>
double DiagonalMatrix<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < size; ++i) {
        double currentAbs = MathUtils::Abs((*data)[i]);
        if (currentAbs > maxNorm) maxNorm = currentAbs;
    }
    return maxNorm;
}