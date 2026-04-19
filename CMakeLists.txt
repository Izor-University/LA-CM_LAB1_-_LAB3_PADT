cmake_minimum_required(VERSION 3.14)
project(MatrixAlgebra CXX)

# Используем современный стандарт C++14 (или выше)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Скачиваем Google Test автоматически (не нужно ставить в систему вручную)
# ---------------------------------------------------------------------------
include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# Флаг для Windows, чтобы избежать конфликтов с Runtime Library
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Включаем систему тестирования CTest
enable_testing()

# ---------------------------------------------------------------------------
# Подключаем наши подпапки
# ---------------------------------------------------------------------------
add_subdirectory(LAB2)
add_subdirectory(src)
add_subdirectory(tests)