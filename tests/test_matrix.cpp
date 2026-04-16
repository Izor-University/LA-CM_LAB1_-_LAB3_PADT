#include <gtest/gtest.h>
#include "../src/Matrix.hpp"
#include "../src/Vector.hpp"
#include "../src/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ЖИЗНЕННОГО ЦИКЛА (LIFECYCLE)
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
    // Матрица не может иметь размер <= 0
    EXPECT_THROW(SquareMatrix<double>(0), IndexOutOfRange);
    EXPECT_THROW(SquareMatrix<double>(-5), IndexOutOfRange);
    EXPECT_THROW(SquareMatrix<double>(0, 1.0), IndexOutOfRange);
}

// =========================================================================
// 2. ТЕСТЫ КОПИРОВАНИЯ И ПЕРЕМЕЩЕНИЯ (RULE OF FIVE)
// =========================================================================

TEST(SquareMatrixTest, DeepCopyConstructor) {
    SquareMatrix<double> original(2, 3.14);
    SquareMatrix<double> copy(original);

    // Изменяем копию, проверяем, что оригинал не тронут (Deep Copy)
    copy(0, 1) = 99.0;
    EXPECT_DOUBLE_EQ(original(0, 1), 3.14);
    EXPECT_DOUBLE_EQ(copy(0, 1), 99.0);
}

TEST(SquareMatrixTest, CopyAssignmentOperator) {
    SquareMatrix<double> original(2, 5.0);
    SquareMatrix<double> assigned(4); // Специально берем другой размер

    assigned = original;

    // Размер должен корректно обновиться
    EXPECT_EQ(assigned.GetRows(), 2);
    EXPECT_DOUBLE_EQ(assigned(1, 1), 5.0);

    // Самоприсваивание не должно ломать память (защита if (this != &other))
    assigned = assigned;
    EXPECT_EQ(assigned.GetRows(), 2);
    EXPECT_DOUBLE_EQ(assigned(0, 0), 5.0);
}

TEST(SquareMatrixTest, MoveSemantics) {
    SquareMatrix<double> source(3, 8.0);

    // Move-конструктор
    SquareMatrix<double> target(std::move(source));
    EXPECT_EQ(target.GetRows(), 3);
    EXPECT_DOUBLE_EQ(target(2, 2), 8.0);

    // Источник должен стать пустым (защита от Double Free)
    EXPECT_EQ(source.GetRows(), 0);

    // Move-присваивание
    SquareMatrix<double> target2(1);
    target2 = std::move(target);
    EXPECT_EQ(target2.GetRows(), 3);
    EXPECT_EQ(target.GetRows(), 0);
}

// =========================================================================
// 3. ДОСТУП К ПАМЯТИ И ЗАЩИТА ГРАНИЦ (BOUNDARY TESTING)
// =========================================================================

TEST(SquareMatrixTest, OperatorAccess) {
    SquareMatrix<double> A(2);
    A(0, 1) = 4.2;
    A(1, 0) = -1.1;

    EXPECT_DOUBLE_EQ(A(0, 1), 4.2);
    EXPECT_DOUBLE_EQ(A(1, 0), -1.1);

    // Проверка константного доступа
    const SquareMatrix<double>& cA = A;
    EXPECT_DOUBLE_EQ(cA(0, 1), 4.2);
}

TEST(SquareMatrixTest, OutOfBoundsAccessThrows) {
    SquareMatrix<double> A(2); // Индексы: 0, 1

    // Отрицательные индексы
    EXPECT_THROW(A(-1, 0), IndexOutOfRange);
    EXPECT_THROW(A(0, -1), IndexOutOfRange);
    EXPECT_THROW(A(-5, -5), IndexOutOfRange);

    // Индексы, выходящие за пределы (N и выше)
    EXPECT_THROW(A(2, 0), IndexOutOfRange);
    EXPECT_THROW(A(0, 2), IndexOutOfRange);
    EXPECT_THROW(A(10, 10), IndexOutOfRange);
}

// =========================================================================
// 4. ТЕСТЫ МАНИПУЛЯЦИИ СО СТРОКАМИ (ROW OPERATIONS)
// =========================================================================

