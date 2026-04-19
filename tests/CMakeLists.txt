# Встроенный модуль CMake для работы с GTest
include(GoogleTest)

# =========================================================================
# 1. СБОРКА ЮНИТ-ТЕСТОВ (ОТДЫЛЬНЫЙ ФАЙЛ)
# =========================================================================
add_executable(UnitTests
        test_vector.cpp
        test_matrix.cpp
        test_square_matrix.cpp
        test_triangular_matrix.cpp
        test_matrix_solver.cpp
)

# Подключаем нашу матричную библиотеку и GTest
target_link_libraries(UnitTests PRIVATE
        MatrixLib
        gtest_main
)

# Регистрируем юнит-тесты в CTest
gtest_discover_tests(UnitTests)


# =========================================================================
# 2. СБОРКА БЕНЧМАРКОВ ЛР-1 (ОТДЫЛЬНЫЙ ФАЙЛ)
# =========================================================================
add_executable(BenchmarkLR1
        benchmark_lr1.cpp
)

# Подключаем матричную библиотеку и GTest
target_link_libraries(BenchmarkLR1 PRIVATE
        MatrixLib
        gtest_main
)

# Регистрируем бенчмарки в CTest (они будут видны как тесты, но запускаются отдельно)
gtest_discover_tests(BenchmarkLR1)