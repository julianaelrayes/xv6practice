#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#define BUF_SIZE 256
#define O_RDONLY  0x000

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

// new_wait syscall ******** changed whole function
int
sys_new_wait(void){

  int *create_time, *stop_time, *run_time;
  if(argptr(0, (char**)&create_time, sizeof(int)) < 0 ||
     argptr(1, (char**)&stop_time, sizeof(int)) < 0 ||
     argptr(2, (char**)&run_time, sizeof(int)) < 0)
     {
      return -1;
     }

     return new_wait(create_time, stop_time, run_time);
}
  // int *run_time, *wait_time;
  // if(argptr(0, (char**)&wait_time, sizeof(int)) < 0)
  //   return 12;

  // if(argptr(1, (char**)&run_time, sizeof(int)) < 0)
  //   return 13;

  // return new_wait(wait_time,run_time);
// }


// This function compares two strings 'p' and 'q' until a difference or the end of either string is reached.
int strcmp(const char *p, const char *q) {
    // Iterate through the characters of 'p' and 'q' as long as they are equal.
    while (*p && *p == *q) {
        p++; 
        q++; 
    }
    // Calculate and return the ASCII difference between the current characters in 'p' and 'q'.
    return (uchar)*p - (uchar)*q;
}

// This function duplicates the content of string 'source' into string 'destination' and returns the initial address of 'destination'.
char* strcpy(char *destination, const char *source) {
    char *original_destination; // Store the original address of 'destination' to return later

    original_destination = destination; // Save the initial address of 'destination'

    // Iterate until the end of 'source' is reached
    while ((*destination++ = *source++) != 0) {
        // Copy each character from 'source' to 'destination' while advancing both pointers
        // The loop continues until the end of 'source', effectively duplicating 'source' into 'destination'
        ;
    }

    // Return the original address of 'destination' (the start of the copied string)
    return original_destination;
}

// Read a line of text from the given 'inode' starting from 'startPosition' and store it in 'buf'
int readline(struct inode *inode, char *buf, int start, int max_len) {
    // Attempt to read data from the 'inode' starting at 'startPosition' and copy it into 'buf'
    int bytes_read = readi(inode, buf, start, BUF_SIZE);
    int char_index = 0;

    // Check if there was an error during the read operation; if so, return -1
    if (bytes_read < 0) {
        return -1;
    }

    // If the number of bytes read is less than the buffer size, null-terminate 'buf'
    if (bytes_read < sizeof(buf)) {
        buf[bytes_read] = '\0';
    }

    // Iterate through the characters in 'buf' until a newline character is found or we reach the total read bytes
    for (; char_index < strlen(buf); char_index++) {
        // If a newline character is encountered or we've reached the end of the read data, null-terminate 'buf'
        if (buf[char_index] == '\n' || char_index >= bytes_read) {
            buf[char_index] = '\0';
            break;
        }
    }

    // Return the char_index, representing the number of characters read.
    return char_index;
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

// Extracts a line from the 'data' buffer starting at the given 'offset'
int extract_line(int startPos, char *data, char *line) {
    int lineIndex = 0; // Initialize the index for the 'line' buffer.
    int dataIndex = startPos; // Initialize 'dataIndex' with the provided 'startPos'.

    // Check if 'startPos' is beyond the length of the 'data' buffer.
    if (startPos >= strlen(data)) {
        return -1; // Return -1 to indicate an error (startPos out of bounds).
    }

    // Iterate through the characters in 'data' starting from 'startPos'.
    while (data[dataIndex] != '\0' && data[dataIndex] != '\n') {
        line[lineIndex] = data[dataIndex]; // Copy the character from 'data' to 'line'.
        lineIndex++; // Move to the next position in 'line'.
        dataIndex++; // Move to the next character in 'data'.
    }

    // Null-terminate 'line' to create a valid C string.
    line[lineIndex] = '\0';

    // Check if the loop exited due to a newline or the end of 'data'.
    if (data[dataIndex] == '\n') {
        dataIndex++; // Move past the newline character.
    }

    // Return the index of the last character read (position after the newline or end of 'data').
    return dataIndex;
}


// Kernel function to implement the 'uniq' command
int sys_uniq_kernel(void) {
    char *filename; // Pointer to store the filename
    char *flag;     // Pointer to store the flag (e.g., '-c', '-i', '-d', '-n', or 'z')

    // Check if the first and second arguments are valid strings and store them in 'filename' and 'flag'
    if (argstr(0, &filename) < 0 || argstr(1, &flag) < 0)
        return -1; // Return -1 to indicate an error

    char current_line[BUF_SIZE] = "";  // Buffer to store the current line
    char previous_line[BUF_SIZE] = ""; // Buffer to store the previous line
    char repeated_line[256] = "";      // Buffer to store repeated lines

    int lineCount = 1;        // Line count, initialized to 1
    int bytesRead = 0;        // Number of bytes read
    int isRepeated = 0;       // Flag to track repeated lines
    int indexCounter = 0;     // Index counter for tracking the file position
    int isCountFlag = (strcmp(flag, "-c") == 0);            // Flag for counting lines
    int isCaseInsensitive = (strcmp(flag, "-i") == 0);      // Flag for case-insensitive comparison
    int isDelete = (strcmp(flag, "-d") == 0);              // Flag for deleting duplicate lines
    int isNoFlag = (strcmp(flag, "-n") == 0);              // Flag for deleting or no flag
    int isStdin = (strcmp(flag, "-z") == 0);               // Flag for stdin input

    // Check if the provided flag is 'z' (indicating input from stdin)
    if (isStdin) {
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
        while ((offset = extract_line(offset, buffer, current_line)) > 0) {
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
        cprintf("%d  %s\n", lineCount, previous_line); // Print with line count
            
      } else if (isNoFlag && lineCount > 1) {
        cprintf("%s\n", previous_line);
    
     } else if (isDelete) {
        isRepeated = 0;

      }  else {
          cprintf("%s\n", previous_line); // Always print
          strcpy(previous_line, current_line); // Update previous_line
        }
            
        
      strcpy(previous_line, current_line); // Update previous_line
      lineCount = 1; // Reset line count for the next unique line

    } else {
      if (isDelete && !isRepeated){
        strcpy(repeated_line, current_line);
        cprintf("%s\n", previous_line);
        isRepeated = 1;
      }
      lineCount++; // Increment line count for consecutive lines
    }

  }
  
  // Print the last line if necessary.
  if (strcmp(previous_line, "") != 0) {
    if (isCountFlag) {
      cprintf("%d  %s\n", lineCount, previous_line); // Print with line count
    
    } else if (isNoFlag || isCaseInsensitive) {
      cprintf("%s\n", previous_line); // Always print for -i and -n flags

    }

  } else {
    cprintf("\n"); // Print a newline if the file was empty
  
  }
         iunlock(ip);
    end_op();
  }
  return 0;
}


