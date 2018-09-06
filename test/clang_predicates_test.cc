// clang_predicates_test.cc

#include "clang_predicates.h"
#include "clang_predicates_test_file_stats.h"

#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const string dir = "test_files/clang_testing/";

typedef bool (*cursor_predicate)(CXCursor);

struct test_data {
  cursor_predicate pred;
  int *tally;
  bool display;
};

CXChildVisitResult tally_visitor(CXCursor cursor, CXCursor,
                                 CXClientData client_data) {
  test_data *data = (test_data *)client_data;
  if (data->pred(cursor)) {
    if (data->display) {
      cout << "\tfound: " << get_usr(cursor) << " --\t"
           << get_display_name(cursor) << " --\t"
           << get_start_loc(cursor).second << endl;
    }
    *data->tally += 1;
  }
  return CXChildVisit_Recurse;
}

int tally_predicates(cursor_predicate pred, CXTranslationUnit TU,
                     bool display = false) {
  int result = 0;
  test_data data{pred, &result, display};
  clang_visitChildren(clang_getTranslationUnitCursor(TU), tally_visitor, &data);
  if (data.display)
    cout << "total: " << result << endl;
  return result;
}

void run_tests(const vector<string> &display_files) {
  int failed_tests = 0;
  bool display = false;
  CXIndex index = clang_createIndex(0, 0);
  for (auto &&fileStat : fileStats) {
    if (count(display_files.begin(), display_files.end(), fileStat.first)) {
      display = true;
    } else {
      display = false;
    }
    if (display) {
      cout << "----------------------------------------" << endl;
      cout << "displaying: " << fileStat.first << ", expecting:" << endl;
      for (auto &&predicate_stat : fileStat.second) {
        cout << "\t" << clang_predicate_info(predicate_stat.first) << ": "
             << predicate_stat.second << endl;
      }
      cout << "<---" << endl;
    }
    CXTranslationUnit TU = clang_createTranslationUnitFromSourceFile(
        index, (dir + fileStat.first).c_str(), 0, nullptr, 0, nullptr);
    for (auto &&predicate_stat : fileStat.second) {
      if (display) {
        cout << clang_predicate_info(predicate_stat.first) << endl;
      }
      // run test
      int result = tally_predicates(predicate_stat.first, TU, display);
      // check results
      if (result != predicate_stat.second) {
        ++failed_tests;
        cout << "fail on predicate: " << clang_predicate_info(predicate_stat.first);
        if (!display) {
          cout << ", file: " << fileStat.first << endl;
        } else {
          cout << endl;
        }
      }
      if (display) {
        cout << endl;
      }
    }
  }
  if (failed_tests) {
    cout << "failed: " << failed_tests << " tests" << endl;
  }
}

int main(int argc, char **argv) {
  vector<string> display_files;
  for (int i = 1; i < argc; ++i) {
    display_files.push_back(argv[i]);
  }
  run_tests(display_files);
  cout << "clang_predicates testing complete" << endl;
}
