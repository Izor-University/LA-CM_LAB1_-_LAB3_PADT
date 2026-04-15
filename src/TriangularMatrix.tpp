template <class T>
int TriangularMatrix<T>::Get1DIndex(int row, int col) const {
    if (isLower) {
        if (row < col) return -1;
        return (row * (row + 1)) / 2 + col;
    } else {
        if (row > col) return -1;
        return (col * (col + 1)) / 2 + row;
    }
}

template <class T>
TriangularMatrix<T>::TriangularMatrix(int n, bool lower) : size(n), isLower(lower), zeroElement(T()) {
    if (n <= 0) throw IndexOutOfRange();
    int elementsCount = (n * (n + 1)) / 2;
    data = new MutableArraySequence<T>();
    for (int i = 0; i < elementsCount; ++i) data->Append(T());
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
TriangularMatrix<T>::~TriangularMatrix() { delete data; }

template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator=(const TriangularMatrix<T>& other) {
    if (this != &other) { delete data; size = other.size; isLower = other.isLower; data = new MutableArraySequence<T>(*(other.data)); }
    return *this;
}

template <class T>
TriangularMatrix<T>& TriangularMatrix<T>::operator=(TriangularMatrix<T>&& other) noexcept {
    if (this != &other) { delete data; data = other.data; size = other.size; isLower = other.isLower; other.data = nullptr; }
    return *this;
}

template <class T>
T& TriangularMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    int index = Get1DIndex(row, col);
    if (index == -1) throw InvalidArgument("Write access outside the triangle is forbidden");
    return (*data)[index];
}

template <class T>
const T& TriangularMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    int index = Get1DIndex(row, col);
    if (index == -1) return zeroElement;
    return (*data)[index];
}

template <class T>
void TriangularMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= size || col < 0 || col >= size) throw IndexOutOfRange();
    int index = Get1DIndex(row, col);
    if (index == -1) {
        if (value != zeroElement) throw InvalidArgument("Cannot set non-zero value outside the triangle.");
        return;
    }
    (*data)[index] = value;
}