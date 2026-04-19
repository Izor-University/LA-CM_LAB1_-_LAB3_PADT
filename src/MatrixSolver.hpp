#ifndef MATRIX_SOLVER_HPP
#define MATRIX_SOLVER_HPP

#include "SquareMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Vector.hpp"
#include "Exceptions.hpp"
#include "Utils.hpp"
#include "MathUtils.hpp"
#include <random> // Для генерации матриц

template <class T>
class MatrixSolver {
public:
    // --- 1. АЛГОРИТМЫ РЕШЕНИЯ СЛАУ ---
    static Vector<T> SolveGaussNoPivot(SquareMatrix<T> A, Vector<T> y);
    static Vector<T> SolveGaussPartialPivot(SquareMatrix<T> A, Vector<T> y);
    static Pair<TriangularMatrix<T>, TriangularMatrix<T>> DecomposeLU(SquareMatrix<T> A);
    static Vector<T> SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Vector<T>& b);

    // --- 2. ГЕНЕРАЦИЯ ДАННЫХ ДЛЯ ТЕСТОВ (ЛР-1) ---
    static SquareMatrix<T> GenerateRandomMatrix(int n, unsigned int seed = 42);
    static Vector<T> GenerateRandomVector(int n, unsigned int seed = 42);
    static SquareMatrix<T> GenerateHilbertMatrix(int n);
    static Vector<T> GenerateOnesVector(int n);

    // --- 3. МЕТРИКИ КАЧЕСТВА (ЛР-1) ---
    static double CalculateResidual(const SquareMatrix<T>& A, const Vector<T>& x_approx, const Vector<T>& b);
    static double CalculateRelativeError(const Vector<T>& x_exact, const Vector<T>& x_approx);
};

#include "MatrixSolver.tpp"
#endif // MATRIX_SOLVER_HPP