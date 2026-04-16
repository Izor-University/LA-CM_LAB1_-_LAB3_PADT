#include <gtest/gtest.h>
#include "../src/Vector.hpp"
#include "../src/Exceptions.hpp"
#include "../src/Complex.hpp"

// =========================================================================
// 1. ТЕСТЫ ИНИЦИАЛИЗАЦИИ И ПАМЯТИ (LIFECYCLE & MEMORY)
// =========================================================================

TEST(VectorTest, DefaultInitialization) {
    Vector<double> v(5);
    EXPECT_EQ(v.GetSize(), 5);

    // Память должна быть инициализирована нулями типа T
    for (int i = 0; i < 5; ++i) {
        EXPECT_DOUBLE_EQ(v[i], 0.0);
    }
}

TEST(VectorTest, ValueInitialization) {
    Vector<double> v(3, -7.25);
    EXPECT_EQ(v.GetSize(), 3);
    EXPECT_DOUBLE_EQ(v[0], -7.25);
    EXPECT_DOUBLE_EQ(v[1], -7.25);
    EXPECT_DOUBLE_EQ(v[2], -7.25);
}

TEST(VectorTest, InvalidInitializationThrows) {
    // Вектор не может иметь отрицательный или нулевой размер
    EXPECT_THROW(Vector<double>(0), IndexOutOfRange);
    EXPECT_THROW(Vector<double>(-10), IndexOutOfRange);
    EXPECT_THROW(Vector<double>(0, 5.0), IndexOutOfRange);
}

TEST(VectorTest, DeepCopySemantics) {
    Vector<double> original(4, 3.14);

    // Copy-конструктор
    Vector<double> copy(original);
    EXPECT_EQ(copy.GetSize(), 4);

    // Мутируем копию, чтобы проверить глубокое копирование (Deep Copy)
    copy[2] = 99.0;
    EXPECT_DOUBLE_EQ(original[2], 3.14); // Оригинал не должен пострадать
    EXPECT_DOUBLE_EQ(copy[2], 99.0);

    // Copy-оператор присваивания
    Vector<double> assigned(2); // Специально берем вектор другого размера
    assigned = original;
    EXPECT_EQ(assigned.GetSize(), 4); // Размер должен перераспределиться
    EXPECT_DOUBLE_EQ(assigned[3], 3.14);

    // Самоприсваивание не должно ломать данные
    assigned = assigned;
    EXPECT_EQ(assigned.GetSize(), 4);
    EXPECT_DOUBLE_EQ(assigned[0], 3.14);
}

TEST(VectorTest, MoveSemantics) {
    Vector<double> source(5, 42.0);

    // Move-конструктор
    Vector<double> target(std::move(source));
    EXPECT_EQ(target.GetSize(), 5);
    EXPECT_DOUBLE_EQ(target[4], 42.0);

    // Источник должен стать "опустошенным" (защита от двойного удаления)
    EXPECT_EQ(source.GetSize(), 0);

    // Move-оператор присваивания
    Vector<double> target2(1);
    target2 = std::move(target);
    EXPECT_EQ(target2.GetSize(), 5);
    EXPECT_DOUBLE_EQ(target2[0], 42.0);
    EXPECT_EQ(target.GetSize(), 0);
}

// =========================================================================
// 2. ТЕСТЫ ДОСТУПА К ПАМЯТИ И ГРАНИЦ (BOUNDARY TESTING)
// =========================================================================

TEST(VectorTest, ElementAccess) {
    Vector<double> v(3);
    v[0] = 10.5;
    v[1] = -5.0;
    v[2] = 0.0;

    EXPECT_DOUBLE_EQ(v[0], 10.5);
    EXPECT_DOUBLE_EQ(v[1], -5.0);

    // Проверка константного доступа
    const Vector<double>& cv = v;
    EXPECT_DOUBLE_EQ(cv[0], 10.5);
}

TEST(VectorTest, OutOfBoundsAccessThrows) {
    Vector<double> v(4); // Допустимые индексы: 0, 1, 2, 3

    // Левая граница
    EXPECT_THROW(v[-1], IndexOutOfRange);
    EXPECT_THROW(v[-100], IndexOutOfRange);

    // Правая граница
    EXPECT_THROW(v[4], IndexOutOfRange); // off-by-one ошибка
    EXPECT_THROW(v[10], IndexOutOfRange);
}

// =========================================================================
// 3. ТЕСТЫ АЛГЕБРЫ И ИСКЛЮЧЕНИЙ РАЗМЕРНОСТИ (MATH & ALGEBRA)
// =========================================================================

