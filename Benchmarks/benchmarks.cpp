#include <bits/stdc++.h>

#include "bench.h"
#include "SegmentTrees/ParrallelTrees/Version1/tree_v1.h"
#include "SegmentTrees/ParrallelTrees/Version2/tree_v2.h"
#include "SegmentTrees/ParrallelTrees/Version3/tree_v3.h"
#include "SegmentTrees/ParrallelTrees/Version3/tree_v3_modified.h"
#include "SegmentTrees/ParrallelTrees/Version4/tree_v4.h"

#define TreeType ParallelSegmentTree_V3
// #define TreeType Tree

// #define MANY_TESTS
// constexpr const std::array<size_t, 5> threads_count{2, 4, 6, 8, 10};

// constexpr const std::array<size_t, 4> threads_count{2, 3, 4, 5};
#define SINGLE_TEST
constexpr const std::array<size_t, 1> threads_count{2};


#define FAST_DATA
// #define SLOW_DATA
#define MODIFY
// #define STATE

void PrintTable(const std::vector<std::vector<bench::duration>>& results,
                const std::vector<size_t>& sizes,
                const std::string& caption,
                const std::string& first_field_value = "T | N") {
  std::cerr << "\\begin{table}[!htb]\n";
  std::cerr << "\t"
            << "\\caption{" << caption << "}\n";
  std::cerr << "\t" << "\\begin{tabular}{|l|l|l|l|l|l|}\n";
  std::cerr << "\t" << "\\hline\n";
  std::cerr << "\t" << std::setw(9) << first_field_value;
  for (size_t size : sizes) {
    std::cerr << " &" << std::setw(9) << size;
  }
  std::cerr << " \\\\ \\hline\n";
  for (size_t index = 0; index < results.size(); ++index) {
    std::cerr << std::setw(9) << threads_count[index];
    for (auto time : results[index]) {
      std::cerr << " &" << std::setw(9) << time.count();
    }
    std::cerr << " \\\\ \\hline\n";
  }
  std::cerr << "\t" << "\\end{tabular}\n";
  std::cerr << "\t" << "\\label{tab:oth}\n";
  std::cerr << "\\end{table}\n";
}

int main() {

  std::vector<size_t>
      sizes = {size_t(1e6), size_t(2e6), size_t(4e6), size_t(1e7), size_t(2e7)};
#ifdef FAST_DATA
#ifdef MODIFY
  {
    std::cerr << "--MODIFY BENCHMARK:\n";
    std::vector<std::vector<bench::duration>>        results;
    std::cerr << threads_count[0] << " threads\n";
    results.push_back(bench::ModifyOnFastData<TreeType,
                                              threads_count[0]>(sizes));
#ifndef SINGLE_TEST
    std::cerr << threads_count[1] << " threads\n";
    results.push_back(bench::ModifyOnFastData<TreeType,
                                              threads_count[1]>(sizes));
    std::cerr << threads_count[2] << " threads\n";
    results.push_back(bench::ModifyOnFastData<TreeType,
                                              threads_count[2]>(sizes));
    std::cerr << threads_count[3] << " threads\n";
    results.push_back(bench::ModifyOnFastData<TreeType,
                                              threads_count[3]>(sizes));
#ifdef MANY_TESTS
    std::cerr << threads_count[4] << " threads\n";
    results.push_back(bench::ModifyOnFastData<TreeType,
                                              threads_count[4]>(sizes));
#endif
#endif
    PrintTable(results,
               sizes,
               "Среднее время работы ModifyTree в наносекундах",
               "T | N");
  }
#endif

#ifdef STATE
  {
    std::cerr << "--STATE BENCHMARK:\n";
    std::vector<std::vector<bench::duration>> results;
    std::cerr << threads_count[0] << " threads\n";
    results.push_back(bench::GetStateOnFastData<TreeType, threads_count[0]>(
        sizes));
#ifndef SINGLE_TEST
    std::cerr << threads_count[1] << " threads\n";
    results.push_back(bench::GetStateOnFastData<TreeType, threads_count[1]>(
        sizes));
    std::cerr << threads_count[2] << " threads\n";
    results.push_back(bench::GetStateOnFastData<TreeType, threads_count[2]>(
        sizes));
    std::cerr << threads_count[3] << " threads\n";
    results.push_back(bench::GetStateOnFastData<TreeType, threads_count[3]>(
        sizes));
#ifdef MANY_TESTS
    std::cerr << threads_count[4] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[4]>(
        sizes));
#endif
#endif
    PrintTable(results,
               sizes,
               "Среднее время работы GetTreeState в наносекундах",
               "T | N");
  }
#endif
#endif

#ifdef SLOW_DATA
#ifdef MODIFY
  {
    std::cerr << "--MODIFY BENCHMARK (SLOW DATA):\n";
    std::vector<std::vector<bench::duration>> results;
    std::cerr << threads_count[0] << " threads\n";
    results.push_back(bench::ModifyOnSlowData<TreeType,
                                              threads_count[0]>(sizes));
#ifndef SINGLE_TEST
    std::cerr << threads_count[1] << " threads\n";
    results.push_back(bench::ModifyOnSlowData<TreeType,
                                              threads_count[1]>(sizes));
    std::cerr << threads_count[2] << " threads\n";
    results.push_back(bench::ModifyOnSlowData<TreeType,
                                              threads_count[2]>(sizes));
    std::cerr << threads_count[3] << " threads\n";
    results.push_back(bench::ModifyOnSlowData<TreeType,
                                              threads_count[3]>(sizes));
#ifdef MANY_TESTS
    std::cerr << threads_count[4] << " threads\n";
    results.push_back(bench::ModifyOnSlowData<TreeType, threads_count[4]>(sizes));
#endif
#endif
    PrintTable(results,
               sizes,
               "Среднее время работы ModifyTree в наносекундах на медленных данных",
               "T | N");
  }

#endif

#ifdef STATE
  {
    std::cerr << "--STATE BENCHMARK (SLOW DATA):\n";
    std::vector<std::vector<bench::duration>> results;
    std::cerr << threads_count[0] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[0]>(
        sizes));
#ifndef SINGLE_TEST
    std::cerr << threads_count[1] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[1]>(
        sizes));
    std::cerr << threads_count[2] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[2]>(
        sizes));
    std::cerr << threads_count[3] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[3]>(
        sizes));
#ifdef MANY_TESTS
    std::cerr << threads_count[4] << " threads\n";
    results.push_back(bench::GetStateOnSlowData<TreeType, threads_count[4]>(
        sizes));
#endif
#endif
    PrintTable(results,
               sizes,
               "Среднее время работы GetTreeState в наносекундах на медленных данных",
               "T | N");
  }
#endif
#endif
}