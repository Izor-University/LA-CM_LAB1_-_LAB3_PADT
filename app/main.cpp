#include <iostream>
#include <iomanip>
#include <ctime>
#include <limits>

// Подключаем все наши классы
#include "../LAB2/LinkedList.hpp"
#include "../src/IMatrix.hpp"
#include "../src/Matrix.hpp"
#include "../src/SquareMatrix.hpp"
#include "../src/TriangularMatrix.hpp"
#include "../src/DiagonalMatrix.hpp"
#include "../src/Vector.hpp"
#include "../src/MatrixSolver.hpp"
#include "../LAB2/Exceptions.hpp"
#include "../src/MathUtils.hpp"

// Утилиты кроссплатформенного интерфейса
void ClearScreen() {
    std::cout << "\x1B[2J\x1B[H" << std::flush;
}

void WaitEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.clear();
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

// Безопасный ввод
int ReadInt(const char* prompt = "") {
    int value;
    while (true) {
        if (*prompt) {
            std::cout << prompt;
        }
        std::cin >> value;
        if (!std::cin.fail()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof() || std::cin.bad()) {
            std::exit(1);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Error. Please enter an integer.\n";
    }
}

double ReadDouble(const char* prompt = "") {
    double value;
    while (true) {
        if (*prompt) {
            std::cout << prompt;
        }
        std::cin >> value;
        if (!std::cin.fail()) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return value;
        }
        if (std::cin.eof() || std::cin.bad()) {
            std::exit(1);
        }
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "  [!] Error. Please enter a number.\n";
    }
}

// Форматированный вывод
void PrintVectorCompact(const Vector<double>& v) {
    int n = v.GetSize();
    std::cout << "[ ";
    int limit = n > 5 ? 5 : n;
    for (int i = 0; i < limit; ++i) {
        std::cout << v[i] << " ";
    }
    if (n > 5) {
        std::cout << "... (" << n << " elements) ";
    }
    std::cout << "]";
}

// Полиморфный вывод любой матрицы через интерфейс IMatrix
void PrintMatrixCompact(const IMatrix<double>* m) {
    int r = m->GetRows();
    int c = m->GetCols();
    if (r <= 5 && c <= 5) {
        std::cout << "\n";
        for (int i = 0; i < r; ++i) {
            std::cout << "      | ";
            for (int j = 0; j < c; ++j) {
                std::cout << std::setw(8) << std::setprecision(4) << m->Get(i, j) << " ";
            }
            std::cout << "|\n";
        }
    } else {
        std::cout << "[Matrix " << r << "x" << c << " (Too large to display)]";
    }
}

// Система реестров
struct MatrixRecord {
    const char* typeName;
    IMatrix<double>* mat; // УНИВЕРСАЛЬНЫЙ УКАЗАТЕЛЬ
};

struct VectorRecord {
    const char* typeName;
    Vector<double>* vec;
};

LinkedList<MatrixRecord*> g_MatrixRegistry;
LinkedList<VectorRecord*> g_VectorRegistry;

void PrintRegistries() {
    std::cout << "--- UNIVERSAL MATRICES REGISTRY ---\n";
    if (g_MatrixRegistry.GetLength() == 0) {
        std::cout << "  (Empty)\n";
    }
    for (int i = 0; i < g_MatrixRegistry.GetLength(); ++i) {
        MatrixRecord* rec = g_MatrixRegistry.Get(i);
        std::cout << "  [" << i << "] " << rec->typeName << " ";
        PrintMatrixCompact(rec->mat);
        if (rec->mat->GetRows() > 5) {
            std::cout << "\n";
        }
    }

    std::cout << "\n--- VECTORS REGISTRY ---\n";
    if (g_VectorRegistry.GetLength() == 0) {
        std::cout << "  (Empty)\n";
    }
    for (int i = 0; i < g_VectorRegistry.GetLength(); ++i) {
        VectorRecord* rec = g_VectorRegistry.Get(i);
        std::cout << "  [" << i << "] " << rec->typeName << " : ";
        PrintVectorCompact(*(rec->vec));
        std::cout << "\n";
    }
    std::cout << "-----------------------------------\n";
}

