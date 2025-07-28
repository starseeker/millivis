#include <expat.h>
#include <zlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

static void
printTime(const char * ts)
{
  time_t t = atol(ts);
  char buffer[1024];
  struct tm * tm = localtime(&t);
  strftime(buffer, 1024, "%d %b %Y %H:%M:%S", tm);
  fputs(buffer, stdout);
}

static void
startElement(void *userData,
	     const char *qname, const char **atts)
{
  const char * name = "";
  const char * length = "";
  const char * atime = "";
  const char * mtime = "";
  const char * ctime = "";
  const char * chr;

  if (strcmp(qname, "dir") == 0 ||
      strcmp(qname, "file") == 0) {
    const char ** a;
    for (a = atts; *a != 0; a += 2) {
      if (strcmp(a[0], "name") == 0) {
	name = a[1];
      }
      else if (strcmp(a[0], "length") == 0) {
	length = a[1];
      }
      else if (strcmp(a[0], "atime") == 0) {
	atime = a[1];
      }
      else if (strcmp(a[0], "mtime") == 0) {
	mtime = a[1];
      }
      else if (strcmp(a[0], "ctime") == 0) {
	ctime = a[1];
      }
    }
  }
  chr = name;
  if ((chr = strchr(chr, ';')) != NULL) {
    fwrite(name, 1, chr - name, stdout);
  }
  else
    fputs(name, stdout);
  putchar(';');
  fputs(length, stdout);
  putchar(';');
  printTime(atime);
  putchar(';');
  printTime(mtime);
  putchar(';');
  printTime(ctime);
  putchar('\n');
}

static void endElement(void *userData, const char *name)
{
}



int main(int argc, char * argv[])
{
    char buf[1024];
    gzFile input;
    XML_Parser parser;
    int done = 0;

    if (argc < 2) {
      fprintf(stderr, "syntax: %s filename\n", argv[0]);
      exit(1);
    }

    input = gzopen(argv[1], "rb");
    if (input == NULL) {
      fprintf(stderr, "cannot open %s\n", argv[0]);
      exit(1);
    }

    parser = XML_ParserCreate(NULL);
    XML_SetElementHandler(parser, startElement, endElement);
    do {
      size_t len = gzread(input, buf, sizeof(buf));
      done = len < sizeof(buf);
      if (!XML_Parse(parser, buf, len, done)) {
	fprintf(stderr,
		"%s at line %d\n",
		XML_ErrorString(XML_GetErrorCode(parser)),
		XML_GetCurrentLineNumber(parser));
	done = 1;
      }
    } while (!done);
    gzclose(input);
    XML_ParserFree(parser);
    return 0;
}
