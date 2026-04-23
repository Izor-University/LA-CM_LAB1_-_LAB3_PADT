// Алгоритмы решения СЛАУ
template <class T>
Vector<T> MatrixSolver<T>::SolveGaussNoPivot(SquareMatrix<T> A, Vector<T> y) {
    int n = A.GetRows();
    if (y.GetSize() != n) {
        throw InvalidArgument("Mismatch");
    }

    // Прямой ход
    for (int k = 0; k < n - 1; ++k) {
        if (MathUtils::Abs(A(k, k)) < 1e-9) {
            throw SingularMatrixError("Zero pivot");
        }
        for (int i = k + 1; i < n; ++i) {
            T factor = A(i, k) / A(k, k);
            for (int j = k; j < n; ++j) {
                A(i, j) = A(i, j) - factor * A(k, j);
            }
            y[i] = y[i] - factor * y[k];
        }
    }

    // Обратный ход
    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        if (MathUtils::Abs(A(i, i)) < 1e-9) {
            throw SingularMatrixError("Matrix is singular");
        }
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum = sum + A(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / A(i, i);
    }
    return x;
}

template <class T>
Vector<T> MatrixSolver<T>::SolveGaussPartialPivot(SquareMatrix<T> A, Vector<T> y) {
    int n = A.GetRows();
    if (y.GetSize() != n) {
        throw InvalidArgument("Mismatch");
    }

    // Прямой ход Гаусса с выбором главного элемента
    for (int k = 0; k < n; ++k) {
        int maxRow = k;
        double maxVal = MathUtils::Abs(A(k, k));

        for (int i = k + 1; i < n; ++i) {
            double currentAbs = MathUtils::Abs(A(i, k));
            if (currentAbs > maxVal) {
                maxVal = currentAbs;
                maxRow = i;
            }
        }

        if (maxVal < 1e-9) {
            throw SingularMatrixError("Matrix is singular");
        }

        // Перестановка строк
        if (maxRow != k) {
            A.SwapRows(k, maxRow);
            T tempY = y[k];
            y[k] = y[maxRow];
            y[maxRow] = tempY;
        }

        for (int i = k + 1; i < n; ++i) {
            T factor = A(i, k) / A(k, k);
            for (int j = k; j < n; ++j) {
                A(i, j) = A(i, j) - factor * A(k, j);
            }
            y[i] = y[i] - factor * y[k];
        }
    }

    // Обратный ход
    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        if (MathUtils::Abs(A(i, i)) < 1e-9) {
            throw SingularMatrixError("Matrix is singular");
        }
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum = sum + A(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / A(i, i);
    }
    return x;
}

// Алгоритмы декомпозиции
template <class T>
Pair<TriangularMatrix<T>, TriangularMatrix<T>> MatrixSolver<T>::DecomposeLU(SquareMatrix<T> A) {
    int n = A.GetRows();
    TriangularMatrix<T> L(n, true);
    TriangularMatrix<T> U(n, false);

    // Инициализация диагонали L единицами
    for (int i = 0; i < n; ++i) {
        L.Set(i, i, static_cast<T>(1));
    }

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) {
                sum = sum + L.Get(i, k) * U.Get(k, j);
            }
            U.Set(i, j, A(i, j) - sum);
        }

        if (MathUtils::Abs(U.Get(i, i)) < 1e-9) {
            throw SingularMatrixError("LU fails: zero pivot");
        }

        for (int j = i + 1; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) {
                sum = sum + L.Get(j, k) * U.Get(k, i);
            }
            L.Set(j, i, (A(j, i) - sum) / U.Get(i, i));
        }
    }

    return Pair<TriangularMatrix<T>, TriangularMatrix<T>>(std::move(L), std::move(U));
}

template <class T>
Vector<T> MatrixSolver<T>::SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Vector<T>& b) {
    int n = L.GetRows();

    // Прямой ход (Ly = b)
    Vector<T> y(n);
    for (int i = 0; i < n; ++i) {
        T sum = T();
        for (int j = 0; j < i; ++j) {
            sum = sum + L.Get(i, j) * y[j];
        }
        y[i] = (b[i] - sum) / L.Get(i, i);
    }

    // Обратный ход (Ux = y)
    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) {
            sum = sum + U.Get(i, j) * x[j];
        }
        x[i] = (y[i] - sum) / U.Get(i, i);
    }
    return x;
}

// Генерация тестовых данных
template <class T>
SquareMatrix<T> MatrixSolver<T>::GenerateRandomMatrix(int n, unsigned int seed) {
    SquareMatrix<T> A(n);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            A(i, j) = static_cast<T>(dist(gen));
        }
    }
    return A;
}

template <class T>
Vector<T> MatrixSolver<T>::GenerateRandomVector(int n, unsigned int seed) {
    Vector<T> b(n);
    std::mt19937 gen(seed);
    std::uniform_real_distribution<double> dist(-1.0, 1.0);

    for (int i = 0; i < n; ++i) {
        b[i] = static_cast<T>(dist(gen));
    }
    return b;
}

template <class T>
SquareMatrix<T> MatrixSolver<T>::GenerateHilbertMatrix(int n) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }
    SquareMatrix<T> H(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            H(i, j) = static_cast<T>(1.0 / (i + j + 1));
        }
    }
    return H;
}

template <class T>
Vector<T> MatrixSolver<T>::GenerateOnesVector(int n) {
    if (n <= 0) {
        throw IndexOutOfRange("Size must be positive");
    }
    return Vector<T>(n, static_cast<T>(1.0));
}

// Метрики качества
template <class T>
double MatrixSolver<T>::CalculateResidual(const SquareMatrix<T>& A, const Vector<T>& x_approx, const Vector<T>& b) {
    Vector<T> residual_vector = (A * x_approx) - b;
    return residual_vector.Norm();
}

template <class T>
double MatrixSolver<T>::CalculateRelativeError(const Vector<T>& x_exact, const Vector<T>& x_approx) {
    Vector<T> diff = x_approx - x_exact;
    double exact_norm = x_exact.Norm();

    if (exact_norm < 1e-9) {
        throw MathError("Exact norm is zero");
    }
    return diff.Norm() / exact_norm;
}