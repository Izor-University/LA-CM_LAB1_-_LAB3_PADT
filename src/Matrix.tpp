template <class T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) throw IndexOutOfRange("Dimensions must be positive");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) data->Append(T());
}

template <class T>
Matrix<T>::Matrix(int rows, int cols, const T& defaultValue) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) throw IndexOutOfRange("Dimensions must be positive");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) data->Append(defaultValue);
}

template <class T>
Matrix<T>::Matrix(const Matrix<T>& other) : rows(other.rows), cols(other.cols) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
Matrix<T>::Matrix(Matrix<T>&& other) noexcept : data(other.data), rows(other.rows), cols(other.cols) {
    other.data = nullptr;
    other.rows = 0;
    other.cols = 0;
}

template <class T>
Matrix<T>::~Matrix() { delete data; }

template <class T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T>& other) {
    if (this != &other) {
        delete data;
        rows = other.rows;
        cols = other.cols;
        data = new MutableArraySequence<T>(*(other.data));
    }
    return *this;
}

template <class T>
Matrix<T>& Matrix<T>::operator=(Matrix<T>&& other) noexcept {
    if (this != &other) {
        delete data;
        data = other.data;
        rows = other.rows;
        cols = other.cols;
        other.data = nullptr;
        other.rows = 0;
        other.cols = 0;
    }
    return *this;
}

template <class T>
T& Matrix<T>::operator()(int row, int col) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
const T& Matrix<T>::operator()(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) throw IndexOutOfRange();
    return (*data)[Get1DIndex(row, col)];
}

template <class T>
Matrix<T> Matrix<T>::operator+(const Matrix<T>& other) const {
    if (rows != other.rows || cols != other.cols) throw InvalidArgument("Dimensions mismatch");
    Matrix<T> result(rows, cols);
    for (int i = 0; i < rows * cols; ++i) (*result.data)[i] = (*data)[i] + (*other.data)[i];
    return result;
}

template <class T>
Matrix<T> Matrix<T>::operator*(const T& scalar) const {
    Matrix<T> result(rows, cols);
    for (int i = 0; i < rows * cols; ++i) (*result.data)[i] = (*data)[i] * scalar;
    return result;
}

template <class T>
double Matrix<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < rows; ++i) {
        double currentSum = 0.0;
        for (int j = 0; j < cols; ++j) currentSum += MathUtils::Abs((*this)(i, j));
        if (currentSum > maxNorm) maxNorm = currentSum;
    }
    return maxNorm;
}

template <class T>
void Matrix<T>::SwapRows(int row1, int row2) {
    if (row1 < 0 || row1 >= rows || row2 < 0 || row2 >= rows) throw IndexOutOfRange();
    if (row1 == row2) return;
    for (int j = 0; j < cols; ++j) {
        T temp = (*this)(row1, j);
        (*this)(row1, j) = (*this)(row2, j);
        (*this)(row2, j) = temp;
    }
}

template <class T>
void Matrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& factor) {
    if (sourceRow < 0 || sourceRow >= rows || targetRow < 0 || targetRow >= rows) throw IndexOutOfRange();
    for (int j = 0; j < cols; ++j) (*this)(targetRow, j) = (*this)(targetRow, j) + (*this)(sourceRow, j) * factor;
}

template <class T>
void Matrix<T>::MultiplyRow(int row, const T& factor) {
    if (row < 0 || row >= rows) throw IndexOutOfRange();
    for (int j = 0; j < cols; ++j) (*this)(row, j) = (*this)(row, j) * factor;
}