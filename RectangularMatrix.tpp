// --- Конструкторы ---

template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw IndexOutOfRange("Matrix dimensions must be positive.");
    }
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) {
        data->Append(T());
    }
}

template <class T>
RectangularMatrix<T>::RectangularMatrix(int rows, int cols, const T& defaultValue) : rows(rows), cols(cols) {
    if (rows <= 0 || cols <= 0) {
        throw IndexOutOfRange("Matrix dimensions must be positive.");
    }
    data = new MutableArraySequence<T>();
    for (int i = 0; i < rows * cols; ++i) {
        data->Append(defaultValue);
    }
}

// Копирующий конструктор
template <class T>
RectangularMatrix<T>::RectangularMatrix(const RectangularMatrix<T>& other)
    : rows(other.rows), cols(other.cols) {
    // Используем конструктор копирования ArraySequence
    data = new MutableArraySequence<T>(*(other.data));
}

// Деструктор
template <class T>
RectangularMatrix<T>::~RectangularMatrix() {
    delete data;
}

// --- Геттеры ---

template <class T>
int RectangularMatrix<T>::GetRows() const {
    return rows;
}

template <class T>
int RectangularMatrix<T>::GetCols() const {
    return cols;
}

// --- Доступ к элементам ---

template <class T>
const T& RectangularMatrix<T>::Get(int row, int col) const {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw IndexOutOfRange("Matrix index out of bounds.");
    }
    return data->Get(Get1DIndex(row, col));
}

template <class T>
void RectangularMatrix<T>::Set(int row, int col, const T& value) {
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        throw IndexOutOfRange("Matrix index out of bounds.");
    }
    data->Set(Get1DIndex(row, col), value);
}

// --- Математические операции ---

template <class T>
RectangularMatrix<T>* RectangularMatrix<T>::Add(const RectangularMatrix<T>& other) const {
    if (this->rows != other.rows || this->cols != other.cols) {
        throw InvalidArgument("Matrix dimensions must match for addition.");
    }
    RectangularMatrix<T>* result = new RectangularMatrix<T>(this->rows, this->cols);
    for (int i = 0; i < rows * cols; ++i) {
        result->data->Set(i, this->data->Get(i) + other.data->Get(i));
    }
    return result;
}

template <class T>
RectangularMatrix<T>* RectangularMatrix<T>::MultiplyScalar(const T& scalar) const {
    RectangularMatrix<T>* result = new RectangularMatrix<T>(this->rows, this->cols);
    for (int i = 0; i < rows * cols; ++i) {
        result->data->Set(i, this->data->Get(i) * scalar);
    }
    return result;
}

// Октаэдрическая норма (максимальная сумма модулей элементов по строкам)
template <class T>
T RectangularMatrix<T>::Norm() const {
    T maxNorm = T();
    for (int i = 0; i < rows; ++i) {
        T currentSum = T();
        for (int j = 0; j < cols; ++j) {
            currentSum += Abs(this->Get(i, j));
        }
        if (currentSum > maxNorm) {
            maxNorm = currentSum;
        }
    }
    return maxNorm;
}

// --- Элементарные преобразования строк ---

template <class T>
void RectangularMatrix<T>::SwapRows(int row1, int row2) {
    if (row1 < 0 || row1 >= rows || row2 < 0 || row2 >= rows) {
        throw IndexOutOfRange("Row index out of bounds.");
    }
    if (row1 == row2) return;

    for (int j = 0; j < cols; ++j) {
        T temp = this->Get(row1, j);
        this->Set(row1, j, this->Get(row2, j));
        this->Set(row2, j, temp);
    }
}

template <class T>
void RectangularMatrix<T>::AddRowMultiple(int sourceRow, int targetRow, const T& factor) {
    if (sourceRow < 0 || sourceRow >= rows || targetRow < 0 || targetRow >= rows) {
        throw IndexOutOfRange("Row index out of bounds.");
    }
    for (int j = 0; j < cols; ++j) {
        T newValue = this->Get(targetRow, j) + this->Get(sourceRow, j) * factor;
        this->Set(targetRow, j, newValue);
    }
}

template <class T>
void RectangularMatrix<T>::MultiplyRow(int row, const T& factor) {
    if (row < 0 || row >= rows) {
        throw IndexOutOfRange("Row index out of bounds.");
    }
    for (int j = 0; j < cols; ++j) {
        this->Set(row, j, this->Get(row, j) * factor);
    }
}