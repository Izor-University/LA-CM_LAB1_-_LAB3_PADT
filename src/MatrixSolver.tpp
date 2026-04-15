template <class T>
Vector<T> MatrixSolver<T>::SolveGaussNoPivot(SquareMatrix<T> U, Vector<T> y) {
    int n = U.GetRows();
    if (y.GetSize() != n) throw InvalidArgument("Mismatch");

    for (int k = 0; k < n - 1; ++k) {
        if (MathUtils::Abs(U(k, k)) < 1e-9) throw SingularMatrixError("Zero pivot");
        for (int i = k + 1; i < n; ++i) {
            T factor = U(i, k) / U(k, k);
            for (int j = k; j < n; ++j) U(i, j) = U(i, j) - factor * U(k, j);
            y[i] = y[i] - factor * y[k];
        }
    }

    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) sum = sum + U(i, j) * x[j];
        x[i] = (y[i] - sum) / U(i, i);
    }
    return x;
}

template <class T>
Vector<T> MatrixSolver<T>::SolveGaussPartialPivot(SquareMatrix<T> U, Vector<T> y) {
    int n = U.GetRows();
    if (y.GetSize() != n) throw InvalidArgument("Mismatch");

    for (int k = 0; k < n; ++k) {
        int maxRow = k;
        double maxVal = MathUtils::Abs(U(k, k));

        for (int i = k + 1; i < n; ++i) {
            double currentAbs = MathUtils::Abs(U(i, k));
            if (currentAbs > maxVal) { maxVal = currentAbs; maxRow = i; }
        }
        if (maxVal < 1e-9) throw SingularMatrixError("Matrix is singular");

        if (maxRow != k) {
            U.SwapRows(k, maxRow);
            T tempY = y[k]; y[k] = y[maxRow]; y[maxRow] = tempY;
        }

        for (int i = k + 1; i < n; ++i) {
            T factor = U(i, k) / U(k, k);
            for (int j = k; j < n; ++j) U(i, j) = U(i, j) - factor * U(k, j);
            y[i] = y[i] - factor * y[k];
        }
    }

    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) sum = sum + U(i, j) * x[j];
        x[i] = (y[i] - sum) / U(i, i);
    }
    return x;
}

template <class T>
Pair<TriangularMatrix<T>, TriangularMatrix<T>> MatrixSolver<T>::DecomposeLU(SquareMatrix<T> A) {
    int n = A.GetRows();
    TriangularMatrix<T> L(n, true);
    TriangularMatrix<T> U(n, false);

    for (int i = 0; i < n; ++i) L.Set(i, i, T(1));

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) sum = sum + L.Get(i, k) * U.Get(k, j);
            U.Set(i, j, A(i, j) - sum);
        }

        if (MathUtils::Abs(U.Get(i, i)) < 1e-9) throw SingularMatrixError("LU fails: zero pivot");

        for (int j = i + 1; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) sum = sum + L.Get(j, k) * U.Get(k, i);
            L.Set(j, i, (A(j, i) - sum) / U.Get(i, i));
        }
    }
    return Pair<TriangularMatrix<T>, TriangularMatrix<T>>(std::move(L), std::move(U));
}

template <class T>
Vector<T> MatrixSolver<T>::SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Vector<T>& b) {
    int n = L.GetRows();
    Vector<T> y(n);
    for (int i = 0; i < n; ++i) {
        T sum = T();
        for (int j = 0; j < i; ++j) sum = sum + L.Get(i, j) * y[j];
        y[i] = (b[i] - sum) / L.Get(i, i);
    }

    Vector<T> x(n);
    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) sum = sum + U.Get(i, j) * x[j];
        x[i] = (y[i] - sum) / U.Get(i, i);
    }
    return x;
}