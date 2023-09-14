#include "types.h"
#include "stat.h"
#include "fcntl.h"
#include "user.h"
#include "x86.h"

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}

int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

uint
strlen(const char *s)
{
  int n;

  for(n = 0; s[n]; n++)
    ;
  return n;
}

void*
memset(void *dst, int c, uint n)
{
  stosb(dst, c, n);
  return dst;
}

char*
strchr(const char *s, char c)
{
  for(; *s; s++)
    if(*s == c)
      return (char*)s;
  return 0;
}

char*
gets(char *buf, int max)
{
  int i, cc;
  char c;

  for(i=0; i+1 < max; ){
    cc = read(0, &c, 1);
    if(cc < 1)
      break;
    buf[i++] = c;
    if(c == '\n' || c == '\r')
      break;
  }
  buf[i] = '\0';
  return buf;
}

int
stat(const char *n, struct stat *st)
{
  int fd;
  int r;

  fd = open(n, O_RDONLY);
  if(fd < 0)
    return -1;
  r = fstat(fd, st);
  close(fd);
  return r;
}

int
atoi(const char *s)
{
  int n;

  n = 0;
  while('0' <= *s && *s <= '9')
    n = n*10 + *s++ - '0';
  return n;
}

void*
memmove(void *vdst, const void *vsrc, int n)
{
  char *dst;
  const char *src;

  dst = vdst;
  src = vsrc;
  while(n-- > 0)
    *dst++ = *src++;
  return vdst;
}

//************************************ 611 Assignment Methods **************************************************
// This function reads the data from the file descriptor either to the end of line or max length and stores in the buffer.
int readline(int fd, char *buf, int max_len) {
    int i = 0;
    char c;

    while (i < max_len - 1) {
        int n = read(fd, &c, 1);
        if (n <= 0 || c == '\n') {
            break;
        }
        buf[i++] = c;
    }
    buf[i] = '\0';
    return i;
}

void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Convert uppercase character to lowercase by adding ASCII offset
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

void uniq_user(int fd,char *uflag) {
  char current_line[80]="";
  char previous_line[80]="";
  char previous_line_1[80]="";
  char current_line_1[80]="";

    int bytesRead;
    int lineCount=1;
    
   // while ((bytesRead = readline(fd, current_line, sizeof(current_line))) > 0) { 
    while ((bytesRead = readline(fd, current_line,80)) > 0) {     
            if (strcmp(previous_line,"") == 0) {
                strcpy(previous_line,current_line); 
                lineCount=1;
            } 
            else if (strcmp(uflag,"-i") == 0) {
                strcpy(current_line_1,current_line); 
                strcpy(previous_line_1,previous_line); 
                toLower(current_line_1);
                toLower(previous_line_1);
                if (strcmp(current_line_1, previous_line_1) != 0) {
                    printf(1,"%s\n",previous_line);
                    strcpy(previous_line,current_line);
                }
            }
            else if (strcmp(current_line, previous_line) != 0) {
                if (strcmp(uflag,"-c") == 0) 
                    printf(1,"%d\t%s\n",lineCount,previous_line);
                else if ( (strcmp(uflag,"-d") == 0 && lineCount >1 ) ||  (strcmp(uflag,"-g") == 0))
                    printf(1,"%s\n",previous_line);

                strcpy(previous_line,current_line);
                lineCount=1;  // resets the line count
            }  
            else 
                lineCount = lineCount + 1;
    }
    if (strcmp(previous_line,"") != 0 ) {
        if (strcmp(uflag,"-c") == 0)
            printf(1,"%d\t%s\n",lineCount,previous_line);
        else if ( (strcmp(uflag,"-d") == 0 && lineCount >1 ) 
                        ||  (strcmp(uflag,"-i") == 0) ||  (strcmp(uflag,"-g") == 0))
            printf(1,"%s\n",previous_line);
    }
    else {
        printf(1,"\n");
    }    
}