int PickMatrix() {
    if (g_MatrixRegistry.GetLength() == 0) {
        return -1;
    }
    while (true) {
        int idx = ReadInt("Select Matrix ID (-1 to cancel): ");
        if (idx == -1) return -1;
        if (idx >= 0 && idx < g_MatrixRegistry.GetLength()) return idx;
        std::cout << "  [!] ID out of bounds.\n";
    }
}

int PickVector() {
    if (g_VectorRegistry.GetLength() == 0) {
        return -1;
    }
    while (true) {
        int idx = ReadInt("Select Vector ID (-1 to cancel): ");
        if (idx == -1) return -1;
        if (idx >= 0 && idx < g_VectorRegistry.GetLength()) return idx;
        std::cout << "  [!] ID out of bounds.\n";
    }
}

// Удаление
void DeleteMatrix() {
    PrintRegistries();
    int idx = PickMatrix();
    if (idx == -1) return;

    MatrixRecord* rec = g_MatrixRegistry.Get(idx);
    delete rec->mat; // Виртуальный деструктор корректно очистит любую матрицу
    delete rec;
    g_MatrixRegistry.RemoveAt(idx);
    std::cout << "Matrix deleted successfully.\n";
}

void DeleteVector() {
    PrintRegistries();
    int idx = PickVector();
    if (idx == -1) return;

    VectorRecord* rec = g_VectorRegistry.Get(idx);
    delete rec->vec;
    delete rec;
    g_VectorRegistry.RemoveAt(idx);
    std::cout << "Vector deleted successfully.\n";
}

// Универсальный конвертер в прямоугольную матрицу (для смешанных операций)
Matrix<double> ConvertToGeneralMatrix(const IMatrix<double>* m) {
    Matrix<double> res(m->GetRows(), m->GetCols());
    for (int i = 0; i < m->GetRows(); ++i) {
        for (int j = 0; j < m->GetCols(); ++j) {
            res.Set(i, j, m->Get(i, j));
        }
    }
    return res;
}

// Вычисление нормы для любого типа матрицы
double GetUniversalNorm(const IMatrix<double>* m) {
    if (auto mat = dynamic_cast<const Matrix<double>*>(m)) return mat->Norm();
    if (auto diag = dynamic_cast<const DiagonalMatrix<double>*>(m)) return diag->Norm();

    // Fallback для TriangularMatrix (и ручной подсчет октаэдрической нормы)
    double maxNorm = 0.0;
    for (int i = 0; i < m->GetRows(); ++i) {
        double currentSum = 0.0;
        for (int j = 0; j < m->GetCols(); ++j) {
            currentSum += MathUtils::Abs(m->Get(i, j));
        }
        if (currentSum > maxNorm) maxNorm = currentSum;
    }
    return maxNorm;
}

