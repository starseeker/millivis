#include <stdio.h>
#include <string.h>

#include <sys/stat.h>
#ifdef _WIN32
#include <io.h>
#else
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#endif

void tab(int i)
{
  while (i-- > 0)
    putchar(' ');
}

void print_quoted(const char * str)
{
  const unsigned char * s = (const unsigned char *)str;
  while (*s) {
    if (*s < 32) {
      putchar('^');
      putchar('A'+*s);
    }
    else if (*s > 127) {
      putchar('|');
      putchar(-127+'A'+*s);
    }
    else switch (*s) {
    case '&': puts("&amp;"); break;
    case '<': puts("&lt;"); break;
    case '>': puts("&gt;"); break;
    case '"': puts("&quot;"); break;
    case '\'': puts("&apos;"); break;
    default:
      putchar(*s);
    }
    s++;
  }
}

#ifdef _WIN32
void print_dir(const char * name, int depth)
{
  struct _finddata_t f_data;
  long code, handle;
  char dirname[2048];
  char * p;
  struct stat st;

  tab(depth);
  strcpy(dirname, name);
  p = strrchr(dirname, '/');
  if (p == 0)
    p = dirname;
  else
    p++;
  stat(dirname, &st);
  strcat(dirname, "/");
  fputs("<dir name='", stdout);
  print_quoted(p);
  printf("' size='%ld' atime='%ld' mtime='%ld' ctime='%ld'>\n",
	 st.st_size,
	 st.st_atime,
	 st.st_mtime,
	 st.st_ctime);
  strcat(dirname, "*");
  for (handle = code = _findfirst(dirname, &f_data);
       code != -1;
       code = _findnext(handle, &f_data)) {
    if (f_data.name[0] == '.')
      continue;
    if (f_data.attrib & _A_SUBDIR) {
      strcpy(dirname, name);
      strcat(dirname, "/");
      strcat(dirname, f_data.name);
      print_dir(dirname, depth+1);
    }
    else {
      stat(dirname, &st);
      tab(depth+1);
      fputs("<file name='", stdout);
      print_quoted(f_data.name);
      printf("' size='%ld' atime='%ld' mtime='%ld' ctime='%ld'/>\n",
	     f_data.size,
	     st.st_atime,
	     st.st_mtime,
	     st.st_ctime);
    }
  }
  if (handle != -1)
    _findclose(handle);
  tab(depth);
  printf("</dir>\n");
}
#else
void print_dir(const char * name, int depth)
{
  DIR * dir;
  struct dirent * ent;
  char dirname[2048];
  char * p;
  struct stat st;

  tab(depth);
  strcpy(dirname, name);
  p = strrchr(dirname, '/');
  if (p == 0)
    p = dirname;
  else
    p++;
  stat(dirname, &st);
  dir =opendir(dirname);
  strcat(dirname, "/");
  fputs("<dir name='", stdout);
  print_quoted(p);
  printf("' size='%ld' atime='%ld' mtime='%ld' ctime='%ld' uid='%ld' gid='%ld'>\n",
	 st.st_size,
	 st.st_atime,
	 st.st_mtime,
	 st.st_ctime,
	 st.st_uid,
	 st.st_gid);
  if (dir != 0) {
    while ((ent = readdir(dir)) != 0) {
      if (ent->d_name[0] != '.') {
	strcpy(dirname, name);
	strcat(dirname, "/");
	strcat(dirname, ent->d_name);
	if (lstat(dirname, &st) == 0 &&
	    S_ISDIR(st.st_mode))
	  print_dir(dirname, depth+1);
	else {
	  tab(depth+1);
	  fputs("<file name='", stdout);
	  print_quoted(ent->d_name);
	  if ((st.st_mode&(S_IXUSR | S_IXGRP | S_IXOTH)) != 0 &&
	      strchr(ent->d_name, '.') == 0) {
	    fputs(".sh", stdout); /* add dummy executable suffix */
	  }
	  printf("' size='%ld' atime='%ld' mtime='%ld' ctime='%ld'/>\n",
		 st.st_size,
		 st.st_atime,
		 st.st_mtime,
		 st.st_ctime);
	}
      }
    }
    closedir(dir);
  }
  tab(depth);
  printf("</dir>\n");
}
#endif

int main(int argc, char * argv[])
{
  if (argc < 2) {
    fprintf(stderr, "syntax: dirtree <dir>\n");
    return(1);
  }
  printf("<?xml version=\"1.0\" encoding=\"iso-8859-1\"?>\n");
  print_dir(argv[1], 0);
  return 0;
}
