#include <gtest/gtest.h>
#include "../src/MatrixSolver.hpp"
#include "../src/SquareMatrix.hpp"
#include "../src/Vector.hpp"
#include "../LAB2/Exceptions.hpp"


const double EPSILON = 1e-9;

// =========================================================================
// 1. ТЕСТЫ МЕТОДА ГАУССА (GAUSSIAN ELIMINATION)
// =========================================================================

TEST(MatrixSolverTest, GaussSimpleSystem) {
    SquareMatrix<double> A(2);
    A(0, 0) = 2.0; A(0, 1) = 1.0;
    A(1, 0) = 1.0; A(1, 1) = -3.0;

    Vector<double> b(2);
    b[0] = 5.0;
    b[1] = -1.0;

    Vector<double> solution = MatrixSolver<double>::SolveGaussNoPivot(A, b);

    EXPECT_EQ(solution.GetSize(), 2);
    EXPECT_NEAR(solution[0], 2.0, EPSILON);
    EXPECT_NEAR(solution[1], 1.0, EPSILON);
}

TEST(MatrixSolverTest, GaussWithPartialPivoting) {
    SquareMatrix<double> A(2);
    A(0, 0) = 0.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = -1.0;

    Vector<double> b(2);
    b[0] = 4.0;
    b[1] = 5.0;

    Vector<double> solution = MatrixSolver<double>::SolveGaussPartialPivot(A, b);

    EXPECT_NEAR(solution[1], 2.0, EPSILON);
    EXPECT_NEAR(solution[0], 7.0 / 3.0, EPSILON);
}

TEST(MatrixSolverTest, GaussLargeSystem) {
    SquareMatrix<double> A(3);
    A(0, 0) = 3.0;  A(0, 1) = 2.0;  A(0, 2) = -1.0;
    A(1, 0) = 2.0;  A(1, 1) = -2.0; A(1, 2) = 4.0;
    A(2, 0) = -1.0; A(2, 1) = 0.5;  A(2, 2) = -1.0;

    Vector<double> b(3);
    b[0] = 1.0; b[1] = -2.0; b[2] = 0.0;

    Vector<double> solution = MatrixSolver<double>::SolveGaussPartialPivot(A, b);

    EXPECT_NEAR(solution[0], 1.0, EPSILON);
    EXPECT_NEAR(solution[1], -2.0, EPSILON);
    EXPECT_NEAR(solution[2], -2.0, EPSILON);
}

// =========================================================================
// 2. ЗАЩИТА ОТ СИНГУЛЯРНОСТИ И ОШИБОК РАЗМЕРНОСТЕЙ
// =========================================================================

TEST(MatrixSolverTest, GaussSingularMatrixThrows) {
    SquareMatrix<double> A(2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 2.0; A(1, 1) = 4.0;

    Vector<double> b(2);
    b[0] = 3.0; b[1] = 6.0;

    EXPECT_THROW(MatrixSolver<double>::SolveGaussNoPivot(A, b), SingularMatrixError);
}

TEST(MatrixSolverTest, GaussDimensionMismatchThrows) {
    SquareMatrix<double> A(3);
    Vector<double> b(2);

    EXPECT_THROW(MatrixSolver<double>::SolveGaussPartialPivot(A, b), InvalidArgument);
}

// =========================================================================
// 3. ТЕСТЫ LU-РАЗЛОЖЕНИЯ (LU DECOMPOSITION)
// =========================================================================

TEST(MatrixSolverTest, LUDecompositionCorrectness) {
    SquareMatrix<double> A(2);
    A(0, 0) = 2.0; A(0, 1) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 7.0;

    auto result = MatrixSolver<double>::DecomposeLU(A);

    const auto& L = result.first;
    const auto& U = result.second;

    EXPECT_TRUE(L.IsLower());
    EXPECT_FALSE(U.IsLower());

    EXPECT_NEAR(L(0, 0), 1.0, EPSILON);
    EXPECT_NEAR(L(1, 1), 1.0, EPSILON);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            double sum = 0.0;
            for (int k = 0; k < 2; ++k) {
                sum += L(i, k) * U(k, j);
            }
            EXPECT_NEAR(sum, A(i, j), EPSILON);
        }
    }
}

TEST(MatrixSolverTest, LUSingularMatrixThrows) {
    SquareMatrix<double> A(2);
    A(0, 0) = 0.0; A(0, 1) = 1.0;
    A(1, 0) = 0.0; A(1, 1) = 2.0;

    EXPECT_THROW(MatrixSolver<double>::DecomposeLU(A), SingularMatrixError);
}