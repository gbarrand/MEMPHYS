
#include <inlib/args>

#include <inlib/file>
#include <inlib/mnmx>
#include <inlib/sjust>

#include <cstdlib>
#include <iostream>

int main(int argc,char* argv[]) {

  inlib::args args(argc,argv);

  std::vector<std::string> txt;
  if(!inlib::file::read("tmp_0",txt)) {
    std::cout << "main :"
              << " can't read file."
              << std::endl;
    return EXIT_FAILURE;
  }

  unsigned int sz = 0;
 {std::vector<std::string>::iterator it;
  for(it=txt.begin();it!=txt.end();++it) {
    std::vector<std::string> words;
    inlib::words(*it," ",false,words);
    sz = inlib::mx<unsigned int>(sz,words[2].size());
  }}

  std::vector<std::string> ntxt;
  ntxt.push_back("#ifndef ourex_png_png_h");
  ntxt.push_back("#define ourex_png_png_h");
  ntxt.push_back("");

  std::vector<std::string>::iterator it;
  for(it=txt.begin();it!=txt.end();++it) {
    std::vector<std::string> words;
    inlib::words(*it," ",false,words);
    std::string symb = words[2].substr(1,words[2].size()-1);
    std::string s("#define ");    
    inlib::justify(symb,sz);
    s += symb;
    s += " ourex_"+symb;
    ntxt.push_back(s);
  }

  ntxt.push_back("");
  ntxt.push_back("#include \"png.h\"");
  ntxt.push_back("");
  ntxt.push_back("#endif");

  if(!inlib::file::write("tmp_1",ntxt)) {
    std::cout << "main :"
              << " can't write file."
              << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
