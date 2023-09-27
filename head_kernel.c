#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_LINES 14

int main(int argc, char *argv[]) { 
    char *targetFile = "";    // Initialize the filename as an empty string
    int numLines = DEFAULT_LINES;
    int sourceType = 0;       // 0 for Standard Input, 1 for File

    // Display a message indicating that the program is executing in kernel mode.
    printf(1,"\n*** Head command is getting executed in kernel mode ***\n\n");

    // Check the number of command-line arguments
    if (argc == 1) {


        numLines = DEFAULT_LINES; // If no arguments, use the default number of lines.
    
    
    
    } else if (argc == 2) {
        // If there's one argument, check if it starts with a dash 
       
       
        if (argv[1][0] == '-') {
            numLines = atoi(&argv[1][1]);
        
           
            if (numLines <= 0) {
                printf(2, "Invalid number of lines specified: %s\n", argv[1]);
                exit();
           
        
            }
        } else {
            sourceType = 1;       // from a file
            targetFile = argv[1]; // if it's not a dash-prefixed number 
        }
    } else if (argc == 3) {
        // first one is the line count and  second one is the filename 
        
        
        numLines = atoi(&argv[1][1]); 
        if (numLines <= 0) {
            printf(2, "Invalid number of lines specified: %s\n", argv[1]);
            exit(); //invalid number of lines.
        }
       
       
        targetFile = argv[2]; // Set the filename.
        sourceType = 1;       // from a file
    }

    // Call the head_kernel function with the specified filename, line count, and line type.
    head_kernel(targetFile, numLines, sourceType);
   
//
   //
    exit(); // Exit the program.
}
