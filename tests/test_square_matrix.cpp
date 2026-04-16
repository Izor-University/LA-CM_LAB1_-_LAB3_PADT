#include <gtest/gtest.h>
#include "../src/SquareMatrix.hpp"
#include "../src/Vector.hpp"
#include "../src/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ЖИЗНЕННОГО ЦИКЛА (RULE OF ZERO PROOF)
// =========================================================================

TEST(SquareMatrixTest, DefaultInitializationIsZero) {
    SquareMatrix<double> A(3);
    EXPECT_EQ(A.GetRows(), 3);
    EXPECT_EQ(A.GetCols(), 3);

    // Вся матрица обязана быть заполнена нулями
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
    SquareMatrix<double> copy(original); // Вызов дефолтного copy-конструктора

    // Изменяем копию, проверяем, что оригинал не тронут
    copy(0, 1) = 99.0;
    EXPECT_DOUBLE_EQ(original(0, 1), 3.14);
    EXPECT_DOUBLE_EQ(copy(0, 1), 99.0);
}

TEST(SquareMatrixTest, MoveSemantics) {
    SquareMatrix<double> source(3, 8.0);

    // Move-конструктор (сгенерирован компилятором)
    SquareMatrix<double> target(std::move(source));
    EXPECT_EQ(target.GetRows(), 3);
    EXPECT_DOUBLE_EQ(target(2, 2), 8.0);

    // Источник должен стать пустым (доказательство работы базового класса)
    EXPECT_EQ(source.GetRows(), 0);

    // Move-присваивание
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

    // Константный доступ
    const SquareMatrix<double>& cA = A;
    EXPECT_DOUBLE_EQ(cA(0, 1), 4.2);

    // Границы
    EXPECT_THROW(A(-1, 0), IndexOutOfRange);
    EXPECT_THROW(A(2, 0), IndexOutOfRange); // Индексы только 0, 1
}

TEST(SquareMatrixTest, InheritedMathAndNorm) {
    SquareMatrix<double> A(2, 1.0);
    SquareMatrix<double> B(2, 2.5);

    SquareMatrix<double> sum = A + B;
    EXPECT_DOUBLE_EQ(sum(0, 0), 3.5);

    SquareMatrix<double> scaled = A * 4.0;
    EXPECT_DOUBLE_EQ(scaled(1, 1), 4.0);

    // Норма (Октаэдрическая)
    SquareMatrix<double> N(2);
    N(0, 0) = -1.0; N(0, 1) = 5.0; // Сумма = 6
    N(1, 0) = -8.0; N(1, 1) = 2.0; // Сумма = 10 (Норма)
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
    // A = | 1  2  3 |
    //     | 4  5  6 |
    //     | 7  8  9 |
    SquareMatrix<double> A(3);
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 5.0; A(1, 2) = 6.0;
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 9.0;

    // x = | 1 |
    //     | 2 |
    //     | 3 |
    Vector<double> x(3);
    x[0] = 1.0; x[1] = 2.0; x[2] = 3.0;

    // b = A * x
    Vector<double> b = A * x;

    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_DOUBLE_EQ(b[0], 14.0); // 1*1 + 2*2 + 3*3
    EXPECT_DOUBLE_EQ(b[1], 32.0); // 4*1 + 5*2 + 6*3
    EXPECT_DOUBLE_EQ(b[2], 50.0); // 7*1 + 8*2 + 9*3

    Vector<double> wrongSize(2);
    EXPECT_THROW(A * wrongSize, InvalidArgument);
}

TEST(SquareMatrixTest, MatrixMultiplication) {
    // A = | 1 2 |
    //     | 3 4 |
    SquareMatrix<double> A(2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = 4.0;

    // B = | 2 0 |
    //     | 1 2 |
    SquareMatrix<double> B(2);
    B(0, 0) = 2.0; B(0, 1) = 0.0;
    B(1, 0) = 1.0; B(1, 1) = 2.0;

    // C = A * B = | 4  4 |
    //             | 10 8 |
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

    // Умножение квадратных матриц разных размеров должно блокироваться
    EXPECT_THROW(A * B, InvalidArgument);
}

TEST(SquareMatrixTest, TraceCalculation) {
    SquareMatrix<double> A(3);
    A(0, 0) = 5.0; A(0, 1) = 99.0; A(0, 2) = -10.0;
    A(1, 0) = 4.0; A(1, 1) = -2.0; A(1, 2) = 8.0;
    A(2, 0) = 0.0; A(2, 1) = 1.0;  A(2, 2) = 7.0;

    // След (Trace) = 5.0 + (-2.0) + 7.0 = 10.0
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
                // На главной диагонали должны быть 1.0
                EXPECT_DOUBLE_EQ(E(i, j), 1.0);
            } else {
                // Вне диагонали должны быть 0.0
                EXPECT_DOUBLE_EQ(E(i, j), 0.0);
            }
        }
    }

    // A * E = A
    SquareMatrix<double> A(4, 3.14);
    SquareMatrix<double> Result = A * E;

    EXPECT_DOUBLE_EQ(Result(2, 2), 3.14);
    EXPECT_DOUBLE_EQ(Result(0, 3), 3.14);
}