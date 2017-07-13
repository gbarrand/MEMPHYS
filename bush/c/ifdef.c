// Copyright (C) 2010, Guy Barrand. All rights reserved.
// See the file bush.license for terms.

/* not include here */

const char* WIN32_defined() {
#ifdef WIN32
  static const char s[] = "#ifdef WIN32";
#else
  static const char s[] = "WIN32 not defined.";
#endif
  return s;
}

const char* _WIN32_defined() {
#ifdef _WIN32
  static const char s[] = "#ifdef _WIN32";
#else
  static const char s[] = "_WIN32 not defined.";
#endif
  return s;
}

const char* _WIN64_defined() {
#ifdef _WIN64
  static const char s[] = "#ifdef _WIN64";
#else
  static const char s[] = "_WIN64 not defined.";
#endif
  return s;
}

const char* __WIN32___defined() {
#ifdef __WIN32__
  static const char s[] = "#ifdef __WIN32__";
#else
  static const char s[] = "__WIN32__ not defined.";
#endif
  return s;
}

const char* _MSC_VER_defined() {
#ifdef _MSC_VER
  static const char s[] = "#ifdef _MSC_VER";
#else
  static const char s[] = "_MSC_VER not defined.";
#endif
  return s;
}

const char* __i386___defined() {
#ifdef __i386__
  static const char s[] = "#ifdef __i386__";
#else
  static const char s[] = "__i386__ not defined.";
#endif
  return s;
}

const char* __x86_64___defined() {
#ifdef __x86_64__
  static const char s[] = "#ifdef __x86_64__";
#else
  static const char s[] = "__x86_64__ not defined.";
#endif
  return s;
}

const char* __ia64___defined() {
#ifdef __ia64__
  static const char s[] = "#ifdef __ia64__";
#else
  static const char s[] = "__ia64__ not defined.";
#endif
  return s;
}

const char* __APPLE___defined() {
#ifdef __APPLE__
  static const char s[] = "#ifdef __APPLE__";
#else
  static const char s[] = "__APPLE__ not defined.";
#endif
  return s;
}

const char* __GNUC___defined() {
#ifdef __GNUC__
  static const char s[] = "#ifdef __GNUC__";
#else
  static const char s[] = "__GNUC__ not defined.";
#endif
  return s;
}

const char* GNU_GCC_defined() {
#ifdef GNU_GCC
  static const char s[] = "#ifdef GNU_GCC";
#else
  static const char s[] = "GNU_GCC not defined.";
#endif
  return s;
}

const char* __clang___defined() {
#ifdef __clang__
  static const char s[] = "#ifdef __clang__";
#else
  static const char s[] = "__clang__ not defined.";
#endif
  return s;
}

const char* __CYGWIN___defined() {
#ifdef __CYGWIN__
  static const char s[] = "#ifdef __CYGWIN__";
#else
  static const char s[] = "__CYGWIN__ not defined.";
#endif
  return s;
}

const char* __linux___defined() {
#ifdef __linux__
  static const char s[] = "#ifdef __linux__";
#else
  static const char s[] = "__linux__ not defined.";
#endif
  return s;
}

const char* __cplusplus_defined() {
#ifdef __cplusplus
  static const char s[] = "#ifdef __cplusplus";
#else
  static const char s[] = "__cplusplus not defined.";
#endif
  return s;
}

const char* MSDOS_defined() {
#ifdef MSDOS
  static const char s[] = "#ifdef MSDOS";
#else
  static const char s[] = "MSDOS not defined.";
#endif
  return s;
}

const char* __unix___defined() {
#ifdef __unix__
  static const char s[] = "#ifdef __unix__";
#else
  static const char s[] = "__unix__ not defined.";
#endif
  return s;
}

const char* __unix_defined() {
#ifdef __unix
  static const char s[] = "#ifdef __unix";
#else
  static const char s[] = "__unix not defined.";
#endif
  return s;
}

const char* unix_defined() {
#ifdef unix
  static const char s[] = "#ifdef unix";
#else
  static const char s[] = "unix not defined.";
#endif
  return s;
}

const char* _LP64_defined() {
#ifdef _LP64
  static const char s[] = "#ifdef _LP64";
#else
  static const char s[] = "_LP64 not defined.";
#endif
  return s;
}

const char* __NT___defined() {
#ifdef __NT__
  static const char s[] = "#ifdef __NT__";
#else
  static const char s[] = "__NT__ not defined.";
#endif
  return s;
}

#include <stdio.h>

int main() {
  printf("%s\n",WIN32_defined());
  printf("%s\n",_WIN32_defined());
  printf("%s\n",__WIN32___defined());
  printf("%s\n",_WIN64_defined());
  printf("%s\n",_MSC_VER_defined());
#ifdef _MSC_VER
  printf("_MSC_VER : %d\n",_MSC_VER);
#endif
  printf("%s\n",__CYGWIN___defined());
  printf("%s\n",MSDOS_defined());
  printf("%s\n",__NT___defined());
  printf("%s\n",__APPLE___defined());
  printf("%s\n",__linux___defined());
  printf("%s\n",__unix___defined());
  printf("%s\n",__unix_defined());
  printf("%s\n",unix_defined());
  printf("%s\n",__GNUC___defined());
  printf("%s\n",GNU_GCC_defined());
  printf("%s\n",__clang___defined());
  printf("%s\n",__cplusplus_defined());
  printf("%s\n",_LP64_defined());
  printf("%s\n",__i386___defined());
  printf("%s\n",__x86_64___defined());
  printf("%s\n",__ia64___defined());
  return 0;
}


