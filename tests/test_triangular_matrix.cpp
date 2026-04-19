#include <gtest/gtest.h>
#include "../src/TriangularMatrix.hpp"
#include "../LAB2/Exceptions.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ПАМЯТИ (LIFECYCLE & MEMORY)
// =========================================================================

TEST(TriangularMatrixTest, ValidInitialization) {
    TriangularMatrix<double> L(3, true);
    EXPECT_EQ(L.GetRows(), 3);
    EXPECT_EQ(L.GetCols(), 3);
    EXPECT_TRUE(L.IsLower());

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

    TriangularMatrix<double> copy(original);
    copy(2, 0) = 99.0;

    EXPECT_DOUBLE_EQ(original(2, 0), 42.0);
    EXPECT_DOUBLE_EQ(copy(2, 0), 99.0);
    EXPECT_TRUE(copy.IsLower());

    TriangularMatrix<double> assigned(2, false);
    assigned = original;
    EXPECT_TRUE(assigned.IsLower());
    EXPECT_EQ(assigned.GetRows(), 3);
    EXPECT_DOUBLE_EQ(assigned(2, 0), 42.0);
}

TEST(TriangularMatrixTest, MoveSemantics) {
    TriangularMatrix<double> source(3, false);
    source(0, 2) = -15.5;

    TriangularMatrix<double> target(std::move(source));
    EXPECT_EQ(target.GetRows(), 3);
    EXPECT_FALSE(target.IsLower());
    EXPECT_DOUBLE_EQ(target(0, 2), -15.5);
    EXPECT_EQ(source.GetRows(), 0);

    TriangularMatrix<double> target2(1, true);
    target2 = std::move(target);
    EXPECT_FALSE(target2.IsLower());
    EXPECT_EQ(target2.GetRows(), 3);
}

// =========================================================================
// 2. ИНВАРИАНТЫ НИЖНЕТРЕУГОЛЬНОЙ МАТРИЦЫ (LOWER TRIANGLE)
// =========================================================================

TEST(TriangularMatrixTest, LowerTriangleValidAccess) {
    TriangularMatrix<double> L(3, true);
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

    // ИСПРАВЛЕНИЕ: Читаем через const ссылку
    const TriangularMatrix<double>& cL = L;

    EXPECT_DOUBLE_EQ(cL(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(cL(0, 2), 0.0);
    EXPECT_DOUBLE_EQ(cL(1, 2), 0.0);
}

TEST(TriangularMatrixTest, LowerTriangleVirtualZerosWriteProtection) {
    TriangularMatrix<double> L(3, true);

    EXPECT_THROW(L(0, 1) = 5.0, InvalidArgument);
    EXPECT_THROW(L(1, 2) = -3.14, InvalidArgument);
    EXPECT_NO_THROW(L.Set(0, 2, 0.0));
}

// =========================================================================
// 3. ИНВАРИАНТЫ ВЕРХНЕТРЕУГОЛЬНОЙ МАТРИЦЫ (UPPER TRIANGLE)
// =========================================================================

TEST(TriangularMatrixTest, UpperTriangleValidAccess) {
    TriangularMatrix<double> U(3, false);
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

    // ИСПРАВЛЕНИЕ: Читаем через const ссылку
    const TriangularMatrix<double>& cU = U;

    EXPECT_DOUBLE_EQ(cU(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(cU(2, 0), 0.0);
    EXPECT_DOUBLE_EQ(cU(2, 1), 0.0);
}

TEST(TriangularMatrixTest, UpperTriangleVirtualZerosWriteProtection) {
    TriangularMatrix<double> U(3, false);

    EXPECT_THROW(U(1, 0) = 5.0, InvalidArgument);
    EXPECT_THROW(U(2, 1) = -3.14, InvalidArgument);
    EXPECT_NO_THROW(U.Set(2, 0, 0.0));
}

// =========================================================================
// 4. ТЕСТЫ ГРАНИЦ И ИСКЛЮЧЕНИЙ (OUT OF BOUNDS)
// =========================================================================

TEST(TriangularMatrixTest, OutOfBoundsAccessThrows) {
    TriangularMatrix<double> T(2, true);

    EXPECT_THROW(T(-1, 0), IndexOutOfRange);
    EXPECT_THROW(T(0, -1), IndexOutOfRange);
    EXPECT_THROW(T(-1, -1), IndexOutOfRange);

    EXPECT_THROW(T(2, 0), IndexOutOfRange);
    EXPECT_THROW(T(0, 2), IndexOutOfRange);
    EXPECT_THROW(T(5, 5), IndexOutOfRange);
}