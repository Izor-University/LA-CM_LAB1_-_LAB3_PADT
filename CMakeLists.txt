cmake_minimum_required(VERSION 3.14)
project(MatrixAlgebra CXX)

# Используем современный стандарт C++14
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Автоматическая загрузка Google Test
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

# ---------------------------------------------------------------------------
# Подключение модулей проекта
# ---------------------------------------------------------------------------

# 1. Сначала подключаем базовые зависимости и саму библиотеку матриц
add_subdirectory(LAB2)
add_subdirectory(src)

# 2. Подключаем пользовательский интерфейс (Консольное приложение)
add_subdirectory(app)

# 3. Подключаем юнит-тесты и бенчмарки
add_subdirectory(tests)