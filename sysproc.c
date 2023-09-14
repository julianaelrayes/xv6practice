#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#define BUF_SIZE 80
#define O_RDONLY  0x000


// Compare two null-terminated strings 'p' and 'q'
int strcmp(const char *p, const char *q) {
    while (*p && *p == *q) {
        p++; // Move to the next character in 'p'
        q++; // Move to the next character in 'q'
    }
    // Return the difference between the ASCII values of the current characters in 'p' and 'q'
    return (uchar)*p - (uchar)*q;
}

// Copy the string 't' to 's' and return 's'
char* strcpy(char *s, const char *t) {
    char *os; // Store the original address of 's' to return it later

    os = s; // Assign the original address of 's' to 'os'

    // Loop until the null terminator of 't' is encountered
    while ((*s++ = *t++) != 0) {
        // Copy the character from 't' to 's' and increment both pointers
        // The loop continues until the null terminator is reached, and then it terminates
        // This effectively copies the entire string 't' to 's'
        ;
    }

    // Return the original address of 's' (the address of the beginning of the copied string)
    return os;
}

// Function to read a line from an inode 'ip' into 'buf'
// 'startPosition' specifies the starting position in the inode
// 'max_len' specifies the maximum number of characters to read
int readline(struct inode *ip, char *buf, int startPosition, int max_len) {
    // Read data from the inode 'ip' starting from 'startPosition' and store it in 'buf'
    int total = readi(ip, buf, startPosition, BUF_SIZE);
    //cprintf("Total Bytes Read:%d\n", total);

    // If the read operation returns an error, return -1
    if (total < 0)
        return -1;

    // Null-terminate 'buf' if the total read bytes are less than the buffer size
    if (total < sizeof(buf))
        buf[total] = '\0';

    int index = 0;

    // Loop through the characters in 'buf' until a newline character is encountered
    // or the index reaches the total number of characters read
    for (; index < strlen(buf); index++) {
        if (buf[index] == '\n' || index >= total) {
            buf[index] = '\0'; // Null-terminate 'buf' at the newline character or at the end of the read data
            break;
        }
    }
    //cprintf("Readline:%d\t%d\n", total, index);

    // Return the index, which represents the number of characters read
    return index;
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

// Function to extract a line from a data buffer starting from a given offset
// 'offset' specifies the starting position in the 'data' buffer
// 'data' is the source data buffer
// 'line' is the output buffer to store the extracted line
int getline_array(int offset, char *data, char *line) {
    int arrayIndex = 0; // Index for the 'line' buffer
    int index = offset; // Initialize 'index' with the provided 'offset'

    // Check if 'offset' is beyond the length of the 'data' buffer
    if (offset >= strlen(data))
        return -1; // Return -1 to indicate an error

    // Loop through the characters in 'data' starting from 'offset'
    for (; index < strlen(data); index++) {
        line[arrayIndex] = data[index]; // Copy the character from 'data' to 'line'

        // Check if the copied character is a newline character
        if (data[index] == '\n') {
            line[arrayIndex] = '\0'; // Null-terminate 'line' at the newline character
            break; // Exit the loop when a newline character is encountered
        } else {
            arrayIndex++; // Increment the 'arrayIndex' to move to the next position in 'line'
        }
    }
    return index; // Return the index of the last character read (position after the newline or end of 'data')
}

// Kernel function to implement the 'uniq' command
int sys_uniq_kernel(void) {
    char *filename; // Pointer to store the filename
    char *flag;     // Pointer to store the flag (e.g., '-c', '-d', '-i', '-g', or 'x')

    // Check if the first and second arguments are valid strings and store them in 'filename' and 'flag'
    if (argstr(0, &filename) < 0 || argstr(1, &flag) < 0)
        return -1; // Return -1 to indicate an error

    int lineCount = 1;        // Line count, initialized to 1
    int bytesRead = 0;        // Number of bytes read
    int indexCounter = 0;     // Index counter for tracking the position in the file

    char current_line[BUF_SIZE] = "";  // Current line buffer
    char previous_line[BUF_SIZE] = ""; // Previous line buffer
    char previous_line_1[80] = "";    // Previous line buffer (for case-insensitive comparison)
    char current_line_1[80] = "";     // Current line buffer (for case-insensitive comparison)

    // Check if the provided flag is 'x' (indicating input from stdin)
    if (strcmp(flag, "x") == 0) {
        char buffer[1024];
        char c;
        struct file *f = myproc()->ofile[0]; // Access stdin file descriptor

        int inbytes = -1;
        int i = 0;

        // Read input data from stdin into 'buffer'
        while ((inbytes = fileread(f, &c, 1)) > 0) {
            if (inbytes < 0)
                break;
            else
                buffer[i++] = c;
        }
        buffer[i] = '\0';

        int offset = 0;

        // Process the data from 'buffer' line by line
        while ((offset = getline_array(offset, buffer, current_line)) > 0) {
            if (offset < 0) {
                break;
            }

            // Check if 'previous_line' is empty (first line)
            if (strcmp(previous_line, "") == 0) {
                strcpy(previous_line, current_line);
                lineCount = 1;
            } else if (strcmp(current_line, previous_line) != 0) {
                cprintf("%s\n", previous_line);
                strcpy(previous_line, current_line);
                lineCount = 1;
            } else
                lineCount = lineCount + 1;
            offset++;
        }
        if (strcmp(previous_line, "") != 0) {
            cprintf("%s\n", previous_line);
        } else {
            cprintf("\n");
        }
    } else {
        begin_op();
        struct inode *ip;

        // Look up the inode for the specified filename
        if ((ip = namei(filename)) == 0) {
            end_op();
            cprintf("File Not Found ....\n");
            return -1; // File Not Found
        }
        ilock(ip);

        // Read data from the file into 'current_line' and process it
        while ((bytesRead = readline(ip, current_line, indexCounter, BUF_SIZE)) > 0) {
            if (bytesRead < 0)
                break;
            indexCounter = indexCounter + bytesRead + 1;

            // Check if 'previous_line' is empty (first line)
            if (strcmp(previous_line, "") == 0) {
                strcpy(previous_line, current_line);
                lineCount = 1;
            } else if (strcmp(flag, "-i") == 0) {
                strcpy(current_line_1, current_line);
                strcpy(previous_line_1, previous_line);
                toLower(current_line_1);
                toLower(previous_line_1);
                if (strcmp(current_line_1, previous_line_1) != 0) {
                    cprintf("%s\n", previous_line);
                    strcpy(previous_line, current_line);
                }
            } else if (strcmp(current_line, previous_line) != 0) {
                if (strcmp(flag, "-c") == 0)
                    cprintf("%d\t%s\n", lineCount, previous_line);
                else if ((strcmp(flag, "-d") == 0 && lineCount > 1) || (strcmp(flag, "-g") == 0))
                    cprintf("%s\n", previous_line);
                strcpy(previous_line, current_line);
                lineCount = 1;
            } else
                lineCount = lineCount + 1;
        }

        // Handle the last line if 'previous_line' is not empty
        if (strcmp(previous_line, "") != 0) {
            if (strcmp(flag, "-c") == 0)
                cprintf("%d\t%s\n", lineCount, previous_line);
            else if ((strcmp(flag, "-d") == 0 && lineCount > 1) || (strcmp(flag, "-i") == 0) || (strcmp(flag, "-g") == 0))
                cprintf("%s\n", previous_line);
        } else {
            cprintf("\n");
        }
         iunlock(ip);
    end_op();
  }
  return 0;
}


// Kernel function to implement the 'head' command
int sys_head_kernel(void) {
    char *filename;     // Pointer to store the filename
    int linecount;      // Number of lines to display
    int inputType;      // 0: Standard Input, 1: File Name

    // Check if the provided arguments are valid strings and integers and store them in 'filename', 'linecount', and 'inputType'
    if (argstr(0, &filename) < 0 || argint(1, &linecount) < 0 || argint(2, &inputType) < 0)
        return -1; // Return -1 to indicate an error

    if (inputType == 0) {
        char buffer[1024];
        char c;
        struct file *f = myproc()->ofile[0]; // Access stdin file descriptor

        int inbytes = -1;
        int i = 0;
        int lines = 1;

        // Read input data from stdin into 'buffer' and print the specified number of lines
        while ((inbytes = fileread(f, &c, 1)) > 0) {
            if (inbytes < 0) {
                break;
            } else {
                if (c == '\n') {
                    buffer[i++] = '\0';
                    cprintf("%s\n", buffer);
                    memset(buffer, '\0', sizeof(buffer)); // Reset the array
                    lines = lines + 1;
                    i = 0;

                    // Break the loop if the specified number of lines is reached
                    if (lines > linecount)
                        break;
                } else
                    buffer[i++] = c;
            }
        }
    } else {
        int readlines = 1;
        int bytesRead = 0;
        int indexCounter = 0;
        char current_line[1024] = "";

        // Look up the inode for the specified filename
        begin_op();
        struct inode *ip;
        if ((ip = namei(filename)) == 0) {
            end_op();
            cprintf("File Not Found ....\n");
            return -1; // File Not Found
        }
        ilock(ip);

        // Read data from the file into 'current_line' and print the specified number of lines
        while (readlines <= linecount && (bytesRead = readline(ip, current_line, indexCounter, BUF_SIZE)) > 0) {
            if (bytesRead < 0)
                break;
            indexCounter = indexCounter + bytesRead + 1;

            cprintf("%s\n", current_line);
            readlines = readlines + 1;
        }
        cprintf("\n");
        iunlock(ip);
        end_op();
    }
    return linecount; // Return the number of lines displayed
}

//********************************************Above Methods are added for Assignment 611 ********************************************
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
