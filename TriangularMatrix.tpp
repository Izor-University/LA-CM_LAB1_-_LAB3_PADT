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
TriangularMatrix<T>::TriangularMatrix(int n, bool lower)
    : size(n), isLower(lower), zeroElement(T()) {

    if (n <= 0) throw IndexOutOfRange("Matrix size must be positive.");

    int elementsCount = (n * (n + 1)) / 2;

    data = new MutableArraySequence<T>();

    for (int i = 0; i < elementsCount; ++i) {
        data->Append(T());
    }
}

template <class T>
TriangularMatrix<T>::~TriangularMatrix() {
    delete data;
}

template <class T>
int TriangularMatrix<T>::GetRows() const { return size; }

template <class T>
int TriangularMatrix<T>::GetCols() const { return size; }

template <class T>
bool TriangularMatrix<T>::IsLower() const { return isLower; }

template <class T>
const T& TriangularMatrix<T>::Get(int row, int col) const {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange("Matrix index out of bounds.");
    }
    int index = Get1DIndex(row, col);
    if (index == -1) return zeroElement;
    return data->Get(index);
}

template <class T>
void TriangularMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= size || col < 0 || col >= size) {
        throw IndexOutOfRange("Matrix index out of bounds.");
    }
    int index = Get1DIndex(row, col);
    if (index == -1) {
        if (value != zeroElement) {
            throw InvalidArgument("Cannot set non-zero value outside the triangle.");
        }
        return;
    }
    data->Set(index, value);
}