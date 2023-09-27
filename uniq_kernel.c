#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{ 
    // Print a message indicating the space that the code is being executed in
    printf(1,"\n*** Uniq command is getting executed in kernel mode ***\n\n");

    // Verify the number of command-line arguments.
    if (argc == 1) 
        // When there are no arguments, invoke uniq_kernel with "stdin" as the input and "z" as the option
        uniq_kernel("stdin", "z");
    else if (argc == 2)
        // In case of a single argument, use it as input and add the "-n" option when calling uniq_kernel
        uniq_kernel(argv[1], "-n");
    else if (argc == 3) {
        // Examine if the first argument qualifies as a valid option (-c, -d, or -i)
        if (argv[1][0] == '-' && (argv[1][1] == 'c' || argv[1][1] == 'i' || argv[1][1] == 'd')) {
            // If it's a valid option, invoke uniq_kernel with the second argument and the first argument as options
            uniq_kernel(argv[2], argv[1]);
        }
        else
            // Print an error message to notify about an invalid option and suggest using '-c', '-i', or '-d' as valid alternatives.
            printf(2, "Invalid option: %s. Please use '-c', '-i', or '-d' as valid options.\n", argv[1]);

    }
    
    // Exit the program.
    exit();
}
 