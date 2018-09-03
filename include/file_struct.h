//file_reader.h

#include <string>
#include <vector>

/**
 * \brief \class char_array is used to get and hold c std::strings from
 * vector<std::string>
 */
class CharArray {
private:
  std::vector<const char *> my_array;

public:
  const char *const *get_array() const;
  int size() const;
  void clear_array();
  void set_array(const std::vector<std::string> &v);
  std::string dump() const;
};

/**
 * \brief \class FileStruct this class is used to make the translation unit
 * getting easier.  Holds a file name and std::vector of compilation flags
 */
struct FileStruct {
  std::string fname;
  CharArray options;

  FileStruct();
  FileStruct(const std::string &name, const std::string &opt);

  std::string dump() const;
};

std::vector<FileStruct> get_FileStructs(const std::string &fname);


