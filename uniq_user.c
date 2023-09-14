#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{ 
    // Print a message indicating that the Uniq command is executing in user mode.
    printf(1, "********************* Uniq command is getting executed in user mode ******************************\n");

    int fd = -1;

    // Check the number of command-line arguments.
    if (argc == 1) 
        uniq_user(0, "-g");
    else if (argc == 2)
    {
        // Try to open the input file specified in argv[1].
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            printf(2, "Error opening input file: %s\n", argv[1]);
            exit();
        }

        // Call the uniq_user function with the file descriptor and "-g" as arguments.
        uniq_user(fd, "-g");
    }
    else if (argc == 3) {
        // Check if the first argument is a valid option (-c, -d, or -i).
        if (argv[1][0] == '-' && (argv[1][1] == 'c' || argv[1][1] == 'd' || argv[1][1] == 'i')) {
            // Try to open the input file specified in argv[2].
            if ((fd = open(argv[2], O_RDONLY)) < 0) {
                printf(2, "Error opening input file: %s\n", argv[1]);
                exit();
            }

            // Call the uniq_user function with the file descriptor and the specified option.
            uniq_user(fd, argv[1]);
        }
        else
            // Print an error message for an invalid option.
            printf(2, "Error Invalid Option %s. The valid Options are '-c' or '-d' or '-i'\n", argv[1]);       
    }

    // Exit the program.
    exit();
}
