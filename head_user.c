#include "types.h"
#include "stat.h"
#include "user.h"

#define DEFAULT_NLINES 14

// Function to display the first 'nlines' lines of a file
void head(int fd, int nlines) {
    char buf[512];
    int n = -1;
    int lines = 0;

    // Read from the file descriptor 'fd' into 'buf' until 'nlines' lines are read or EOF is reached
    while (lines < nlines && (n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                lines++; // Increment line count when a newline character is encountered
                if (lines == nlines) {
                    break; // Exit the loop if the specified number of lines is reached
                }
            }
            printf(1, "%c", buf[i]); // Print the character to the console
        }
    }

    // Add an extra newline after displaying the lines
    printf(1, "\n");
}

int main(int argc, char *argv[]) {
    int nlines = DEFAULT_NLINES;
    int fd;
    printf(1, "********************* Head command is getting executed in user mode ******************************\n");

    // Check for the presence of a command-line argument that specifies the number of lines
    if (argc > 1 && argv[1][0] == '-') {
        nlines = atoi(&argv[1][1]); // Extract and convert the number of lines
        if (nlines <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[1]);
            exit();
        }
    }

    // If there are no arguments or only a line count argument, read from standard input
    if (argc == 1 || (argc == 2 && argv[1][0] == '-')) {
        head(0, nlines); // Read from standard input
        exit();
    }

    // Loop through the command-line arguments (file names)
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && (fd = open(argv[i], 0)) < 0) {
            printf(2, "head: cannot open %s\n", argv[i]); // Handle file opening errors
            exit();
        }

        // Print a section header if there are multiple files
        if (argc > 2 && argv[i][0] != '-') {
            if (i > 1) {
                printf(1, "\n"); // Print a newline before the content of the second and subsequent files
            }
            printf(1, "==> %s <==\n", argv[i]); // Display the current file name as a section header
        }

        // Call the 'head' function to display the first 'nlines' lines of the current file
        if (argv[i][0] != '-') {
            head(fd, nlines);
            close(fd); // Close the file descriptor
        }
    }

    exit(); // Exit the program
}