TEST(VectorTest, AdditionAndSubtraction) {
    Vector<double> v1(3); v1[0] = 1.0; v1[1] = 2.0; v1[2] = 3.0;
    Vector<double> v2(3); v2[0] = 4.0; v2[1] = 5.0; v2[2] = 6.0;

    Vector<double> sum = v1 + v2;
    EXPECT_DOUBLE_EQ(sum[0], 5.0);
    EXPECT_DOUBLE_EQ(sum[1], 7.0);
    EXPECT_DOUBLE_EQ(sum[2], 9.0);

    Vector<double> diff = v1 - v2;
    EXPECT_DOUBLE_EQ(diff[0], -3.0);
    EXPECT_DOUBLE_EQ(diff[2], -3.0);
}

TEST(VectorTest, MathematicalDimensionMismatchThrows) {
    Vector<double> v1(2, 1.0);
    Vector<double> v2(3, 1.0);

    // Алгебраические операции между векторами разной длины невозможны
    EXPECT_THROW(v1 + v2, InvalidArgument);
    EXPECT_THROW(v1 - v2, InvalidArgument);
    EXPECT_THROW(v1 * v2, InvalidArgument); // Скалярное произведение
}

TEST(VectorTest, ScalarMultiplicationAndChaining) {
    Vector<double> v(2); v[0] = 1.5; v[1] = -2.0;

    Vector<double> scaled = v * 4.0;
    EXPECT_DOUBLE_EQ(scaled[0], 6.0);
    EXPECT_DOUBLE_EQ(scaled[1], -8.0);

    // Проверка цепочечных вычислений: (v * 2) - v
    Vector<double> chained = (v * 2.0) - v;
    EXPECT_DOUBLE_EQ(chained[0], 1.5);
    EXPECT_DOUBLE_EQ(chained[1], -2.0);
}

TEST(VectorTest, DotProduct) {
    Vector<double> v1(3); v1[0] = 1.0; v1[1] = 2.0; v1[2] = 3.0;
    Vector<double> v2(3); v2[0] = 4.0; v2[1] = -5.0; v2[2] = 6.0;

    // 1*4 + 2*(-5) + 3*6 = 4 - 10 + 18 = 12
    EXPECT_DOUBLE_EQ(v1 * v2, 12.0);

    // Ортогональные векторы (оси X и Y)
    Vector<double> x(2); x[0] = 1.0; x[1] = 0.0;
    Vector<double> y(2); y[0] = 0.0; y[1] = 1.0;
    EXPECT_DOUBLE_EQ(x * y, 0.0);
}

TEST(VectorTest, NormLInfinity) {
    Vector<double> v1(4);
    v1[0] = 1.0;
    v1[1] = -15.5; // Максимальный по модулю
    v1[2] = 7.0;
    v1[3] = 0.0;

    // L-бесконечность норма вектора = max(|x_i|)
    EXPECT_DOUBLE_EQ(v1.Norm(), 15.5);

    // Норма нулевого вектора
    Vector<double> vZero(5, 0.0);
    EXPECT_DOUBLE_EQ(vZero.Norm(), 0.0);
}

// =========================================================================
// 4. ТЕСТЫ ШАБЛОНИЗАЦИИ И СЛОЖНЫХ ТИПОВ (TEMPLATE ROBUSTNESS)
// =========================================================================

TEST(VectorTest, WorksWithComplexNumbers) {
    // Вектор должен работать с любым АТД, поддерживающим математику
    Vector<Complex> cv1(2);
    cv1[0] = Complex(1.0, 2.0);  // 1 + 2i
    cv1[1] = Complex(-3.0, 4.0); // -3 + 4i

    Vector<Complex> cv2(2);
    cv2[0] = Complex(2.0, -1.0); // 2 - i
    cv2[1] = Complex(3.0, 0.0);  // 3

    // Сложение комплексных векторов
    Vector<Complex> sum = cv1 + cv2;
    EXPECT_DOUBLE_EQ(sum[0].re, 3.0);
    EXPECT_DOUBLE_EQ(sum[0].im, 1.0); // (1+2i) + (2-i) = 3+i

    EXPECT_DOUBLE_EQ(sum[1].re, 0.0);
    EXPECT_DOUBLE_EQ(sum[1].im, 4.0); // (-3+4i) + 3 = 4i

    // Норма комплексного вектора cv1
    // |1 + 2i| = sqrt(1 + 4) = 2.236
    // |-3 + 4i| = sqrt(9 + 16) = 5.0 (Максимум!)
    EXPECT_DOUBLE_EQ(cv1.Norm(), 5.0);
}