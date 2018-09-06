// clang_predicates.cc

#include "clang_predicates.h"

#include <map>

using namespace std;

string cx_string_handler(CXCursor cursor,
                         CXString (*cx_string_getter)(CXCursor)) {
  CXString cx = cx_string_getter(cursor);
  string result = clang_getCString(cx);
  clang_disposeString(cx);
  return result;
}

string get_usr(CXCursor cursor) {
  return cx_string_handler(cursor, clang_getCursorUSR);
}

string get_display_name(CXCursor cursor) {
  return cx_string_handler(cursor, clang_getCursorDisplayName);
}

pair<string, string> get_start_loc(CXCursor cursor) {
  string filename;
  string position;
  CXFile file;
  unsigned line;
  unsigned col;
  unsigned off;
  CXString cx_file;
  CXSourceLocation loc = clang_getCursorLocation(cursor);

  clang_getSpellingLocation(loc, &file, &line, &col, &off);
  cx_file = clang_getFileName(file);
  filename = string(clang_getCString(cx_file));
  position = ::to_string(line) + ":" + ::to_string(col);
  clang_disposeString(cx_file);
  return make_pair(move(filename), move(position));
}

bool is_clang_function_defintion(CXCursor cursor) {
  return cursor.kind == CXCursor_FunctionDecl &&
         clang_isCursorDefinition(cursor);
}

bool is_clang_function_template(CXCursor cursor) {
  return cursor.kind == CXCursor_FunctionTemplate;
}

bool is_clang_function_call(CXCursor cursor) {
  return cursor.kind == CXCursor_CallExpr;
}

bool is_clang_method(CXCursor cursor) {
  return cursor.kind == CXCursor_CXXMethod;
}

bool is_clang_method_definition(CXCursor cursor) {
  return cursor.kind == CXCursor_CXXMethod && clang_isCursorDefinition(cursor);
}

bool is_clang_special_member_fn(CXCursor cursor) {
  return cursor.kind == CXCursor_Constructor ||
         cursor.kind == CXCursor_Destructor ||
         cursor.kind == CXCursor_ConversionFunction;
}

map<bool(*)(CXCursor), string> clang_predicate_info_map{
    {is_clang_function_defintion, "is_clang_function_defintion"},
    {is_clang_function_template, "is_clang_function_template"},
    {is_clang_function_call, "is_clang_function_call"},
    {is_clang_method, "is_clang_method"},
    {is_clang_method_definition, "is_clang_method_definition"},
    {is_clang_special_member_fn, "is_clang_special_member_fn"}};

string clang_predicate_info(bool(*pred)(CXCursor)) {
  auto it = clang_predicate_info_map.find(pred);
  if (it != clang_predicate_info_map.end()) {
    return it->second;
  } else {
    return "no info";
  }
}
