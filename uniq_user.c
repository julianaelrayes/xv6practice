#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[]) { 

    int fd =-1;
    char *defaultFlag = "-n"; // Default flag if no flag is provided
    
    printf(1,"\n*** Uniq command is getting executed in user mode ***\n\n");

    // If no command-line arguments provided
    if (argc == 1) { 
        uniq_user(0,defaultFlag); // Execute uniq_user with default flag

    // If one command-line argument provided
    } else if (argc == 2) {
        // Attempt to open the specified input file
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            printf(2, "Failed to open the input file: %s\n", argv[1]);
            exit();
        }

        uniq_user(fd,defaultFlag); // Execute uniq_user with default flag

    // If two command-line arguments provided
    } else if (argc == 3) {
        // Attempt to open the specified input file
        if ((fd = open(argv[2], O_RDONLY)) < 0) {
            printf(2, "Failed to open the input file: %s\n", argv[1]);
            exit();
        }

        uniq_user(fd,argv[1]); // Execute uniq_user with the specified flag

    }

    exit();
}
