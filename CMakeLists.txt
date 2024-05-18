cmake_minimum_required(VERSION 3.24)
project(Coursework_6sem)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STACK_SIZE 1000000)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(MAIN main.cpp)

add_executable(COMMON_TEST
        Benchmarks/bench.cpp

        SegmentTrees/ClassicTree/classic_tree_tests.cpp
        SegmentTrees/ParrallelTrees/Version0/tree_v0_tests.cpp
        SegmentTrees/ParrallelTrees/Version1/tree_v1_tests.cpp
        SegmentTrees/ParrallelTrees/Version2/tree_v2_tests.cpp
        SegmentTrees/ParrallelTrees/Version3/tree_v3_tests.cpp
        SegmentTrees/ParrallelTrees/Version4/tree_v4_tests.cpp
)

target_link_libraries(COMMON_TEST gtest_main)

add_executable(BENCH
        Benchmarks/bench.cpp
        Benchmarks/benchmarks.cpp
)

add_executable(HELPER
        Utils/array_builder.cpp
)


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
