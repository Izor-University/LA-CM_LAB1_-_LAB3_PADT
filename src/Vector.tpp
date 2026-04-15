template <class T>
Vector<T>::Vector(int size) : size(size) {
    if (size <= 0) throw IndexOutOfRange("Size must be positive");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < size; ++i) data->Append(T());
}

template <class T>
Vector<T>::Vector(int size, const T& defaultValue) : size(size) {
    if (size <= 0) throw IndexOutOfRange("Size must be positive");
    data = new MutableArraySequence<T>();
    for (int i = 0; i < size; ++i) data->Append(defaultValue);
}

template <class T>
Vector<T>::Vector(const Vector<T>& other) : size(other.size) {
    data = new MutableArraySequence<T>(*(other.data));
}

template <class T>
Vector<T>::Vector(Vector<T>&& other) noexcept : data(other.data), size(other.size) {
    other.data = nullptr;
    other.size = 0;
}

template <class T>
Vector<T>::~Vector() { delete data; }

template <class T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
    if (this != &other) { delete data; size = other.size; data = new MutableArraySequence<T>(*(other.data)); }
    return *this;
}

template <class T>
Vector<T>& Vector<T>::operator=(Vector<T>&& other) noexcept {
    if (this != &other) { delete data; data = other.data; size = other.size; other.data = nullptr; }
    return *this;
}

template <class T>
int Vector<T>::GetSize() const { return size; }

template <class T>
T& Vector<T>::operator[](int index) {
    if (index < 0 || index >= size) throw IndexOutOfRange();
    return (*data)[index];
}

template <class T>
const T& Vector<T>::operator[](int index) const {
    if (index < 0 || index >= size) throw IndexOutOfRange();
    return (*data)[index];
}

template <class T>
Vector<T> Vector<T>::operator+(const Vector<T>& other) const {
    if (size != other.size) throw InvalidArgument("Sizes mismatch");
    Vector<T> result(size);
    for (int i = 0; i < size; ++i) result[i] = (*this)[i] + other[i];
    return result;
}

template <class T>
Vector<T> Vector<T>::operator-(const Vector<T>& other) const {
    if (size != other.size) throw InvalidArgument("Sizes mismatch");
    Vector<T> result(size);
    for (int i = 0; i < size; ++i) result[i] = (*this)[i] - other[i];
    return result;
}

template <class T>
Vector<T> Vector<T>::operator*(const T& scalar) const {
    Vector<T> result(size);
    for (int i = 0; i < size; ++i) result[i] = (*this)[i] * scalar;
    return result;
}

template <class T>
T Vector<T>::operator*(const Vector<T>& other) const {
    if (size != other.size) throw InvalidArgument("Sizes mismatch");
    T sum = T();
    for (int i = 0; i < size; ++i) sum = sum + ((*this)[i] * other[i]);
    return sum;
}

template <class T>
double Vector<T>::Norm() const {
    double maxNorm = 0.0;
    for (int i = 0; i < size; ++i) {
        double current = MathUtils::Abs((*this)[i]);
        if (current > maxNorm) maxNorm = current;
    }
    return maxNorm;
}