// Implementation of the 'head' command kernel function
int sys_head_kernel(void) {
    char *targetFile;     // Store the name of the target file
    int numLinesToShow;   // Specify the number of lines to display
    int inputSource;      // Indicate the input source: 0 for Standard Input, 1 for File

    // Check if the provided arguments are valid strings and integers,
    // and store them in 'targetFile', 'numLinesToShow', and 'inputSource'
    if (argstr(0, &targetFile) < 0 || argint(1, &numLinesToShow) < 0 || argint(2, &inputSource) < 0)
        return -1; // Return -1 to indicate an error

    if (inputSource == 0) {
        char lineBuffer[1024];  // Buffer to store the current line
        char currentChar;       // The current character being read
        struct file *stdinFile = myproc()->ofile[0]; // Access the standard input file descriptor

        int bytesRead = -1;     // Number of bytes read
        int bufferIndex = 0;    // Index in the lineBuffer
        int linesDisplayed = 1; // Count of lines displayed

        // Read input data from stdin into 'lineBuffer' and print the specified number of lines
        while ((bytesRead = fileread(stdinFile, &currentChar, 1)) > 0) {
            if (bytesRead < 0) {
                break;
            } else {
                if (currentChar == '\n') {
                    lineBuffer[bufferIndex++] = '\0';  // Null-terminate the line
                    cprintf("%s\n", lineBuffer);       // Print the line
                    memset(lineBuffer, '\0', sizeof(lineBuffer)); // Reset the buffer
                    linesDisplayed++;                   // Increment the line count
                    bufferIndex = 0;

                    // Break the loop if the specified number of lines is reached
                    if (linesDisplayed > numLinesToShow)
                        break;
                } else
                    lineBuffer[bufferIndex++] = currentChar; // Add character to the buffer
            }
        }
    } else {
        int linesRead = 1;            // Count of lines read
        int bytesRead = 0;            // Number of bytes read
        int fileIndex = 0;            // Index within the file
        char currentLine[1024] = "";  // Buffer to store a line

        // Look up the inode for the specified filename
        begin_op();
        struct inode *targetInode;
        if ((targetInode = namei(targetFile)) == 0) {
            end_op();
            cprintf("Error: The specified file was not found.\n"); // Updated error message
            return -1; // File Not Found
        }
        ilock(targetInode);

        // Read data from the file into 'currentLine' and print the specified number of lines
        while (linesRead <= numLinesToShow && (bytesRead = readline(targetInode, currentLine, fileIndex, BUF_SIZE)) > 0) {
            if (bytesRead < 0)
                break;
            fileIndex += bytesRead + 1;

            cprintf("%s\n", currentLine); // Print the line
            linesRead++;                  // Increment the line count
        }
        cprintf("\n");
        iunlock(targetInode);
        end_op();
    }
    return numLinesToShow; // Return the number of lines displayed
}
