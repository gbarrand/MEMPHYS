// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

#include "os"

#include <cstdio>
#include <cstdlib>

int main(int argc,char** argv) {
  ::printf("%s-%s-%s\n",bush::os(),bush::processor(),bush::compiler());
  return EXIT_SUCCESS;
}

