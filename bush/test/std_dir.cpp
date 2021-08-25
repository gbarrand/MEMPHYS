// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file inlib.license for terms.

//inlib_build_cppflags -std=c++17

// cygwin> cl.exe /std:c++17 /EHsc std_dir.cpp 
// cygwin> ./std_dir.exe

#ifdef INLIB_MEM
#include <inlib/mem>
#endif

#include <filesystem>
#include <regex>

//#include <inlib/args>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

#ifdef INLIB_MEM
  inlib::mem::set_check_by_class(true);{
#endif

  ////////////////////////////////////////////////////////////////////////////////
  /// args : /////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  //inlib::args args(argc,argv);
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////

  std::string pattern("*");

  std::filesystem::path filePattern(pattern);

  // Default pattern : *.g4view
  // Translated to a regexp : ^.*\\.g4view
  // Convert pattern into a regexp
  std::string regexp_pattern("^" + filePattern.filename().string());
  std::string result_pattern = "";
  // Replace '.' by "\\."
  size_t currentPos = 0;
  size_t nextPos = 0;
  std::string currentReplacement = "";
  size_t pos1 = regexp_pattern.find('.', nextPos);
  size_t pos2 = regexp_pattern.find('*', nextPos);
  size_t pos3 = regexp_pattern.find('?', nextPos);
  while ((pos1 != std::string::npos) || (pos2 != std::string::npos) || (pos3 != std::string::npos)) {
    nextPos = pos1;
    currentReplacement = "\\.";
    if (pos2 < nextPos) {
      nextPos = pos2;
      currentReplacement = ".*";
    }
    if (pos3 < nextPos) {
      nextPos = pos3;
      currentReplacement = "(.{1,1})";
    }
    result_pattern += regexp_pattern.substr(currentPos, nextPos - currentPos) + currentReplacement;
    nextPos++;
    currentPos = nextPos;
    pos1 = regexp_pattern.find('.', currentPos);
    pos2 = regexp_pattern.find('*', currentPos);
    pos3 = regexp_pattern.find('?', currentPos);
  }
  result_pattern += regexp_pattern.substr(currentPos);

  // Build view vector of way points
  // Add "./" for empty paths
  std::string parentPath(filePattern.parent_path().string().length() ? filePattern.parent_path().string() : std::string("./"));

  const int safety = 9999;
  int safetyCount = 0;

  // Iterate through files in directory and apply regex match to filter appropriate files
  std::regex result_pattern_regex(result_pattern, std::regex_constants::basic | std::regex_constants::icase);
  for (auto iter = std::filesystem::directory_iterator(parentPath);
       iter != std::filesystem::directory_iterator() && safetyCount++ < safety;
       ++iter)
  {
    const auto& file = iter->path();

    std::string filename(file.filename().string());
    if (std::regex_match(filename, result_pattern_regex))
    {
      std::cout << filename << std::endl;
    }
  }

#ifdef INLIB_MEM
  }inlib::mem::balance(std::cout);
#endif

  return EXIT_SUCCESS;
}
