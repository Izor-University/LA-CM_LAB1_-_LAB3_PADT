#ifndef MATRIX_SOLVER_HPP
#define MATRIX_SOLVER_HPP
#include "SquareMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Vector.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include "MathUtils.hpp"

template <class T>
class MatrixSolver {
public:
    static Vector<T> SolveGaussNoPivot(SquareMatrix<T> U, Vector<T> y);
    static Vector<T> SolveGaussPartialPivot(SquareMatrix<T> U, Vector<T> y);

    // Возврат Pair по значению (без указателей!)
    static Pair<TriangularMatrix<T>, TriangularMatrix<T>> DecomposeLU(SquareMatrix<T> A);
    static Vector<T> SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Vector<T>& b);
};
#include "MatrixSolver.tpp"
#endif