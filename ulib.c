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

//************************************ 611 Assignment Functions **************************************************

// Function to read a line from a file descriptor 'fd' into 'buf'
// 'max_len' specifies the maximum number of characters to read
int readline(int fd, char *buf, int max_len) {
    int i = 0;  // Initialize an index variable 'i' to keep track of characters read.
    char c;     // Initialize a character variable 'c' to store the read character.

    // Loop until 'max_len - 1' characters have been read, or until a newline character is encountered.
    while (i < max_len - 1) {
        int n = read(fd, &c, 1);  // Read a single character from 'fd' into 'c'.
        if (n <= 0 || c == '\n') {
            break;  // Exit the loop if the read operation fails or a newline character is encountered.
        }
        buf[i++] = c;  // Store the read character in 'buf' and increment the index 'i'.
    }
    buf[i] = '\0';  // Null-terminate the 'buf' string to mark the end of the line.
    return i;  // Return the number of characters read (excluding the null terminator).
}


// Function to convert a string 'str' to lowercase
void toLower(char *str) {
    // Loop through each character in the string until the null terminator is encountered ('\0').
    for (int i = 0; str[i] != '\0'; i++) {
        // Check if the current character is an uppercase letter (A-Z).
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Convert the uppercase character to lowercase by adding the ASCII offset.
            // This is achieved by adding the difference between the ASCII values of 'a' and 'A'.
            str[i] = str[i] + ('a' - 'A');
        }
    }
}

void uniq_user(int fd, char *uflag) {
    char current_line[80] = "";    // Current line buffer
    char previous_line[80] = "";   // Previous line buffer
    char previous_line_1[80] = ""; // Previous line buffer (for case-insensitive comparison)
    char current_line_1[80] = "";  // Current line buffer (for case-insensitive comparison)

    int bytesRead;                // Number of bytes read
    int lineCount = 1;            // Line count, initialized to 1

    // Loop to read lines from the file descriptor 'fd' into 'current_line'
    while ((bytesRead = readline(fd, current_line, 80)) > 0) {
        // Check if 'previous_line' is empty (first line)
        if (strcmp(previous_line, "") == 0) {
            strcpy(previous_line, current_line); // Copy 'current_line' to 'previous_line'
            lineCount = 1; // Reset the line count
        } 
        // Check if the '-i' flag is provided for case-insensitive comparison
        else if (strcmp(uflag, "-i") == 0) {
            strcpy(current_line_1, current_line); // Copy 'current_line' to 'current_line_1'
            strcpy(previous_line_1, previous_line); // Copy 'previous_line' to 'previous_line_1'
            toLower(current_line_1); // Convert 'current_line_1' to lowercase
            toLower(previous_line_1); // Convert 'previous_line_1' to lowercase
            // Compare 'current_line_1' and 'previous_line_1' for case-insensitive equality
            if (strcmp(current_line_1, previous_line_1) != 0) {
                // Print 'previous_line' because it's different from 'current_line'
                printf(1, "%s\n", previous_line);
                strcpy(previous_line, current_line); // Copy 'current_line' to 'previous_line'
            }
        }
        // Case-sensitive comparison if no special flag is provided
        else if (strcmp(current_line, previous_line) != 0) {
            if (strcmp(uflag, "-c") == 0)
                // Print line count and 'previous_line' for the '-c' flag
                printf(1, "%d\t%s\n", lineCount, previous_line);
            else if ((strcmp(uflag, "-d") == 0 && lineCount > 1) || (strcmp(uflag, "-g") == 0))
                // Print 'previous_line' for the '-d' flag (lineCount > 1) or '-g' flag
                printf(1, "%s\n", previous_line);

            strcpy(previous_line, current_line); // Copy 'current_line' to 'previous_line'
            lineCount = 1; // Reset the line count
        }  
        else 
            lineCount++; // Increment the line count if 'current_line' is the same as 'previous_line'
    }

    // Handle the last line if 'previous_line' is not empty
    if (strcmp(previous_line, "") != 0) {
        if (strcmp(uflag, "-c") == 0)
            // Print line count and 'previous_line' for the '-c' flag
            printf(1, "%d\t%s\n", lineCount, previous_line);
        else if ((strcmp(uflag, "-d") == 0 && lineCount > 1) || (strcmp(uflag, "-i") == 0) || (strcmp(uflag, "-g") == 0))
            // Print 'previous_line' for the '-d', '-i', or '-g' flag
            printf(1, "%s\n", previous_line);
    }
    else {
        // Print a newline if 'previous_line' was empty (no lines read)
        printf(1, "\n");
    }
}