// Меню создания универсальных матриц
void CreateMatrixMenu() {
    std::cout << "\n--- Create Matrix ---\n";
    std::cout << "1. Rectangular Matrix\n";
    std::cout << "2. Square Matrix\n";
    std::cout << "3. Diagonal Matrix\n";
    std::cout << "4. Triangular Matrix\n";
    std::cout << "0. Cancel\n";
    int type = ReadInt("Select Matrix Type: ");

    if (type < 1 || type > 4) {
        return;
    }

    try {
        if (type == 1) {
            int r = ReadInt("Rows: ");
            int c = ReadInt("Cols: ");
            auto* mat = new Matrix<double>(r, c);
            for (int i = 0; i < r; ++i) {
                for (int j = 0; j < c; ++j) {
                    mat->Set(i, j, ((std::rand() % 200) - 100) / 10.0);
                }
            }
            g_MatrixRegistry.Append(new MatrixRecord{"RectangularMatrix", mat});

        } else if (type == 2) {
            int n = ReadInt("Size N: ");
            std::cout << "1. Random Fill\n2. Identity (E)\n3. Hilbert Matrix\nSelect mode: ";
            int mode = ReadInt("> ");
            SquareMatrix<double>* mat = nullptr;
            if (mode == 1) mat = new SquareMatrix<double>(MatrixSolver<double>::GenerateRandomMatrix(n));
            else if (mode == 2) mat = new SquareMatrix<double>(SquareMatrix<double>::Identity(n));
            else if (mode == 3) mat = new SquareMatrix<double>(MatrixSolver<double>::GenerateHilbertMatrix(n));
            else return;
            g_MatrixRegistry.Append(new MatrixRecord{"SquareMatrix", mat});

        } else if (type == 3) {
            int n = ReadInt("Size N: ");
            auto* mat = new DiagonalMatrix<double>(n);
            for (int i = 0; i < n; ++i) {
                mat->Set(i, i, ((std::rand() % 200) - 100) / 10.0);
            }
            g_MatrixRegistry.Append(new MatrixRecord{"DiagonalMatrix", mat});

        } else if (type == 4) {
            int n = ReadInt("Size N: ");
            int isLower = ReadInt("1. Lower Triangular\n2. Upper Triangular\nSelect mode: ");
            auto* mat = new TriangularMatrix<double>(n, isLower == 1);
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j < n; ++j) {
                    if ((isLower == 1 && i >= j) || (isLower != 1 && i <= j)) {
                        mat->Set(i, j, ((std::rand() % 200) - 100) / 10.0);
                    }
                }
            }
            g_MatrixRegistry.Append(new MatrixRecord{isLower == 1 ? "LowerTriangular" : "UpperTriangular", mat});
        }
        std::cout << "Matrix created successfully!\n";
    } catch (const Exception& e) {
        std::cout << "  [EXCEPTION]: " << e.GetMessage() << "\n";
    }
}

// Создание вектора
void CreateVectorMenu() {
    std::cout << "\n--- Create Vector ---\n";
    int n = ReadInt("Enter vector size N: ");
    if (n <= 0) { return; }
    int mode = ReadInt("1. Manual Input\n2. Random Fill\nSelect mode: ");

    try {
        Vector<double>* vec = nullptr;
        if (mode == 1) {
            vec = new Vector<double>(n);
            for (int i = 0; i < n; ++i) {
                std::cout << "b[" << i << "] = ";
                (*vec)[i] = ReadDouble();
            }
        } else {
            vec = new Vector<double>(MatrixSolver<double>::GenerateRandomVector(n));
        }
        g_VectorRegistry.Append(new VectorRecord{"Vector", vec});
        std::cout << "Vector created successfully!\n";
    } catch (const Exception& e) {
        std::cout << "  [EXCEPTION]: " << e.GetMessage() << "\n";
    }
}

