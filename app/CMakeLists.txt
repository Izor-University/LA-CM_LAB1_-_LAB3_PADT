# Создаем исполняемый файл приложения
add_executable(LinearAlgebraApp main.cpp)

# Подключаем библиотеку MatrixLib (она определена в папке src)
# Также подключаем LAB2, так как она нужна для работы базовых коллекций
target_link_libraries(LinearAlgebraApp PRIVATE MatrixLib LAB2)