// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#include "os"

#include <iostream>
#include <cstdlib>

int main(int argc,char** argv) {
  std::cout << bush::processor() << std::endl;
  return EXIT_SUCCESS;
}