// Универсальное математическое меню
void MatrixMathMenu() {
    PrintRegistries();
    int idx = PickMatrix();
    if (idx == -1) return;

    while (true) {
        ClearScreen();
        IMatrix<double>* A = g_MatrixRegistry.Get(idx)->mat;

        std::cout << "\n=== Math Ops for Matrix [" << idx << "] ===\n";
        PrintMatrixCompact(A);
        std::cout << "\n-----------------------------------------\n";
        std::cout << "1. Add (+ another Matrix)\n";
        std::cout << "2. Multiply (* another Square Matrix)\n";
        std::cout << "3. Multiply (* Vector)\n";
        std::cout << "4. Multiply by Scalar\n";
        std::cout << "5. Calculate Trace\n";
        std::cout << "6. Calculate Universal Norm\n";
        std::cout << "0. Back to Main Menu\n";

        int op = ReadInt("> ");
        if (op == 0) {
            break;
        }

        try {
            switch (op) {
                case 1: { // УНИВЕРСАЛЬНОЕ СЛОЖЕНИЕ
                    int idxB = PickMatrix();
                    if (idxB == -1) break;
                    IMatrix<double>* B = g_MatrixRegistry.Get(idxB)->mat;

                    // Умная маршрутизация: Если обе матрицы диагональные, используем их быстрый оператор сложения
                    auto diagA = dynamic_cast<DiagonalMatrix<double>*>(A);
                    auto diagB = dynamic_cast<DiagonalMatrix<double>*>(B);
                    if (diagA && diagB) {
                        auto* result = new DiagonalMatrix<double>(*diagA + *diagB);
                        g_MatrixRegistry.Append(new MatrixRecord{"Diagonal Result (+)", result});
                    } else {
                        // Иначе конвертируем обе в универсальный прямоугольный формат и складываем
                        auto* result = new Matrix<double>(ConvertToGeneralMatrix(A) + ConvertToGeneralMatrix(B));
                        g_MatrixRegistry.Append(new MatrixRecord{"General Result (+)", result});
                    }
                    std::cout << "Sum calculated and added to registry.\n";
                    break;
                }
                case 2: { // УМНОЖЕНИЕ МАТРИЦ (Только Квадратные)
                    int idxB = PickMatrix();
                    if (idxB == -1) break;
                    IMatrix<double>* B = g_MatrixRegistry.Get(idxB)->mat;

                    auto sqA = dynamic_cast<SquareMatrix<double>*>(A);
                    auto sqB = dynamic_cast<SquareMatrix<double>*>(B);
                    if (sqA && sqB) {
                        auto* result = new SquareMatrix<double>((*sqA) * (*sqB));
                        g_MatrixRegistry.Append(new MatrixRecord{"Square Result (*)", result});
                        std::cout << "Product calculated and added to registry.\n";
                    } else {
                        std::cout << "[!] Multiplication is supported only for square matrices (SquareMatrix).\n";
                    }
                    break;
                }
                case 3: { // УМНОЖЕНИЕ НА ВЕКТОР (Только Квадратные)
                    int vIdx = PickVector();
                    if (vIdx == -1) break;
                    auto sqA = dynamic_cast<SquareMatrix<double>*>(A);
                    if (!sqA) {
                        std::cout << "[!] Available only for SquareMatrix.\n";
                        break;
                    }
                    Vector<double>* vecB = g_VectorRegistry.Get(vIdx)->vec;
                    Vector<double>* resVec = new Vector<double>((*sqA) * (*vecB));
                    g_VectorRegistry.Append(new VectorRecord{"Result (A*x)", resVec});
                    std::cout << "Matrix-Vector product added to Vectors registry.\n";
                    break;
                }
                case 4: { // УМНОЖЕНИЕ НА СКАЛЯР
                    double scalar = ReadDouble("Enter scalar: ");
                    auto diagA = dynamic_cast<DiagonalMatrix<double>*>(A);
                    if (diagA) {
                        g_MatrixRegistry.Append(new MatrixRecord{"Scaled Diagonal", new DiagonalMatrix<double>((*diagA) * scalar)});
                    } else {
                        g_MatrixRegistry.Append(new MatrixRecord{"Scaled Matrix", new Matrix<double>(ConvertToGeneralMatrix(A) * scalar)});
                    }
                    std::cout << "Scaled matrix added to registry.\n";
                    break;
                }
                case 5: { // СЛЕД МАТРИЦЫ
                    auto sqA = dynamic_cast<SquareMatrix<double>*>(A);
                    if (sqA) {
                        std::cout << "Trace: " << sqA->Trace() << "\n";
                    } else {
                        std::cout << "[!] Trace calculation is available only for SquareMatrix.\n";
                    }
                    break;
                }
                case 6: { // УНИВЕРСАЛЬНАЯ НОРМА
                    std::cout << "Universal Norm: " << GetUniversalNorm(A) << "\n";
                    break;
                }
                default: {
                    std::cout << "Invalid operation.\n";
                }
            }
        } catch (const Exception& e) {
            std::cout << "  [EXCEPTION]: " << e.GetMessage() << "\n";
        }
        WaitEnter();
    }
}

