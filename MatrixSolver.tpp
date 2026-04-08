template <class T>
ArraySequence<T>* MatrixSolver<T>::SolveGaussNoPivot(const SquareMatrix<T>& A, const Sequence<T>& b) {
    int n = A.GetRows();
    if (b.GetLength() != n) throw std::invalid_argument("Vector b dimension must match matrix A.");

    // Создаем копии для модификации
    SquareMatrix<T> U = A;
    ArraySequence<T> y;
    for (int i = 0; i < n; ++i) y.Append(b.Get(i));

    // Прямой ход
    for (int k = 0; k < n - 1; ++k) {
        if (std::abs(U.Get(k, k)) < 1e-9) {
            throw std::runtime_error("Zero pivot encountered. Matrix might be singular or requires pivoting.");
        }
        for (int i = k + 1; i < n; ++i) {
            T factor = U.Get(i, k) / U.Get(k, k);
            // Зануляем элемент (оптимизация: можно не вызывать полный AddRowMultiple для всей строки)
            for (int j = k; j < n; ++j) {
                U.Set(i, j, U.Get(i, j) - factor * U.Get(k, j));
            }
            y.Set(i, y.Get(i) - factor * y.Get(k));
        }
    }

    // Обратный ход (решение Ux = y)
    ArraySequence<T>* x = new ArraySequence<T>();
    for (int i = 0; i < n; ++i) x->Append(T()); // Инициализируем нулями

    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum += U.Get(i, j) * x->Get(j);
        }
        x->Set(i, (y.Get(i) - sum) / U.Get(i, i));
    }

    return x;
}

template <class T>
ArraySequence<T>* MatrixSolver<T>::SolveGaussPartialPivot(const SquareMatrix<T>& A, const Sequence<T>& b) {
    int n = A.GetRows();
    if (b.GetLength() != n) throw std::invalid_argument("Vector b dimension must match matrix A.");

    SquareMatrix<T> U = A;
    ArraySequence<T> y;
    for (int i = 0; i < n; ++i) y.Append(b.Get(i));

    // Прямой ход с выбором главного элемента
    for (int k = 0; k < n; ++k) {
        // Ищем максимальный элемент в столбце
        int maxRow = k;
        T maxVal = std::abs(U.Get(k, k));
        for (int i = k + 1; i < n; ++i) {
            if (std::abs(U.Get(i, k)) > maxVal) {
                maxVal = std::abs(U.Get(i, k));
                maxRow = i;
            }
        }

        if (maxVal < 1e-9) throw std::runtime_error("Matrix is singular (or very close to).");

        // Переставляем строки в матрице и векторе
        if (maxRow != k) {
            U.SwapRows(k, maxRow);
            T tempY = y.Get(k);
            y.Set(k, y.Get(maxRow));
            y.Set(maxRow, tempY);
        }

        // Исключение
        for (int i = k + 1; i < n; ++i) {
            T factor = U.Get(i, k) / U.Get(k, k);
            for (int j = k; j < n; ++j) {
                U.Set(i, j, U.Get(i, j) - factor * U.Get(k, j));
            }
            y.Set(i, y.Get(i) - factor * y.Get(k));
        }
    }

    // Обратный ход
    ArraySequence<T>* x = new ArraySequence<T>();
    for (int i = 0; i < n; ++i) x->Append(T());

    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum += U.Get(i, j) * x->Get(j);
        }
        x->Set(i, (y.Get(i) - sum) / U.Get(i, i));
    }

    return x;
}

template <class T>
std::pair<TriangularMatrix<T>*, TriangularMatrix<T>*> MatrixSolver<T>::DecomposeLU(const SquareMatrix<T>& A) {
    int n = A.GetRows();
    
    // L - нижняя (с единицами на диагонали), U - верхняя
    TriangularMatrix<T>* L = new TriangularMatrix<T>(n, true);
    TriangularMatrix<T>* U = new TriangularMatrix<T>(n, false);

    // Инициализация диагонали L единицами (T(1) - требует, чтобы конструктор T принимал число 1, 
    // для double/float это работает по умолчанию)
    for (int i = 0; i < n; ++i) {
        L->Set(i, i, T(1));
    }

    for (int i = 0; i < n; ++i) {
        // Вычисляем строку i матрицы U
        for (int j = i; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) {
                sum += L->Get(i, k) * U->Get(k, j);
            }
            U->Set(i, j, A.Get(i, j) - sum);
        }

        if (std::abs(U->Get(i, i)) < 1e-9) {
            delete L; delete U;
            throw std::runtime_error("LU decomposition fails: zero pivot encountered (pivoting not supported in this version).");
        }

        // Вычисляем столбец i матрицы L
        for (int j = i + 1; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) {
                sum += L->Get(j, k) * U->Get(k, i);
            }
            L->Set(j, i, (A.Get(j, i) - sum) / U->Get(i, i));
        }
    }

    return {L, U};
}

template <class T>
ArraySequence<T>* MatrixSolver<T>::SolveLower(const TriangularMatrix<T>& L, const Sequence<T>& b) {
    if (!L.IsLower()) throw std::invalid_argument("Matrix L must be lower triangular.");
    int n = L.GetRows();
    if (b.GetLength() != n) throw std::invalid_argument("Vector b dimension mismatch.");

    ArraySequence<T>* y = new ArraySequence<T>();
    for (int i = 0; i < n; ++i) y->Append(T());

    // Прямая подстановка
    for (int i = 0; i < n; ++i) {
        T sum = T();
        for (int j = 0; j < i; ++j) {
            sum += L.Get(i, j) * y->Get(j);
        }
        y->Set(i, (b.Get(i) - sum) / L.Get(i, i));
    }
    return y;
}

template <class T>
ArraySequence<T>* MatrixSolver<T>::SolveUpper(const TriangularMatrix<T>& U, const Sequence<T>& y) {
    if (U.IsLower()) throw std::invalid_argument("Matrix U must be upper triangular.");
    int n = U.GetRows();
    if (y.GetLength() != n) throw std::invalid_argument("Vector y dimension mismatch.");

    ArraySequence<T>* x = new ArraySequence<T>();
    for (int i = 0; i < n; ++i) x->Append(T());

    // Обратная подстановка
    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum += U.Get(i, j) * x->Get(j);
        }
        x->Set(i, (y.Get(i) - sum) / U.Get(i, i));
    }
    return x;
}

template <class T>
ArraySequence<T>* MatrixSolver<T>::SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Sequence<T>& b) {
    // Шаг 1: Ly = b
    ArraySequence<T>* y = SolveLower(L, b);
    // Шаг 2: Ux = y
    ArraySequence<T>* x = SolveUpper(U, *y);
    
    delete y; // Чистим промежуточный вектор
    return x;
}