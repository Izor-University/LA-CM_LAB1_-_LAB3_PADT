# Создаем заголовочную библиотеку для матриц
add_library(MatrixLib INTERFACE)

# Экспортируем пути к заголовочным файлам src
target_include_directories(MatrixLib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Говорим CMake: "Чтобы использовать MatrixLib, нужно также подключить LAB2"
target_link_libraries(MatrixLib INTERFACE LAB2)