// Меню решения СЛАУ
void SystemSolverMenu() {
    ClearScreen();
    std::cout << "=== LINEAR SYSTEM SOLVER (Ax = b) ===\n";
    PrintRegistries();

    std::cout << "\nStep 1: Select Matrix [A]\n";
    int aIdx = PickMatrix();
    if (aIdx == -1) return;

    // СТРОГАЯ ПРОВЕРКА ТИПА (Только квадратные матрицы решают СЛАУ)
    auto sqA = dynamic_cast<SquareMatrix<double>*>(g_MatrixRegistry.Get(aIdx)->mat);
    if (!sqA) {
        std::cout << "\n[ERROR] To solve a linear system, matrix [A] MUST be square (SquareMatrix)!\n";
        WaitEnter();
        return;
    }

    std::cout << "Step 2: Select Vector [b]\n";
    int bIdx = PickVector();
    if (bIdx == -1) return;
    Vector<double>* b = g_VectorRegistry.Get(bIdx)->vec;

    std::cout << "\nSelect Solver Algorithm:\n";
    std::cout << "1. Gaussian Elimination (No Pivoting)\n";
    std::cout << "2. Gaussian Elimination (Partial Pivoting)\n";
    std::cout << "3. LU Decomposition & Solve\n";
    std::cout << "0. Cancel\n";

    int algo = ReadInt("> ");
    if (algo == 0) return;

    try {
        Vector<double>* x = nullptr;
        clock_t start = clock();

        if (algo == 1) {
            x = new Vector<double>(MatrixSolver<double>::SolveGaussNoPivot(*sqA, *b));
        } else if (algo == 2) {
            x = new Vector<double>(MatrixSolver<double>::SolveGaussPartialPivot(*sqA, *b));
        } else if (algo == 3) {
            auto lu = MatrixSolver<double>::DecomposeLU(*sqA);
            std::cout << "[i] LU Decomposition successful!\n";
            // Сохраняем промежуточные L и U матрицы в реестр, чтобы их можно было увидеть!
            auto* L_ptr = new TriangularMatrix<double>(lu.first);
            auto* U_ptr = new TriangularMatrix<double>(lu.second);
            g_MatrixRegistry.Append(new MatrixRecord{"L-Matrix (from LU)", L_ptr});
            g_MatrixRegistry.Append(new MatrixRecord{"U-Matrix (from LU)", U_ptr});

            x = new Vector<double>(MatrixSolver<double>::SolveUsingLU(*L_ptr, *U_ptr, *b));
        } else {
            return;
        }
        clock_t end = clock();

        std::cout << "\n--- SOLUTION FOUND ---\n";
        std::cout << "Time taken: " << double(end - start) / CLOCKS_PER_SEC * 1000.0 << " ms\n";
        std::cout << "Residual ||Ax - b||: " << MatrixSolver<double>::CalculateResidual(*sqA, *x, *b) << "\n";

        g_VectorRegistry.Append(new VectorRecord{"Solution 'x'", x});
        std::cout << "\n[!] Solution vector added to registry.\n";

    } catch (const Exception& e) {
        std::cout << "\n  [MATH EXCEPTION]: " << e.GetMessage() << "\n";
    }
    WaitEnter();
}

// Очистка памяти
void CleanupMemory() {
    for (int i = 0; i < g_MatrixRegistry.GetLength(); ++i) {
        MatrixRecord* rec = g_MatrixRegistry.Get(i);
        delete rec->mat; // Виртуальный деструктор спасет от утечек памяти!
        delete rec;
    }
    for (int i = 0; i < g_VectorRegistry.GetLength(); ++i) {
        VectorRecord* rec = g_VectorRegistry.Get(i);
        delete rec->vec;
        delete rec;
    }
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    while (true) {
        ClearScreen();
        PrintRegistries();

        std::cout << "\n=== UNIVERSAL MATRIX CALCULATOR ===\n";
        std::cout << "1. Create Matrix (Any Type)\n";
        std::cout << "2. Create Vector\n";
        std::cout << "3. Universal Math Operations (Context Menu)\n";
        std::cout << "4. Solve Linear System (Ax = b)\n";
        std::cout << "5. Delete Matrix\n";
        std::cout << "6. Delete Vector\n";
        std::cout << "0. Exit Program\n";

        int choice = ReadInt("> ");

        switch (choice) {
            case 1: CreateMatrixMenu(); WaitEnter(); break;
            case 2: CreateVectorMenu(); WaitEnter(); break;
            case 3: MatrixMathMenu(); break;
            case 4: SystemSolverMenu(); break;
            case 5: DeleteMatrix(); WaitEnter(); break;
            case 6: DeleteVector(); WaitEnter(); break;
            case 0:
                CleanupMemory();
                std::cout << "Memory cleared. Goodbye!\n";
                return 0;
            default:
                WaitEnter();
                break;
        }
    }
}