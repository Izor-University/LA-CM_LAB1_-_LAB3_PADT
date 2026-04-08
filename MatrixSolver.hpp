#ifndef MATRIX_SOLVER_HPP
#define MATRIX_SOLVER_HPP

#include "SquareMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp" // Для структуры Pair

template <class T>
class MatrixSolver {
private:
    // Собственный Abs для отказа от <cmath>
    static T Abs(const T& val) {
        return val < 0 ? -val : val;
    }

public:
    // Возвращаем Sequence<T>*, так как работаем через абстракции
    static Sequence<T>* SolveGaussNoPivot(const SquareMatrix<T>& A, const Sequence<T>& b);
    static Sequence<T>* SolveGaussPartialPivot(const SquareMatrix<T>& A, const Sequence<T>& b);

    // Используем кастомный Pair вместо std::pair
    static Pair<TriangularMatrix<T>*, TriangularMatrix<T>*> DecomposeLU(const SquareMatrix<T>& A);

    static Sequence<T>* SolveLower(const TriangularMatrix<T>& L, const Sequence<T>& b);
    static Sequence<T>* SolveUpper(const TriangularMatrix<T>& U, const Sequence<T>& y);
    static Sequence<T>* SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Sequence<T>& b);
};

#include "MatrixSolver.tpp"

#endif // MATRIX_SOLVER_HPP