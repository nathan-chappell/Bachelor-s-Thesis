// clang_predicates_test_file_stats.h
#pragma once

#include "clang_predicates.h"

#include "clang-c/Index.h"

#include <map>
#include <string>
#include <vector>

using FileStat = std::pair<std::string, std::map<bool (*)(CXCursor), int>>;

FileStat test0 = {"test0.cc",
                  {{is_clang_function_defintion, 2},
                   {is_clang_function_template, 0},
                   {is_clang_function_call, 1},
                   {is_clang_method, 0},
                   {is_clang_method_definition, 0},
                   {is_clang_special_member_fn, 0}}};

FileStat test1 = {"test1.cc",
                  {{is_clang_function_defintion, 5},
                   {is_clang_function_template, 0},
                   {is_clang_function_call, 4},
                   {is_clang_method, 0},
                   {is_clang_method_definition, 0},
                   {is_clang_special_member_fn, 0}}};

FileStat test2 = {"test2.cc",
                  {{is_clang_function_defintion, 5},
                   {is_clang_function_template, 0},
                   {is_clang_function_call, 6},
                   {is_clang_method, 0},
                   {is_clang_method_definition, 0},
                   {is_clang_special_member_fn, 0}}};

FileStat test3 = {"test3.cc",
                  {{is_clang_function_defintion, 1},
                   {is_clang_function_template, 0},
                   {is_clang_function_call, 6},
                   {is_clang_method, 4},
                   {is_clang_method_definition, 3},
                   {is_clang_special_member_fn, 0}}};

FileStat test4 = {"test4.cc",
                  {{is_clang_function_defintion, 2},
                   {is_clang_function_template, 1},
                   {is_clang_function_call, 5},
                   {is_clang_method, 0},
                   {is_clang_method_definition, 0},
                   {is_clang_special_member_fn, 1}}};

std::vector<FileStat> fileStats = {test0, test1, test2, test3, test4};
