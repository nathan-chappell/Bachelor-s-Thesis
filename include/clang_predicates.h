//clang_predicates.h

#include "clang-c/Index.h"

#include <string>

std::string cx_string_handler(CXCursor cursor,
                         CXString (*cx_string_getter)(CXCursor));

std::string get_usr(CXCursor cursor);
std::string get_display_name(CXCursor cursor);
std::pair<std::string,std::string> get_start_loc(CXCursor cursor); //<filename, position>

bool is_clang_function_defintion(CXCursor cursor);
bool is_clang_function_template(CXCursor cursor);
bool is_clang_function_call(CXCursor cursor);
bool is_clang_method(CXCursor cursor);
bool is_clang_method_definition(CXCursor cursor);
bool is_clang_special_member_fn(CXCursor cursor);

std::string clang_predicate_info(bool(*)(CXCursor));
