# Создаем заголовочную (INTERFACE) библиотеку
add_library(LAB2 INTERFACE)

# Указываем, что любой, кто подключит LAB2, получит доступ к её хедерам
target_include_directories(LAB2 INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})