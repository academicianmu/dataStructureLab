cmake_minimum_required(VERSION 3.5)

project(DataStructureLab)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找 Qt 包
find_package(Qt5 REQUIRED COMPONENTS Widgets Core)

# 启用自动处理 UI 文件
set(CMAKE_AUTOUIC ON)

# 启用自动 MOC
set(CMAKE_AUTOMOC ON)

# 启用自动 RCC
set(CMAKE_AUTORCC ON)

# 添加 UI 文件路径
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui)

# 添加源文件
set(SOURCES
    src/main.cpp
    src/main_window.cpp
    src/expression_window.cpp
    src/graph.cpp
    src/josephus_window.cpp
    src/quicksort_window.cpp
    src/linkedlist.cpp
    src/stack.cpp
    src/sequencelist.cpp
    src/edgelist.cpp
    src/doublelinkedlist.cpp
    src/mst_campus_window.cpp
    src/deque.cpp
)

# 添加头文件
set(HEADERS
    src/main_window.h
    src/josephus_window.h
    src/expression_window.h
    src/quicksort_window.h
    src/mst_campus_window.h
    include/linkedlist.h
    include/stack.h
    include/sequencelist.h
    include/edgelist.h
    include/doublelinkedlist.h
    include/deque.h
    include/graph.h
)

# 添加可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接 Qt 库
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core)

# 添加 include 文件夹到头文件搜索路径
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# 设置编译选项
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Windows特定设置
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()