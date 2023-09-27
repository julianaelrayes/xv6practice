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

/* New functions inmplemented below...
*
*
*
*
*
*
*
*
*/ 





// Reads characters until a newline character or the end of the file is encountered.
int readline(int fd, char *buffer, int max_len) {
  int chars_read = 0; // Number of characters read
  char current_char; // The current character being read

  for (chars_read = 0; chars_read < max_len - 1; chars_read++) {
    // Read one character from the file descriptor
    int read_result = read(fd, &current_char, 1);

    // Check for errors or end of file
    if (read_result <= 0 || current_char == '\n') {
      break; // Stop reading
    }

    // Store the character in the buffer
    buffer[chars_read] = current_char;
  }

  // Null-terminate the string
  buffer[chars_read] = '\0';

  return chars_read;
}

// Converts uppercase letters in a string to lowercase.
void convertToLowercase(char *str) {
    // Loop through the string until the null terminator is reached
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if the character is an uppercase letter
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Convert uppercase character to lowercase by adding 32
            str[i] += 32;
        }
    }
}

// Identifies and prints unique lines based on the specified flag.
void uniq_user(int fd, char *flag) {
  char current_line[256] = "";        // Stores the current line being read
  char previous_line[256] = "";       // Stores the previous line
  char repeated_line[256] = ""; 

  int bytesRead;                      // Keeps track of the number of bytes
  int lineCount = 1;                  // Keeps track of the consecutive line count
  int isRepeated = 0;
  int isCountFlag = (strcmp(flag, "-c") == 0);            // Flag for counting lines
  int isCaseInsensitive = (strcmp(flag, "-i") == 0);      // Flag for case-insensitive
  int isDelete = (strcmp(flag, "-d") == 0);  // Flag for deleting 
  int isNoFlag = (strcmp(flag, "-n") == 0);

  // Read lines from the file descriptor until the end of the file
  while ((bytesRead = readline(fd, current_line, 256)) > 0) {
    int isDifferent; // Flag to indicate if the current line is different from the previous
    
    if (isCaseInsensitive) {
      char current_line_lowercase[256]; // Stores the lowercase version of the current line
      char previous_line_lowercase[256]; // Stores the lowercase version of the previous line

      strcpy(current_line_lowercase, current_line);
      convertToLowercase(current_line_lowercase);
      strcpy(previous_line_lowercase, previous_line);
      convertToLowercase(previous_line_lowercase);

      // Compare lowercase lines for differences
      isDifferent = strcmp(current_line_lowercase, previous_line_lowercase) != 0;
        
        
    } else {
      // Compare original lines
      isDifferent = strcmp(current_line, previous_line) != 0;
    }

    // Handle different lines
    if (isDifferent) {
      if (!strcmp(previous_line, "")) {
        lineCount = 1; // Reset line count if the previous line was empty
            
      } else if (isCountFlag) {
        printf(1, "%d  %s\n", lineCount, previous_line); // Print with line count
            
      } else if (isNoFlag && lineCount > 1) {

          printf(1, "%s\n", previous_line); // Print only if deleting flag or no flag
            
      } else if (isDelete) {
        isRepeated = 0;

      } else {
        printf(1, "%s\n", previous_line); // Always print
        strcpy(previous_line, current_line); // Update previous_line
      }
            
        
      strcpy(previous_line, current_line); // Update previous_line
      lineCount = 1; // Reset line count for the next unique line

    } else {
      if (isDelete && !isRepeated){
        strcpy(repeated_line, current_line);
        printf(1, "%s\n", previous_line);
        isRepeated = 1;
      }
      lineCount++; // Increment line count for consecutive lines
    }

  }
  
  // Print the last line if necessary.
  if (strcmp(previous_line, "") != 0) {
    if (isCountFlag) {
      printf(1, "%d  %s\n", lineCount, previous_line); // Print with line count
    
    } else if (isNoFlag || isCaseInsensitive) {
      printf(1, "%s\n", previous_line); // Always print for -i and -n flags
    }

  } else {
    printf(1, "\n"); // Print a newline if the file was empty
  
  }

}

