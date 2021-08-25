
// must be compiled with -std=c++17 (/std:c++17 if using cl.exe).

#include <filesystem>

int main() {
 std::string pattern;
 std::filesystem::path filePattern = pattern.c_str();
 return 0;
}
