#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{ 
    // Print a message indicating that the Uniq command is executing in kernel mode.
    printf(1, "********************* Uniq command is getting executed in kernel mode ******************************\n");

    // Check the number of command-line arguments.
    if (argc == 1) 
        // If there are no arguments, call uniq_kernel with "stdin" and "x".
        uniq_kernel("stdin", "x");
    else if (argc == 2)
        // If there is one argument, call uniq_kernel with the argument and "-g".
        uniq_kernel(argv[1], "-g");
    else if (argc == 3) {
        // Check if the first argument is a valid option (-c, -d, or -i).
        if (argv[1][0] == '-' && (argv[1][1] == 'c' || argv[1][1] == 'd' || argv[1][1] == 'i')) {
            // If it is a valid option, call uniq_kernel with the second argument and the first argument as options.
            uniq_kernel(argv[2], argv[1]);
        }
        else
            // Print an error message for an invalid option.
            printf(2, "Error Invalid Option %s. The valid Options are '-c' or '-d' or '-i'\n", argv[1]);
    }
    
    // Exit the program.
    exit();
}
