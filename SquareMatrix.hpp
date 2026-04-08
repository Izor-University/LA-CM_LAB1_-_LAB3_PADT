#ifndef MATRIX_SOLVER_HPP
#define MATRIX_SOLVER_HPP

#include "SquareMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Sequence.hpp"
#include "MutableArraySequence.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"

template <class T>
class MatrixSolver {
private:
    static T Abs(const T& val) {
        return val < T() ? -val : val;
    }

public:
    static Sequence<T>* SolveGaussNoPivot(const SquareMatrix<T>& A, const Sequence<T>& b);
    static Sequence<T>* SolveGaussPartialPivot(const SquareMatrix<T>& A, const Sequence<T>& b);

    static Pair<TriangularMatrix<T>*, TriangularMatrix<T>*> DecomposeLU(const SquareMatrix<T>& A);

    static Sequence<T>* SolveLower(const TriangularMatrix<T>& L, const Sequence<T>& b);
    static Sequence<T>* SolveUpper(const TriangularMatrix<T>& U, const Sequence<T>& y);
    static Sequence<T>* SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Sequence<T>& b);
};

#include "MatrixSolver.tpp"
#endif // MATRIX_SOLVER_HPP