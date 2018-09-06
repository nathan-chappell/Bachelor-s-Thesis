// clanger.h

#include "clang_machine.h"
#include "file_struct.h"
#include "marshaller.h"

#include "clang-c/Index.h"

#include <string>

/*
 * Helper function to keep the FileStructs partitioned from the Clang Library
 */
CXTranslationUnit
getTranslationUnitFromFileStruct(CXIndex index, const FileStruct &fileStruct);

/*
 * Takes all the file structs and analyzes them with the given machine;
 */
void analyze_fileStructs(const std::string &files_for_processing,
                         MachinePair machinePair);
