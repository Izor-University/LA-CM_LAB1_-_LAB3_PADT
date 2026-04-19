#include <gtest/gtest.h>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include "../src/MatrixSolver.hpp"

using namespace std;
using namespace std::chrono;

// Вспомогательная функция для заголовков в консоли
void PrintBenchmarkHeader(const string& title) {
    cout << "\n======================================================================\n";
    cout << " [БЕНЧМАРК ЛР-1] " << title << "\n";
    cout << "======================================================================\n";
}

// =========================================================================
// ЭКСПЕРИМЕНТ 1: СРАВНЕНИЕ ВРЕМЕНИ НА СЛУЧАЙНЫХ МАТРИЦАХ
// =========================================================================
TEST(LR1_Benchmarks, TimeComparison) {
    PrintBenchmarkHeader("Сравнение времени решения одной системы (Гаусс vs LU)");
    
    // Размеры из ТЗ: 100, 200, 500
    int sizes[] = {100, 200, 500, 1000};

    for (int n : sizes) {
        cout << "\nРазмер матрицы N = " << n << "x" << n << "\n";
        cout << left << setw(30) << "Метод" << "| Время (мс)\n";
        cout << "----------------------------------------------\n";

        auto A = MatrixSolver<double>::GenerateRandomMatrix(n, 42);
        auto b = MatrixSolver<double>::GenerateRandomVector(n, 42);

        // 1. Гаусс без выбора
        auto start = high_resolution_clock::now();
        MatrixSolver<double>::SolveGaussNoPivot(A, b);
        auto duration_gauss = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        cout << left << setw(30) << "Гаусс (без выбора)" << "| " << duration_gauss << "\n";
        
        // 2. Гаусс с выбором
        start = high_resolution_clock::now();
        MatrixSolver<double>::SolveGaussPartialPivot(A, b);
        auto duration_gauss_pivot = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        cout << left << setw(30) << "Гаусс (с выбором)" << "| " << duration_gauss_pivot << "\n";

        // 3. LU разложение + решение
        start = high_resolution_clock::now();
        auto lu = MatrixSolver<double>::DecomposeLU(A);
        auto duration_lu_decomp = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();
        
        auto lu_start_solve = high_resolution_clock::now();
        MatrixSolver<double>::SolveUsingLU(lu.first, lu.second, b);
        auto duration_lu_solve = duration_cast<milliseconds>(high_resolution_clock::now() - lu_start_solve).count();
        auto duration_lu_total = duration_cast<milliseconds>(high_resolution_clock::now() - start).count();

        cout << left << setw(30) << "LU (только разложение)" << "| " << duration_lu_decomp << "\n";
        cout << left << setw(30) << "LU (только подстановка)"<< "| " << duration_lu_solve << "\n";
        cout << left << setw(30) << "LU (Полный цикл)" << "| " << duration_lu_total << "\n";
    }
}

// =========================================================================
// ЭКСПЕРИМЕНТ 2: ПЛОХО ОБУСЛОВЛЕННЫЕ МАТРИЦЫ (ГИЛЬБЕРТ)
// =========================================================================
TEST(LR1_Benchmarks, HilbertAccuracy) {
    PrintBenchmarkHeader("Проверка точности на матрицах Гильберта");
    
    // Размеры из ТЗ: 5, 10, 15
    int sizes[] = {5, 10, 15};

    for (int n : sizes) {
        cout << "\nМатрица Гильберта N = " << n << "x" << n << "\n";
        
        auto H = MatrixSolver<double>::GenerateHilbertMatrix(n);
        auto x_exact = MatrixSolver<double>::GenerateOnesVector(n);
        Vector<double> b = H * x_exact;

        // Гаусс без выбора
        try {
            auto x_approx_no_pivot = MatrixSolver<double>::SolveGaussNoPivot(H, b);
            double err_no_pivot = MatrixSolver<double>::CalculateRelativeError(x_exact, x_approx_no_pivot);
            cout << "Гаусс (без выбора) Погрешность: " << scientific << err_no_pivot << "\n";
        } catch (const Exception& e) {
            cout << "Гаусс (без выбора) ошибка: " << e.GetMessage() << "\n";
        }

        // Гаусс с выбором
        try {
            auto x_approx_pivot = MatrixSolver<double>::SolveGaussPartialPivot(H, b);
            double err_pivot = MatrixSolver<double>::CalculateRelativeError(x_exact, x_approx_pivot);
            cout << "Гаусс (с выбором)  Погрешность: " << scientific << err_pivot << "\n";
        } catch (const Exception& e) {
            cout << "Гаусс (с выбором) ошибка: " << e.GetMessage() << "\n";
        }
    }
    // Возвращаем формат вывода в норму
    cout << defaultfloat; 
}

// =========================================================================
// ЭКСПЕРИМЕНТ 3: МНОЖЕСТВЕННЫЕ ПРАВЫЕ ЧАСТИ (LU vs Гаусс)
// =========================================================================
TEST(LR1_Benchmarks, MultipleRHS) {
    PrintBenchmarkHeader("Экономия времени при множественных правых частях");
    
    int n = 500; // Фиксируем размер матрицы, как сказано в ТЗ
    cout << "\nМатрица фиксированного размера: N = " << n << "x" << n << "\n";
    cout << left << setw(10) << "K (шт)" 
         << "| " << setw(25) << "Гаусс с выбором (мс)" 
         << "| " << "LU Общее время (мс)\n";
    cout << "------------------------------------------------------------\n";

    auto A = MatrixSolver<double>::GenerateRandomMatrix(n, 42);
    vector<int> k_values = {1, 10, 50}; // Берем k векторов правой части (100 может быть долго для Гаусса)

    for (int k : k_values) {
        // Генерируем K случайных правых частей
        vector<Vector<double>> b_vectors;
        for (int i = 0; i < k; ++i) {
            b_vectors.push_back(MatrixSolver<double>::GenerateRandomVector(n, 100 + i));
        }

        // --- Замер: Гаусс с выбором (k полных циклов) ---
        auto start_gauss = high_resolution_clock::now();
        for (int i = 0; i < k; ++i) {
            MatrixSolver<double>::SolveGaussPartialPivot(A, b_vectors[i]);
        }
        auto duration_gauss = duration_cast<milliseconds>(high_resolution_clock::now() - start_gauss).count();

        // --- Замер: LU разложение ---
        auto start_lu = high_resolution_clock::now();
        
        // 1. Делаем тяжелое разложение ровно ОДИН раз!
        auto lu = MatrixSolver<double>::DecomposeLU(A);
        
        // 2. Делаем быстрые подстановки k раз!
        for (int i = 0; i < k; ++i) {
            MatrixSolver<double>::SolveUsingLU(lu.first, lu.second, b_vectors[i]);
        }
        auto duration_lu = duration_cast<milliseconds>(high_resolution_clock::now() - start_lu).count();

        // Вывод строки таблицы
        cout << left << setw(10) << k 
             << "| " << setw(25) << duration_gauss 
             << "| " << duration_lu << "\n";
    }
}