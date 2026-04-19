#include <gtest/gtest.h>
#include "../src/Matrix.hpp"
#include "../LAB2/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ЖИЗНЕННОГО ЦИКЛА
// =========================================================================

TEST(BaseMatrixTest, RectangularInitialization) {
    Matrix<double> A(2, 3);
    EXPECT_EQ(A.GetRows(), 2);
    EXPECT_EQ(A.GetCols(), 3);

    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 3; ++j) {
            EXPECT_DOUBLE_EQ(A(i, j), 0.0);
        }
    }
}

TEST(BaseMatrixTest, ValueInitialization) {
    Matrix<double> A(3, 4, -7.5);
    EXPECT_EQ(A.GetRows(), 3);
    EXPECT_EQ(A.GetCols(), 4);
    EXPECT_DOUBLE_EQ(A(2, 3), -7.5);
}

TEST(BaseMatrixTest, InvalidDimensionsThrow) {
    EXPECT_THROW(Matrix<double>(0, 5), IndexOutOfRange);
    EXPECT_THROW(Matrix<double>(3, -2), IndexOutOfRange);
}

// =========================================================================
// 2. ТЕСТЫ КОПИРОВАНИЯ И ПЕРЕМЕЩЕНИЯ
// =========================================================================

TEST(BaseMatrixTest, DeepCopyConstructor) {
    Matrix<double> original(2, 3, 3.14);
    Matrix<double> copy(original);

    copy(0, 1) = 99.0;
    EXPECT_DOUBLE_EQ(original(0, 1), 3.14);
    EXPECT_DOUBLE_EQ(copy(0, 1), 99.0);
}

TEST(BaseMatrixTest, MoveSemantics) {
    Matrix<double> source(2, 4, 8.0);
    Matrix<double> target(std::move(source));

    EXPECT_EQ(target.GetRows(), 2);
    EXPECT_EQ(target.GetCols(), 4);
    EXPECT_DOUBLE_EQ(target(1, 3), 8.0);
    EXPECT_EQ(source.GetRows(), 0);
}

// =========================================================================
// 3. ДОСТУП К ПАМЯТИ И ЗАЩИТА ГРАНИЦ
// =========================================================================

TEST(BaseMatrixTest, OutOfBoundsAccessThrows) {
    Matrix<double> A(2, 3);

    EXPECT_THROW(A(-1, 0), IndexOutOfRange);
    EXPECT_THROW(A(2, 0), IndexOutOfRange);
    EXPECT_THROW(A(0, 3), IndexOutOfRange);
}

// =========================================================================
// 4. ТЕСТЫ АЛГЕБРЫ ПРЯМОУГОЛЬНЫХ МАТРИЦ
// =========================================================================

TEST(BaseMatrixTest, Addition) {
    Matrix<double> A(2, 3, 1.0);
    Matrix<double> B(2, 3, 2.5);

    Matrix<double> sum = A + B;
    EXPECT_DOUBLE_EQ(sum(0, 0), 3.5);
    EXPECT_DOUBLE_EQ(sum(1, 2), 3.5);
}

TEST(BaseMatrixTest, AdditionDimensionMismatchThrows) {
    Matrix<double> A(2, 3);
    Matrix<double> B(3, 2);

    EXPECT_THROW(A + B, InvalidArgument);
}

TEST(BaseMatrixTest, ScalarMultiplication) {
    Matrix<double> A(2, 3, -3.0);
    Matrix<double> res = A * 4.0;

    EXPECT_DOUBLE_EQ(res(1, 2), -12.0);
}

TEST(BaseMatrixTest, NormCalculationRectangular) {
    Matrix<double> A(2, 3);
    A(0, 0) = -1.0; A(0, 1) = 2.0; A(0, 2) = -3.0;
    A(1, 0) = 5.0;  A(1, 1) = 0.0; A(1, 2) = 5.0;

    EXPECT_DOUBLE_EQ(A.Norm(), 10.0);
}