#include <gtest/gtest.h>
#include "../src/TriangularMatrix.hpp"
#include "../src/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ПАМЯТИ (LIFECYCLE & MEMORY)
// =========================================================================

TEST(TriangularMatrixTest, ValidInitialization) {
    // Проверка нижнетреугольной матрицы
    TriangularMatrix<double> L(3, true);
    EXPECT_EQ(L.GetRows(), 3);
    EXPECT_EQ(L.GetCols(), 3);
    EXPECT_TRUE(L.IsLower());

    // Проверка верхнетреугольной матрицы
    TriangularMatrix<double> U(4, false);
    EXPECT_EQ(U.GetRows(), 4);
    EXPECT_EQ(U.GetCols(), 4);
    EXPECT_FALSE(U.IsLower());
}

TEST(TriangularMatrixTest, InvalidInitializationThrows) {
    EXPECT_THROW(TriangularMatrix<double>(0, true), IndexOutOfRange);
    EXPECT_THROW(TriangularMatrix<double>(-5, false), IndexOutOfRange);
}

TEST(TriangularMatrixTest, DeepCopySemantics) {
    TriangularMatrix<double> original(3, true);
    original(2, 0) = 42.0;

    // Copy-конструктор
    TriangularMatrix<double> copy(original);

    // Мутируем копию
    copy(2, 0) = 99.0;

    // Проверяем изоляцию памяти
    EXPECT_DOUBLE_EQ(original(2, 0), 42.0);
    EXPECT_DOUBLE_EQ(copy(2, 0), 99.0);

    // Проверяем, что флаг скопировался
    EXPECT_TRUE(copy.IsLower());

    // Copy-присваивание
    TriangularMatrix<double> assigned(2, false); // Изначально Upper
    assigned = original; // Присваиваем Lower
    EXPECT_TRUE(assigned.IsLower()); // Флаг должен обновиться
    EXPECT_EQ(assigned.GetRows(), 3);
    EXPECT_DOUBLE_EQ(assigned(2, 0), 42.0);
}

TEST(TriangularMatrixTest, MoveSemantics) {
    TriangularMatrix<double> source(3, false); // Верхнетреугольная
    source(0, 2) = -15.5;

    // Move-конструктор
    TriangularMatrix<double> target(std::move(source));
    EXPECT_EQ(target.GetRows(), 3);
    EXPECT_FALSE(target.IsLower()); // Флаг обязан сохраниться!
    EXPECT_DOUBLE_EQ(target(0, 2), -15.5);

    // Источник должен быть опустошен
    EXPECT_EQ(source.GetRows(), 0);

    // Move-присваивание
    TriangularMatrix<double> target2(1, true);
    target2 = std::move(target);
    EXPECT_FALSE(target2.IsLower()); // Флаг должен перенестись
    EXPECT_EQ(target2.GetRows(), 3);
}

// =========================================================================
// 2. ИНВАРИАНТЫ НИЖНЕТРЕУГОЛЬНОЙ МАТРИЦЫ (LOWER TRIANGLE)
// =========================================================================

TEST(TriangularMatrixTest, LowerTriangleValidAccess) {
    TriangularMatrix<double> L(3, true);
    // Разрешенная зона: row >= col
    EXPECT_NO_THROW({
        L(0, 0) = 1.0;
        L(1, 0) = 2.0; L(1, 1) = 3.0;
        L(2, 0) = 4.0; L(2, 1) = 5.0; L(2, 2) = 6.0;
    });

    EXPECT_DOUBLE_EQ(L(2, 1), 5.0);
}

TEST(TriangularMatrixTest, LowerTriangleVirtualZerosRead) {
    TriangularMatrix<double> L(3, true);
    L(0, 0) = 1.0; L(1, 1) = 2.0; L(2, 2) = 3.0;

    // Зона виртуальных нулей: row < col
    EXPECT_DOUBLE_EQ(L(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(L(0, 2), 0.0);
    EXPECT_DOUBLE_EQ(L(1, 2), 0.0);
}

TEST(TriangularMatrixTest, LowerTriangleVirtualZerosWriteProtection) {
    TriangularMatrix<double> L(3, true);

    // Попытка записать ненулевое значение вне треугольника строго запрещена
    EXPECT_THROW(L(0, 1) = 5.0, InvalidArgument);
    EXPECT_THROW(L(1, 2) = -3.14, InvalidArgument);

    // Но записать туда ноль - легально, это не ломает инвариант!
    EXPECT_NO_THROW(L.Set(0, 2, 0.0));
}

// =========================================================================
// 3. ИНВАРИАНТЫ ВЕРХНЕТРЕУГОЛЬНОЙ МАТРИЦЫ (UPPER TRIANGLE)
// =========================================================================

TEST(TriangularMatrixTest, UpperTriangleValidAccess) {
    TriangularMatrix<double> U(3, false);
    // Разрешенная зона: row <= col
    EXPECT_NO_THROW({
        U(0, 0) = 1.0; U(0, 1) = 2.0; U(0, 2) = 3.0;
                       U(1, 1) = 4.0; U(1, 2) = 5.0;
                                      U(2, 2) = 6.0;
    });

    EXPECT_DOUBLE_EQ(U(0, 2), 3.0);
}

TEST(TriangularMatrixTest, UpperTriangleVirtualZerosRead) {
    TriangularMatrix<double> U(3, false);
    U(0, 0) = 1.0; U(1, 1) = 2.0; U(2, 2) = 3.0;

    // Зона виртуальных нулей: row > col
    EXPECT_DOUBLE_EQ(U(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(U(2, 0), 0.0);
    EXPECT_DOUBLE_EQ(U(2, 1), 0.0);
}

TEST(TriangularMatrixTest, UpperTriangleVirtualZerosWriteProtection) {
    TriangularMatrix<double> U(3, false);

    // Попытка записать ненулевое значение под главную диагональ запрещена
    EXPECT_THROW(U(1, 0) = 5.0, InvalidArgument);
    EXPECT_THROW(U(2, 1) = -3.14, InvalidArgument);

    // Запись нуля легальна
    EXPECT_NO_THROW(U.Set(2, 0, 0.0));
}

// =========================================================================
// 4. ТЕСТЫ ГРАНИЦ И ИСКЛЮЧЕНИЙ (OUT OF BOUNDS)
// =========================================================================

TEST(TriangularMatrixTest, OutOfBoundsAccessThrows) {
    TriangularMatrix<double> T(2, true); // Индексы 0, 1

    // Отрицательные индексы
    EXPECT_THROW(T(-1, 0), IndexOutOfRange);
    EXPECT_THROW(T(0, -1), IndexOutOfRange);
    EXPECT_THROW(T(-1, -1), IndexOutOfRange);

    // Индексы, превышающие размер
    EXPECT_THROW(T(2, 0), IndexOutOfRange);
    EXPECT_THROW(T(0, 2), IndexOutOfRange);
    EXPECT_THROW(T(5, 5), IndexOutOfRange);
}