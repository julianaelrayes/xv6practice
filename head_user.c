#include "types.h"
#include "stat.h"
#include "user.h"

#define DEFAULT_NLINES 14

// Display the first lines from the file descriptor
void head(int fileDescriptor, int numDisplayedLines) {
    char buffer[512];
    int bytesRead = -1;
    int linesDisplayed = 0;

    // Read until lines are displayed or EOF is reached.
    while (linesDisplayed < numDisplayedLines && (bytesRead = read(fileDescriptor, buffer, sizeof(buffer))) > 0) {
        for (int i = 0; i < bytesRead && linesDisplayed < numDisplayedLines; i++) {
            if (buffer[i] == '\n') {
                linesDisplayed++; 
            }
            printf(1, "%c", buffer[i]); // Print character 
        }
    }

    printf(1, "\n"); 
}


int main(int argc, char *argv[]) {
    int numLinesToDisplay = DEFAULT_NLINES;

    // Display a message indicating that the 'head' command is running in user mode.
    printf(1,"\n*** Head command is getting executed in user mode ***\n\n");

    // Check for a command-line argument specifying the number of lines to display.
    if (argc > 1 && argv[1][0] == '-') {
        numLinesToDisplay = atoi(&argv[1][1]); 
        if (numLinesToDisplay <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[1]);
            exit();
        }
        argc--; // Decrement argument count.
        argv++; // Move to the next argument.
    }

    // If no file arguments are provided, read from standard input.
    if (argc == 1 || (argc == 2 && argv[1][0] == '-')) {
        head(0, numLinesToDisplay); // Read from standard input.
        exit();
    }

    // Process each provided file.
    for (int i = 1; i < argc; i++) {
        int fileDescriptor;
        if (argv[i][0] != '-' && (fileDescriptor = open(argv[i], 0)) < 0) {
            printf(2, "head: cannot open %s\n", argv[i]); 
            continue; // Continue to the next file in case of an error
        }

        // Print a section header if there are multiple files.
        if (argc > 2 && argv[i][0] != '-') {
            if (i > 1) {
                printf(1, "\n");
            }
            printf(1, "%s\n\n", argv[i]); // Display the current file name as a section header.
        }

        // Call the head function to display the first lines of the current file 
        if (argv[i][0] != '-') {
            head(fileDescriptor, numLinesToDisplay);
            close(fileDescriptor); // Close the file descriptor 
        }
    }

    exit(); // Exit the program.
}