// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#include "os"

#include <cstdio>
#include <cstdlib>

int main(int argc,char** argv) {
  char compiler[128];
  bush::compiler(compiler);
  ::printf("%s-%s-%s\n",bush::os(),bush::processor(),compiler);
  return EXIT_SUCCESS;
}

