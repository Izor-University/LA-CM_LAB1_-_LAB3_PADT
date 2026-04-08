template <class T>
Sequence<T>* MatrixSolver<T>::SolveGaussNoPivot(const SquareMatrix<T>& A, const Sequence<T>& b) {
    int n = A.GetRows();
    if (b.GetLength() != n) throw InvalidArgument("Vector dimension mismatch");

    SquareMatrix<T> U = A;
    MutableArraySequence<T> y;
    for (int i = 0; i < n; ++i) y.Append(b.Get(i));

    for (int k = 0; k < n - 1; ++k) {
        if (Abs(U.Get(k, k)) < 1e-9) {
            throw SingularMatrixError("Zero pivot encountered.");
        }
        for (int i = k + 1; i < n; ++i) {
            T factor = U.Get(i, k) / U.Get(k, k);
            for (int j = k; j < n; ++j) {
                U.Set(i, j, U.Get(i, j) - factor * U.Get(k, j));
            }
            y.Set(i, y.Get(i) - factor * y.Get(k));
        }
    }

    Sequence<T>* x = new MutableArraySequence<T>();
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
Sequence<T>* MatrixSolver<T>::SolveGaussPartialPivot(const SquareMatrix<T>& A, const Sequence<T>& b) {
    int n = A.GetRows();
    if (b.GetLength() != n) throw InvalidArgument("Vector dimension mismatch");

    SquareMatrix<T> U = A;
    MutableArraySequence<T> y;
    for (int i = 0; i < n; ++i) y.Append(b.Get(i));

    for (int k = 0; k < n; ++k) {
        int maxRow = k;
        T maxVal = Abs(U.Get(k, k));
        for (int i = k + 1; i < n; ++i) {
            if (Abs(U.Get(i, k)) > maxVal) {
                maxVal = Abs(U.Get(i, k));
                maxRow = i;
            }
        }

        if (maxVal < 1e-9) throw SingularMatrixError("Matrix is singular");

        if (maxRow != k) {
            U.SwapRows(k, maxRow);
            T tempY = y.Get(k);
            y.Set(k, y.Get(maxRow));
            y.Set(maxRow, tempY);
        }

        for (int i = k + 1; i < n; ++i) {
            T factor = U.Get(i, k) / U.Get(k, k);
            for (int j = k; j < n; ++j) {
                U.Set(i, j, U.Get(i, j) - factor * U.Get(k, j));
            }
            y.Set(i, y.Get(i) - factor * y.Get(k));
        }
    }

    Sequence<T>* x = new MutableArraySequence<T>();
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
Pair<TriangularMatrix<T>*, TriangularMatrix<T>*> MatrixSolver<T>::DecomposeLU(const SquareMatrix<T>& A) {
    int n = A.GetRows();

    TriangularMatrix<T>* L = new TriangularMatrix<T>(n, true);
    TriangularMatrix<T>* U = new TriangularMatrix<T>(n, false);

    for (int i = 0; i < n; ++i) L->Set(i, i, T(1));

    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) sum += L->Get(i, k) * U->Get(k, j);
            U->Set(i, j, A.Get(i, j) - sum);
        }

        if (Abs(U->Get(i, i)) < 1e-9) {
            delete L; delete U;
            throw SingularMatrixError("LU fails: zero pivot");
        }

        for (int j = i + 1; j < n; ++j) {
            T sum = T();
            for (int k = 0; k < i; ++k) sum += L->Get(j, k) * U->Get(k, i);
            L->Set(j, i, (A.Get(j, i) - sum) / U->Get(i, i));
        }
    }

    return Pair<TriangularMatrix<T>*, TriangularMatrix<T>*>(L, U);
}

template <class T>
Sequence<T>* MatrixSolver<T>::SolveLower(const TriangularMatrix<T>& L, const Sequence<T>& b) {
    if (!L.IsLower()) throw InvalidArgument("Matrix L must be lower");
    int n = L.GetRows();

    Sequence<T>* y = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) y->Append(T());

    for (int i = 0; i < n; ++i) {
        T sum = T();
        for (int j = 0; j < i; ++j) sum += L.Get(i, j) * y->Get(j);
        y->Set(i, (b.Get(i) - sum) / L.Get(i, i));
    }
    return y;
}

template <class T>
Sequence<T>* MatrixSolver<T>::SolveUpper(const TriangularMatrix<T>& U, const Sequence<T>& y) {
    if (U.IsLower()) throw InvalidArgument("Matrix U must be upper");
    int n = U.GetRows();

    Sequence<T>* x = new MutableArraySequence<T>();
    for (int i = 0; i < n; ++i) x->Append(T());

    for (int i = n - 1; i >= 0; --i) {
        T sum = T();
        for (int j = i + 1; j < n; ++j) sum += U.Get(i, j) * x->Get(j);
        x->Set(i, (y.Get(i) - sum) / U.Get(i, i));
    }
    return x;
}

template <class T>
Sequence<T>* MatrixSolver<T>::SolveUsingLU(const TriangularMatrix<T>& L, const TriangularMatrix<T>& U, const Sequence<T>& b) {
    Sequence<T>* y = SolveLower(L, b);
    Sequence<T>* x = SolveUpper(U, *y);
    delete y;
    return x;
}