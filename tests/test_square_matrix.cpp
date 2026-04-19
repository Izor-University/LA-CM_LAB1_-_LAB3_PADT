#include <gtest/gtest.h>
#include "../src/SquareMatrix.hpp"
#include "../src/Vector.hpp"
#include "../LAB2/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ЖИЗНЕННОГО ЦИКЛА (RULE OF ZERO PROOF)
// =========================================================================

TEST(SquareMatrixTest, DefaultInitializationIsZero) {
    SquareMatrix<double> A(3);
    EXPECT_EQ(A.GetRows(), 3);
    EXPECT_EQ(A.GetCols(), 3);

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(A(i, j), 0.0);
        }
    }
}

TEST(SquareMatrixTest, ValueInitialization) {
    SquareMatrix<double> A(2, -7.5);
    EXPECT_DOUBLE_EQ(A(0, 0), -7.5);
    EXPECT_DOUBLE_EQ(A(1, 1), -7.5);
    EXPECT_DOUBLE_EQ(A(0, 1), -7.5);
}

TEST(SquareMatrixTest, InvalidDimensionsThrow) {
    EXPECT_THROW(SquareMatrix<double>(0), IndexOutOfRange);
    EXPECT_THROW(SquareMatrix<double>(-5), IndexOutOfRange);
    EXPECT_THROW(SquareMatrix<double>(0, 1.0), IndexOutOfRange);
}

TEST(SquareMatrixTest, DeepCopySemantics) {
    SquareMatrix<double> original(2, 3.14);
    SquareMatrix<double> copy(original);

    copy(0, 1) = 99.0;
    EXPECT_DOUBLE_EQ(original(0, 1), 3.14);
    EXPECT_DOUBLE_EQ(copy(0, 1), 99.0);
}

TEST(SquareMatrixTest, MoveSemantics) {
    SquareMatrix<double> source(3, 8.0);
    SquareMatrix<double> target(std::move(source));
    EXPECT_EQ(target.GetRows(), 3);
    EXPECT_DOUBLE_EQ(target(2, 2), 8.0);
    EXPECT_EQ(source.GetRows(), 0);

    SquareMatrix<double> target2(1);
    target2 = std::move(target);
    EXPECT_EQ(target2.GetRows(), 3);
    EXPECT_EQ(target.GetRows(), 0);
}

// =========================================================================
// 2. ДОСТУП К ПАМЯТИ И УНАСЛЕДОВАННЫЕ ОПЕРАЦИИ
// =========================================================================

TEST(SquareMatrixTest, ElementAccessAndBounds) {
    SquareMatrix<double> A(2);
    A(0, 1) = 4.2;
    A(1, 0) = -1.1;

    EXPECT_DOUBLE_EQ(A(0, 1), 4.2);
    EXPECT_DOUBLE_EQ(A(1, 0), -1.1);

    const SquareMatrix<double>& cA = A;
    EXPECT_DOUBLE_EQ(cA(0, 1), 4.2);

    EXPECT_THROW(A(-1, 0), IndexOutOfRange);
    EXPECT_THROW(A(2, 0), IndexOutOfRange);
}

TEST(SquareMatrixTest, InheritedMathAndNorm) {
    SquareMatrix<double> A(2, 1.0);
    SquareMatrix<double> B(2, 2.5);

    // ВАЖНО: Используем auto
    auto sum = A + B;
    EXPECT_DOUBLE_EQ(sum(0, 0), 3.5);

    // ВАЖНО: Используем auto
    auto scaled = A * 4.0;
    EXPECT_DOUBLE_EQ(scaled(1, 1), 4.0);

    SquareMatrix<double> N(2);
    N(0, 0) = -1.0; N(0, 1) = 5.0;
    N(1, 0) = -8.0; N(1, 1) = 2.0;
    EXPECT_DOUBLE_EQ(N.Norm(), 10.0);
}

TEST(SquareMatrixTest, InheritedSwapRows) {
    SquareMatrix<double> A(3);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 9.0;

    A.SwapRows(0, 2);

    EXPECT_DOUBLE_EQ(A(0, 0), 7.0);
    EXPECT_DOUBLE_EQ(A(2, 2), 3.0);
}

// =========================================================================
// 3. СПЕЦИФИЧЕСКАЯ МАТЕМАТИКА КВАДРАТНЫХ МАТРИЦ
// =========================================================================

TEST(SquareMatrixTest, MatrixVectorMultiplication) {
    SquareMatrix<double> A(3);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 5.0; A(1, 2) = 6.0;
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 9.0;

    Vector<double> x(3);
    x[0] = 1.0; x[1] = 2.0; x[2] = 3.0;

    Vector<double> b = A * x;

    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_DOUBLE_EQ(b[0], 14.0);
    EXPECT_DOUBLE_EQ(b[1], 32.0);
    EXPECT_DOUBLE_EQ(b[2], 50.0);

    Vector<double> wrongSize(2);
    EXPECT_THROW(A * wrongSize, InvalidArgument);
}

TEST(SquareMatrixTest, MatrixMultiplication) {
    SquareMatrix<double> A(2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 4.0;

    SquareMatrix<double> B(2);
    B(0, 0) = 2.0; B(0, 1) = 0.0;
    B(1, 0) = 1.0; B(1, 1) = 2.0;

    SquareMatrix<double> C = A * B;

    EXPECT_EQ(C.GetRows(), 2);
    EXPECT_DOUBLE_EQ(C(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(C(0, 1), 4.0);
    EXPECT_DOUBLE_EQ(C(1, 0), 10.0);
    EXPECT_DOUBLE_EQ(C(1, 1), 8.0);
}

TEST(SquareMatrixTest, MatrixMultiplicationDimensionMismatchThrows) {
    SquareMatrix<double> A(2);
    SquareMatrix<double> B(3);

    EXPECT_THROW(A * B, InvalidArgument);
}

TEST(SquareMatrixTest, TraceCalculation) {
    SquareMatrix<double> A(3);
    A(0, 0) = 5.0; A(0, 1) = 99.0; A(0, 2) = -10.0;
    A(1, 0) = 4.0; A(1, 1) = -2.0; A(1, 2) = 8.0;
    A(2, 0) = 0.0; A(2, 1) = 1.0;  A(2, 2) = 7.0;

    EXPECT_DOUBLE_EQ(A.Trace(), 10.0);
}

TEST(SquareMatrixTest, IdentityMatrixGeneration) {
    int n = 4;
    auto E = SquareMatrix<double>::Identity(n);

    EXPECT_EQ(E.GetRows(), 4);
    EXPECT_EQ(E.GetCols(), 4);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j) {
                EXPECT_DOUBLE_EQ(E(i, j), 1.0);
            } else {
                EXPECT_DOUBLE_EQ(E(i, j), 0.0);
            }
        }
    }

    SquareMatrix<double> A(4, 3.14);
    SquareMatrix<double> Result = A * E;

    EXPECT_DOUBLE_EQ(Result(2, 2), 3.14);
    EXPECT_DOUBLE_EQ(Result(0, 3), 3.14);
}