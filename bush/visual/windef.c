/* +---------------------- Copyright notice -------------------------------+ */
/* | Copyright (C) 1995, Guy Barrand, LAL Orsay, (barrand@lal.in2p3.fr)    | */
/* |   Permission to use, copy, modify, and distribute this software       | */
/* |   and its documentation for any purpose and without fee is hereby     | */
/* |   granted, provided that the above copyright notice appear in all     | */
/* |   copies and that both that copyright notice and this permission      | */
/* |   notice appear in supporting documentation.  This software is        | */
/* |   provided "as is" without express or implied warranty.               | */
/* +---------------------- Copyright notice -------------------------------+ */

/*
  This program is a tool to build .def file
 used by lib.exe to build the DLL on Windows.
  It takes the output of a "dumpbin" over an
 archive library and produces the .def file for
 this library. For example :
   DOS> dumpbin /out:tmp /symbols MyLib.arc
   DOS> obuild_windef.exe MyLib < tmp > MyLib.def
 Note that obuild_windef is a standalone program that 
 can be easily reconstructed with :
   DOS> cl /Foobuild_windef.exe obuild_windef.c
*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

char** GetWords (char*,const char*,int*);

int main (int aArgn,char** aArgs) {
#define MAX_STRING 8192
  char buffer[MAX_STRING+1];
  int length;

  /*EXETYPE WINDOWS\n\ */
  printf("\
LIBRARY %s\n\
EXPORTS\n",aArgs[1]);

  while(1){
    if(fgets(buffer,MAX_STRING,stdin)==NULL) return EXIT_FAILURE;
    /*
      On some system (NT) editors when saving binary files 
      put \r\n at place of \n ; we then look for \r\n.
    */
    length = strlen(buffer);
    if( (length>=2) && (buffer[length-2]=='\r') && (buffer[length-1]=='\n') ) {
      buffer[length-2] = '\0';
      length--;
      length--;
    } else if((length>=1) && (buffer[length-1]=='\n')) {
      buffer[length-1] = '\0';
      length--;
    }
    if(strstr(buffer,"SECT")==NULL) continue;
    if(strstr(buffer,"External")==NULL) continue;
    if(strstr(buffer,"??_")!=NULL) {
      if(strstr(buffer,"operator/=")!=NULL) {
        /* Keep operator /= */
    /*} else if(strstr(buffer,"operator[]")!=NULL) {*/
      } else {
        continue;
      }
    }

    {    
      char** words;
      int    wordn;
      words  = GetWords (buffer," ",&wordn);
      if(wordn>=5) {

	if((wordn>=7)&&(strcmp(words[4],"External")==0)) {
          int iword = 6;
          int offset = 0;
#if defined(_MSC_VER) && _MSC_VER <= 1500
          /* On my XP with Visual Studio 9.0, I need the below for C functions starting with an '_'.
             For example to build dll for ourex/Python and ourex/zlib */
	  if(words[iword][0]=='_') offset = 1;
#endif
          // NOTE : with Visual, for global data in a shared lib, for example :
          //   int test_g = 0;
          // a user program must use it with :
          //   __declspec(dllimport) int test_g;
          // (extern int test_g; //does not work).

          if(  (strstr(buffer,": static")!=NULL)
            || (strstr(buffer,"(class")!=NULL)
            ){
            /* static data members are not DATA */
            /* extern objects are not DATA */
            printf(" %s\n",words[iword]+offset);
          } else { /* DATA */
            printf(" %s\tDATA\n",words[iword]+offset);
          }

	} else if((wordn>=8)&&(strcmp(words[4],"()")==0)) { /*code*/
	  int iword = 7;
          int offset = 0;
#if defined(_MSC_VER) && _MSC_VER <= 1500
          /* On my XP with Visual Studio 9.0, I need the below for C functions starting with an '_'.
             For example to build dll for ourex/Python and ourex/zlib */
	  if(words[iword][0]=='_') offset = 1;
#endif
	  printf(" %s\n",words[iword]+offset);
	}

      }
      {int count;
      for(count=0;count<wordn;count++) if(words[count]) free(words[count]);
      if(words) free(words);}
    }
    /*printf("%s\n",buffer);*/
  }

  aArgn = 0;

  return EXIT_SUCCESS;
}

#define STRDUP(str)  ((str) != NULL ? (strcpy((char*)malloc((unsigned)strlen(str) + 1), str)) : (char*)NULL)
#define STRDEL(str) {if((str)!=NULL) {free(str);str=NULL;}}

char** GetWords(char* a_string,const char* a_limiter,int* a_number) {
  int count;
  char*  string;
  char*  token;
  int    iline;
  char** list  = NULL;
  int    nline = 0;

  if(a_number!=NULL) *a_number      = 0;
  if( (a_string==NULL) || (*a_string=='\0') )  return NULL;
  if(a_limiter==NULL) return NULL;

  string = STRDUP(a_string);
  if(string==NULL) return NULL;

  nline = 16;
  list = (char**)malloc(nline*sizeof(char*));
  if(list==NULL) return NULL;
  iline = 0;

  token = string;
  while(1) { 
    char* pos;
    pos = strstr (token,a_limiter);
    if(pos!=NULL) {
      *pos = '\0';
      if(*token!='\0') {
	if(iline>=nline) { 
	  nline    +=16;
	  list      = (char**)realloc(list,nline*sizeof(char*));
	  if(list==NULL) return NULL;
	}
	list[iline]      = token;
	iline++;
      }
      token = pos + strlen(a_limiter);          
    } else { /*last word*/
      if(*token!='\0') {
	if(iline>=nline) {
	  nline    += 16;
	  list      = (char**)realloc(list,nline*sizeof(char*));
	  if(list==NULL) return NULL;
	}
	list[iline]      = token;
	iline++;
      }
      break;
    }
  }
  
  for(count=0;count<iline;count++) list[count] = STRDUP(list[count]);
  STRDEL(string);

  if(iline==0)  {
    if(list) free(list);
    if(a_number!=NULL) *a_number = 0;
    return             NULL;
  } else {
    if(a_number!=NULL) *a_number = iline;
    return             list;
  }
}
