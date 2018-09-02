// file_reader.cc

#include "file_reader.h"

#include <cstring>
#include <fstream>

using namespace std;

const string kSpaces = " \n\t";

vector<string> split(const string &str, size_t begin = 0,
                     const string &split_on = kSpaces) {
  size_t end = 0;
  vector<string> result;

  while (true) {
    begin = str.find_first_not_of(split_on, begin);
    if (begin < str.length()) {
      end = str.find_first_of(split_on, begin);
      result.push_back(str.substr(begin, end - begin));
      begin = end;
    } else {
      break;
    }
  }

  return result;
}

const char *const *CharArray::get_array() const { return &my_array[0]; }

int CharArray::size() const { return (int)my_array.size(); }

void CharArray::clear_array() {
  for (char **cur = const_cast<char **>(&my_array[0]);
       cur <= &my_array[my_array.size() - 1]; ++cur) {
    free(*cur);
  }
}

void CharArray::set_array(const vector<string> &v) {
  my_array.resize(v.size());
  for (size_t i = 0; i < v.size(); ++i) {
    char *tmp = (char *)calloc(1, v[i].size() + 1);
    strcpy(tmp, v[i].c_str());
    tmp[v[i].size()] = 0;
    my_array[i] = tmp;
  }
}

string CharArray::dump() const {
  string result;
  for (auto i = my_array.begin(); i != my_array.end(); ++i) {
    result += string(*i);
    if (next(i) != my_array.end())
      result += " ";
  }
  return result;
}

FileStruct::FileStruct() {}

FileStruct::FileStruct(const string &name, const string &opt) {
  fname = name;
  options.set_array(split(opt));
}

string FileStruct::dump() const {
  string result;
  result += "{fname: " + fname + ", " + options.dump() + "}";
  return result;
}

/*
 * each line of the file name file should be of the form:
 * "//" + a comment
 * OR
 * "filename[ compilation options]"
 */

bool is_blank(const string &str) {
  return str.find_first_not_of(kSpaces, 0) == string::npos;
}

bool is_comment(const string &str) {
  if (str.size() >= 2) {
    return str[0] == '/' && str[1] == '/';
  }
  return false;
}

vector<FileStruct> get_FileStructs(const string &fname) {
  vector<FileStruct> result;
  ifstream file(fname);
  string line;
  size_t file_name_boundary[2];

  while (file.good()) {
    getline(file, line);
    if (!is_blank(line) && !is_comment(line)) {
      file_name_boundary[0] = line.find_first_not_of(kSpaces, 0);
      file_name_boundary[1] =
          line.find_first_of(kSpaces, file_name_boundary[0]);
      result.emplace_back(
          line.substr(file_name_boundary[0],
                      file_name_boundary[1] - file_name_boundary[0]),
          line.substr(file_name_boundary[1]));
    }
  }
  return result;
}


