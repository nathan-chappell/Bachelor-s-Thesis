// clanger.cc

#include "file_struct.h"
#include "marshaller.h"

#include "clang-c/Index.h"

#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Machine {};

CXTranslationUnit
getTranslationUnitFromFileStruct(CXIndex index, const FileStruct &fileStruct) {
  return clang_createTranslationUnitFromSourceFile(
      index, fileStruct.fname.c_str(), fileStruct.options.size(),
      fileStruct.options.get_array(), 0, nullptr);
}

void analyze_TU(CXTranslationUnit TU, Machine& maching) {}

/*
 * Need to figure out what the machine type is going to be
 */
void analyze_files(const string &files_for_processing, Machine machine) {
  vector<FileStruct> fileStructs = get_FileStructs("test_files/files_for_processing");
  CXIndex index = clang_createIndex(0, 0);
  for (auto &&fileStruct : fileStructs) {
    analyze_TU(getTranslationUnitFromFileStruct(index, fileStruct), machine);
  }
}

int main(int argc, char **argv) {
}
