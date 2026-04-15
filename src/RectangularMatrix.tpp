template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) throw IndexOutOfRange("Invalid dimensions");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) data->Append(T());
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols, const T& defaultValue) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) throw IndexOutOfRange("Invalid dimensions");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) data->Append(defaultValue);
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(const RectangularMatrix<T>& other) : rows(other.rows), cols(other.cols) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(RectangularMatrix<T>&& other) noexcept : rows(other.rows), cols(other.cols) {
    data = other.data;
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

template <class T>
RectangularMatrix<T>::~RectangularMatrix() { delete data; }

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::operator=(const RectangularMatrix<T>& other) {
    if (this != &other) { delete data; rows = other.rows; cols = other.cols; data = new MutableArraySequence<T>(*(other.data)); }
    return *this;
}

template <class T>
RectangularMatrix<T>& RectangularMatrix<T>::operator=(RectangularMatrix<T>&& other) noexcept {
    if (this != &other) { delete data; data = other.data; rows = other.rows; cols = other.cols; other.data = nullptr; }
    return *this;
}

template <class T>
T& RectangularMatrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
const T& RectangularMatrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::operator+(const RectangularMatrix<T>& other) const {
    if (this->rows != other.rows || this->cols != other.cols) throw InvalidArgument("Mismatch");
    RectangularMatrix<T> result(rows, cols);
    for (int i = 0; i < rows * cols; ++i) result.data->operator[](i) = (*data)[i] + (*other.data)[i];
    return result;
}

template <class T>
RectangularMatrix<T> RectangularMatrix<T>::operator*(const T& scalar) const {
    RectangularMatrix<T> result(rows, cols);
    for (int i = 0; i < rows * cols; ++i) result.data->operator[](i) = (*data)[i] * scalar;
    return result;
}

template <class T>
double RectangularMatrix<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < rows; ++i) {
        double currentSum = 0.0;
        for (int j = 0; j < cols; ++j) currentSum += MathUtils::Abs((*this)(i, j));
        if (currentSum > maxNorm) maxNorm = currentSum;
    }
    return maxNorm;
}

template <class T>
void RectangularMatrix<T>::SwapRows(int row1, int row2) {
    if (row1 == row2) return;
    for (int j = 0; j < cols; ++j) {
        T temp = (*this)(row1, j);
        (*this)(row1, j) = (*this)(row2, j);
        (*this)(row2, j) = temp;
    }
}

template <class T>
void RectangularMatrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& factor) {
    for (int j = 0; j < cols; ++j) (*this)(targetRow, j) = (*this)(targetRow, j) + (*this)(sourceRow, j) * factor;
}

template <class T>
void RectangularMatrix<T>::MultiplyRow(int row, const T& factor) {
    for (int j = 0; j < cols; ++j) (*this)(row, j) = (*this)(row, j) * factor;
}