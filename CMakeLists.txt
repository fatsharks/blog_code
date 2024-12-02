cmake_minimum_required(VERSION 2.8.7)

project(kyojinDemo CXX)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

option(test "Build all tests" OFF)

execute_process(COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/bin
                WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# add_compile_options(-lrt)

if(test)
    find_package(gtest REQUIRED)
endif()
find_package(Threads)
find_package(OpenCV 3.0 QUIET)
if(NOT OpenCV_FOUND)
   find_package(OpenCV 2.4.3 QUIET)
   if(NOT OpenCV_FOUND)
      message(FATAL_ERROR "OpenCV > 2.4.3 not found.")
   endif()
endif()

add_executable(kyojin_demo
    src/cxx_11/future_test.cpp
    src/cxx_11/mutex_test.cpp
    src/cxx_11/thread_test.cpp
    src/cxx_11/singleton_test.cpp
    src/cxx_11/shared_ptr_test.cpp
    src/cxx_11/virtual_class_test.cpp
    src/cxx_11/dead_lock_test.cpp
    src/cxx_11/thread_pool_test.cpp
    src/cxx_11/variadic_template_test.cpp

    src/data_structure/queue.cpp
    src/data_structure/list.cpp
    src/data_structure/stack.cpp
    src/data_structure/tree.cpp

    src/algorithm/binary_sort_tree.cpp
    src/algorithm/red_black_tree.cpp

    src/system/ipc_test.cpp

    src/visual/v_slam_entr.cpp

    demo/main.cpp
)

if(test)
    target_link_libraries(g_test g_test_main)
endif()

target_link_libraries(kyojin_demo
    pthread
    rt
    ${OpenCV_LIBS}
)