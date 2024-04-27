cmake_minimum_required(VERSION 3.24)
project(Coursework_6sem)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

SET(ASSIGN_AND_SUM Strategies/abstract_mass_strategy.h Strategies/AssignAndSum/assign_sum_strategy.h)

add_executable(MAIN main.cpp)
#add_executable(TEST_CLASSIC_SEGMENT_TREE
#        ${ASSIGN_AND_SUM}
#        Tests/classic_tree_tests.cpp
#        SegmentTree/classic_segment_tree.h
#        SegmentTree/assign_sum_tree.h)
#add_executable(TEST_TREE_V1
#        ${ASSIGN_AND_SUM}
#        Tests/parallel_tree_v1_tests.cpp)

add_executable(COMMON_TEST
        ${ASSIGN_AND_SUM}

        Tests/classic_tree_tests.cpp
        SegmentTree/classic_segment_tree.h
        SegmentTree/assign_sum_tree.h

        Tests/parallel_tree_v1_tests.cpp
)

#target_link_libraries(TEST_CLASSIC_SEGMENT_TREE gtest_main)
#target_link_libraries(TEST_TREE_V1 gtest_main)
target_link_libraries(COMMON_TEST gtest_main)

add_executable(BENCH
        ${ASSIGN_AND_SUM}

        Benchmarks/benchmarks.cpp
)


#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
#set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS} -fno-omit-frame-pointer -fsanitize=address")