TEST(SquareMatrixTest, SwapRows) {
    SquareMatrix<double> A(3);
    // Строка 0: 1, 2, 3
    A(0, 0) = 1.0; A(0, 1) = 2.0; A(0, 2) = 3.0;
    // Строка 2: 7, 8, 9
    A(2, 0) = 7.0; A(2, 1) = 8.0; A(2, 2) = 9.0;

    A.SwapRows(0, 2);

    // Строка 0 должна стать 7, 8, 9
    EXPECT_DOUBLE_EQ(A(0, 0), 7.0);
    EXPECT_DOUBLE_EQ(A(0, 2), 9.0);
    // Строка 2 должна стать 1, 2, 3
    EXPECT_DOUBLE_EQ(A(2, 0), 1.0);
    EXPECT_DOUBLE_EQ(A(2, 2), 3.0);

    // Свап одинаковых строк - это no-op (ничего не ломает)
    EXPECT_NO_THROW(A.SwapRows(1, 1));
}

TEST(SquareMatrixTest, SwapRowsOutOfBoundsThrows) {
    SquareMatrix<double> A(2);
    EXPECT_THROW(A.SwapRows(-1, 0), IndexOutOfRange);
    EXPECT_THROW(A.SwapRows(0, 2), IndexOutOfRange);
}

// =========================================================================
// 5. ТЕСТЫ БАЗОВОЙ МАТЕМАТИКИ И РАЗМЕРНОСТЕЙ (ALGEBRA)
// =========================================================================

TEST(SquareMatrixTest, Addition) {
    SquareMatrix<double> A(2, 1.0);
    SquareMatrix<double> B(2, 2.5);

    SquareMatrix<double> sum = A + B;
    EXPECT_DOUBLE_EQ(sum(0, 0), 3.5);
    EXPECT_DOUBLE_EQ(sum(1, 1), 3.5);
}

TEST(SquareMatrixTest, AdditionDimensionMismatchThrows) {
    SquareMatrix<double> A(2, 1.0);
    SquareMatrix<double> B(3, 1.0); // Другой размер

    EXPECT_THROW(A + B, InvalidArgument);
}

TEST(SquareMatrixTest, ScalarMultiplication) {
    SquareMatrix<double> A(2, -3.0);
    SquareMatrix<double> res = A * 4.0;

    EXPECT_DOUBLE_EQ(res(0, 1), -12.0);
    EXPECT_DOUBLE_EQ(res(1, 0), -12.0);
}

// =========================================================================
// 6. ТЕСТЫ УМНОЖЕНИЯ НА ВЕКТОР И НОРМЫ (ADVANCED MATH)
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
    // b[0] = 1*1 + 2*2 + 3*3 = 14
    // b[1] = 4*1 + 5*2 + 6*3 = 32
    // b[2] = 7*1 + 8*2 + 9*3 = 50
    Vector<double> b = A * x;

    EXPECT_EQ(b.GetSize(), 3);
    EXPECT_DOUBLE_EQ(b[0], 14.0);
    EXPECT_DOUBLE_EQ(b[1], 32.0);
    EXPECT_DOUBLE_EQ(b[2], 50.0);
}

TEST(SquareMatrixTest, MatrixVectorMismatchThrows) {
    SquareMatrix<double> A(3);
    Vector<double> x(2); // Вектор не подходит по размеру

    EXPECT_THROW(A * x, InvalidArgument);
}

TEST(SquareMatrixTest, NormCalculation) {
    // Октаэдрическая норма (Максимальная сумма модулей элементов по строкам)
    SquareMatrix<double> A(3);

    // Сумма = |-1| + |2| + |-3| = 6
    A(0, 0) = -1.0; A(0, 1) = 2.0; A(0, 2) = -3.0;

    // Сумма = |5| + |0| + |5| = 10
    A(1, 0) = 5.0; A(1, 1) = 0.0; A(1, 2) = 5.0;

    // Сумма = |-8| + |-2| + |-1| = 11 (Максимум!)
    A(2, 0) = -8.0; A(2, 1) = -2.0; A(2, 2) = -1.0;

    EXPECT_DOUBLE_EQ(A.Norm(), 11.0);
}