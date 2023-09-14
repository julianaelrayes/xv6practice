#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_NLINES 14

int main(int argc, char *argv[])
{ 
    int nlines   = DEFAULT_NLINES;
    int lineType = 0;       // Standard Input
    char *filename = "";    // Initialize filename to an empty string

    // Print a message indicating that the Head command is executing in kernel mode.
    printf(1, "********************* Head command is getting executed in kernel mode ******************************\n");

    // Check the number of command-line arguments.
    if (argc == 1) {
        nlines = DEFAULT_NLINES; // If no arguments, use the default number of lines.
    }
    else if (argc == 2)
    {
        // If there's one argument, check if it starts with a dash (e.g., "-10" for line count) or if it's a filename.
        if (argv[1][0] == '-') {
            nlines = atoi(&argv[1][1]); // Extract and convert the number of lines.
            if (nlines <= 0) {
                printf(2, "Invalid number of lines: %s\n", argv[1]);
                exit(); // Handle the case of an invalid number of lines.
            }
        }
        else {
            filename = argv[1]; // Set the filename if it's not a dash-prefixed number.
            lineType = 1;       // Indicate that we are reading from a file, not standard input.
        }
    }
    else if (argc == 3)
    {
        // If there are two arguments, assume the first one is the line count and the second one is the filename.
        nlines = atoi(&argv[1][1]); // Extract and convert the number of lines.
        if (nlines <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[1]);
            exit(); // Handle the case of an invalid number of lines.
        }
        filename = argv[2]; // Set the filename.
        lineType = 1;       // Indicate that we are reading from a file, not standard input.
    }

    // Call the head_kernel function with the specified filename, line count, and line type.
    head_kernel(filename, nlines, lineType);
   
    exit(); // Exit the program.
}
