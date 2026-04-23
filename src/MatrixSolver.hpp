#ifndef MATRIX_SOLVER_HPP
#define MATRIX_SOLVER_HPP

#include "SquareMatrix.hpp"
#include "TriangularMatrix.hpp"
#include "Vector.hpp"
#include "../LAB2/Exceptions.hpp"
#include "../LAB2/Utils.hpp"
#include "MathUtils.hpp"
#include <random>

// Решатель систем линейных уравнений
template <class T>
class MatrixSolver {
public:
    // Алгоритмы решения СЛАУ
    static Vector<T> SolveGaussNoPivot(SquareMatrix<T> A, Vector<T> y);
    static Vector<T> SolveGaussPartialPivot(SquareMatrix<T> A, Vector<T> y);

    // Алгоритмы декомпозиции
    static Pair<TriangularMatrix<T>, TriangularMatrix<T>> DecomposeLU(SquareMatrix<T> A);
    static Vector<T> SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Vector<T>& b);

    // Генерация тестовых данных
    static SquareMatrix<T> GenerateRandomMatrix(int n, unsigned int seed = 42);
    static Vector<T> GenerateRandomVector(int n, unsigned int seed = 42);
    static SquareMatrix<T> GenerateHilbertMatrix(int n);
    static Vector<T> GenerateOnesVector(int n);

    // Метрики качества
    static double CalculateResidual(const SquareMatrix<T>& A, const Vector<T>& x_approx, const Vector<T>& b);
    static double CalculateRelativeError(const Vector<T>& x_exact, const Vector<T>& x_approx);
};

#include "MatrixSolver.tpp"
#endif // MATRIX_SOLVER_HPP