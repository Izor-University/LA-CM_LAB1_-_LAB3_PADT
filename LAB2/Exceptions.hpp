#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

// --- Базовый класс для всех исключений ---
class Exception {
protected:
    const char* message;
public:
    explicit Exception(const char* msg) : message(msg) {}
    virtual ~Exception() {}
    virtual const char* GetMessage() const { return message; }
};

// --- Существующие исключения ---
class IndexOutOfRange : public Exception {
public:
    explicit IndexOutOfRange(const char* msg = "Index out of range") : Exception(msg) {}
};

class EmptyCollectionError : public Exception {
public:
    explicit EmptyCollectionError(const char* msg = "Collection is empty") : Exception(msg) {}
};

// --- НОВЫЕ ИСКЛЮЧЕНИЯ (Замена STL stdexcept) ---
class InvalidArgument : public Exception {
public:
    explicit InvalidArgument(const char* msg = "Invalid argument provided") : Exception(msg) {}
};

class MathError : public Exception {
public:
    explicit MathError(const char* msg = "Mathematical error occurred") : Exception(msg) {}
};

class SingularMatrixError : public MathError {
public:
    explicit SingularMatrixError(const char* msg = "Matrix is singular or strictly zero") : MathError(msg) {}
};

#endif // EXCEPTIONS_HPP