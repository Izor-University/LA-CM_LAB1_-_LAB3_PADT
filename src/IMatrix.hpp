#ifndef IMATRIX_HPP
#define IMATRIX_HPP

template <class T>
class IMatrix {
public:
    virtual ~IMatrix() = default;

    virtual const T& Get(int row, int col) const = 0;
    virtual void Set(int row, int col, const T& value) = 0;
    virtual int GetRows() const = 0;
    virtual int GetCols() const = 0;
};

#endif // IMATRIX_HPP