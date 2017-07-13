/* This is simple demonstration of how to use expat. This program
reads an XML document from standard input and writes a line with the
name of each element to standard output indenting child elements by
one tab stop more than their parent element. */

#include <stdio.h>
#include "xmlparse.h"

void startElement(void *userData, const char *name, const char **atts)
{
  int i;
  int *depthPtr = userData;
  for (i = 0; i < *depthPtr; i++)
    putchar('\t');
  puts(name);
  *depthPtr += 1;
}

void endElement(void *userData, const char *name)
{
  int *depthPtr = userData;
  *depthPtr -= 1;
}

void characterDataHandler(
 void* aTag
,const XML_Char* aString
,int aLength
)
{
#define bool int
#define true 1
#define false 0
  bool empty = true;
  bool some = false;
  int i;
  char* p = (char*)aString;
  putchar('|');
  for (i = 0; i < aLength; i++, p++) {
    putchar(*p);
  }
  putchar('|');
  putchar('\n');
  /*
  p = (char*)aString;
  for (i = 0; i < aLength; i++, p++) {
    if(!iscntrl(*p) && (*p!=' ')) {
      empty = false;
      break;
    }
  }
  if(empty) return;
  p = (char*)aString;
  for (i = 0; i < aLength; i++, p++) {
    //printf("%d\n",*p);
    if(!iscntrl(*p)) {
      some = true;
      putchar(*p);
    }
  }
  if(some) putchar('\n');
  */
}

int main()
{
  char buf[BUFSIZ];
  XML_Parser parser = XML_ParserCreate(NULL);
  int done;
  int depth = 0;
  XML_SetUserData(parser, &depth);
  XML_SetElementHandler(parser, startElement, endElement);
  XML_SetCharacterDataHandler(parser,characterDataHandler);
  do {
    size_t len = fread(buf, 1, sizeof(buf), stdin);
    done = len < sizeof(buf);
    if (!XML_Parse(parser, buf, len, done)) {
      fprintf(stderr,
	      "%s at line %d\n",
	      XML_ErrorString(XML_GetErrorCode(parser)),
	      XML_GetCurrentLineNumber(parser));
      return 1;
    }
  } while (!done);
  XML_ParserFree(parser);
  return 0;
}
