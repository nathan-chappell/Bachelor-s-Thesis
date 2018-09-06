// clanger.cc

#include "clanger.h"

#include <iostream>
#include <vector>

using namespace std;

CXTranslationUnit
getTranslationUnitFromFileStruct(CXIndex index, const FileStruct &fileStruct) {
  return clang_createTranslationUnitFromSourceFile(
      index, fileStruct.fname.c_str(), fileStruct.options.size(),
      fileStruct.options.get_array(), 0, nullptr);
}

void analyze_TU(CXTranslationUnit TU, MachinePair machinePair) {
  cout << "entering: " << __func__ << endl;
  clang_visitChildren(clang_getTranslationUnitCursor(TU), machinePair.first,
                      machinePair.second);
  cout << "exiting: " << __func__ << endl;
}

/*
 * Need to figure out what the machine type is going to be
 */
void analyze_fileStructs(const string &files_for_processing,
                         MachinePair machinePair) {
  vector<FileStruct> fileStructs = get_FileStructs(files_for_processing);
  cout << "files: " << fileStructs.size() << endl;
  CXIndex index = clang_createIndex(0, 0);
  for (auto &&fileStruct : fileStructs) {
    cout << "analyzing: " << fileStruct.fname << " "
         << fileStruct.options.dump() << endl;
    analyze_TU(getTranslationUnitFromFileStruct(index, fileStruct),
               machinePair);
  }
}

int main(int argc, char **argv) {
  Clang_Call_Grapher machine;
  MachinePair machinePair{call_graph_visitor, &machine};
  cout << "analyzing files" << endl;
  //analyze_fileStructs("test_files/files_for_processing", machinePair);
  analyze_fileStructs("test_file_for_processing", machinePair);
  cout << machine.marshaller.dump() << endl;
}
