#include <gtest/gtest.h>
#include "../src/MatrixSolver.hpp"
#include "../src/SquareMatrix.hpp"
#include "../src/Vector.hpp"
#include "../src/Exceptions.hpp"
#include "../src/MathUtils.hpp"

// Допуск для проверки чисел с плавающей точкой
const double EPSILON = 1e-9;

// =========================================================================
// 1. ТЕСТЫ МЕТОДА ГАУССА (GAUSSIAN ELIMINATION)
// =========================================================================

TEST(MatrixSolverTest, GaussSimpleSystem) {
    // Система уравнений:
    // 2x + 1y = 5
    // 1x - 3y = -1
    // Ожидаемый ответ: x = 2, y = 1
    
    SquareMatrix<double> A(2);
    A(0, 0) = 2.0; A(0, 1) = 1.0;
    A(1, 0) = 1.0; A(1, 1) = -3.0;

    Vector<double> b(2);
    b[0] = 5.0; 
    b[1] = -1.0;

    Vector<double> solution = MatrixSolver<double>::SolveGauss(A, b);

    EXPECT_EQ(solution.GetSize(), 2);
    EXPECT_NEAR(solution[0], 2.0, EPSILON);
    EXPECT_NEAR(solution[1], 1.0, EPSILON);
}

TEST(MatrixSolverTest, GaussWithPartialPivoting) {
    // Система, где a[0][0] = 0. Требуется перестановка строк!
    // 0x + 2y = 4
    // 3x - 1y = 5
    // Ожидаемый ответ: x = 2.333333..., y = 2
    
    SquareMatrix<double> A(2);
    A(0, 0) = 0.0; A(0, 1) = 2.0;
    A(1, 0) = 3.0; A(1, 1) = -1.0;

    Vector<double> b(2);
    b[0] = 4.0;
    b[1] = 5.0;

    // Если метод Гаусса написан без выбора главного элемента (Pivoting),
    // этот тест упадет из-за деления на ноль.
    Vector<double> solution = MatrixSolver<double>::SolveGauss(A, b);

    EXPECT_NEAR(solution[1], 2.0, EPSILON);
    EXPECT_NEAR(solution[0], 7.0 / 3.0, EPSILON); // 3x - 2 = 5 -> 3x = 7
}

TEST(MatrixSolverTest, GaussLargeSystem) {
    // Система 3x3:
    // 3x + 2y - z = 1
    // 2x - 2y + 4z = -2
    // -x + 0.5y - z = 0
    // Ответ: x = 1, y = -2, z = -2
    
    SquareMatrix<double> A(3);
    A(0, 0) = 3.0;  A(0, 1) = 2.0;  A(0, 2) = -1.0;
    A(1, 0) = 2.0;  A(1, 1) = -2.0; A(1, 2) = 4.0;
    A(2, 0) = -1.0; A(2, 1) = 0.5;  A(2, 2) = -1.0;

    Vector<double> b(3);
    b[0] = 1.0; b[1] = -2.0; b[2] = 0.0;

    Vector<double> solution = MatrixSolver<double>::SolveGauss(A, b);

    EXPECT_NEAR(solution[0], 1.0, EPSILON);
    EXPECT_NEAR(solution[1], -2.0, EPSILON);
    EXPECT_NEAR(solution[2], -2.0, EPSILON);
}

// =========================================================================
// 2. ЗАЩИТА ОТ СИНГУЛЯРНОСТИ И ОШИБОК РАЗМЕРНОСТЕЙ
// =========================================================================

TEST(MatrixSolverTest, GaussSingularMatrixThrows) {
    // Матрица с линейно зависимыми строками (Определитель = 0)
    // 1x + 2y = 3
    // 2x + 4y = 6
    SquareMatrix<double> A(2);
    A(0, 0) = 1.0; A(0, 1) = 2.0;
    A(1, 0) = 2.0; A(1, 1) = 4.0;

    Vector<double> b(2);
    b[0] = 3.0; b[1] = 6.0;

    // Решатель должен понять, что однозначного решения нет
    EXPECT_THROW(MatrixSolver<double>::SolveGauss(A, b), DivideByZero); 
    // Либо SingularMatrixException, в зависимости от твоей реализации
}

TEST(MatrixSolverTest, GaussDimensionMismatchThrows) {
    SquareMatrix<double> A(3);
    Vector<double> b(2); // Вектор не совпадает по размеру с матрицей

    EXPECT_THROW(MatrixSolver<double>::SolveGauss(A, b), InvalidArgument);
}

// =========================================================================
// 3. ТЕСТЫ LU-РАЗЛОЖЕНИЯ (LU DECOMPOSITION)
// =========================================================================

TEST(MatrixSolverTest, LUDecompositionCorrectness) {
    // Разлагаем матрицу A на L (нижнюю) и U (верхнюю)
    // A = | 2 3 |
    //     | 4 7 |
    SquareMatrix<double> A(2);
    A(0, 0) = 2.0; A(0, 1) = 3.0;
    A(1, 0) = 4.0; A(1, 1) = 7.0;

    // Функция должна возвращать пару матриц: Pair<TriangularMatrix, TriangularMatrix>
    // Или структуру с L и U. Предполагаем, что интерфейс возвращает структуру LUResult.
    auto result = MatrixSolver<double>::DecomposeLU(A);
    auto L = result.L;
    auto U = result.U;

    EXPECT_TRUE(L.IsLower());
    EXPECT_FALSE(U.IsLower()); // U - верхнетреугольная

    // Проверяем, что на главной диагонали L стоят единицы (по алгоритму Дулиттла)
    EXPECT_NEAR(L(0, 0), 1.0, EPSILON);
    EXPECT_NEAR(L(1, 1), 1.0, EPSILON);

    // Математическая проверка: A_ij = Сумма(L_ik * U_kj)
    // Восстанавливаем A из L и U вручную и сверяем с оригиналом
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

    // Классическое LU-разложение без перестановок падает на нулевом главном миноре
    EXPECT_THROW(MatrixSolver<double>::DecomposeLU(A), DivideByZero);
}