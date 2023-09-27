#include "types.h"
#include "stat.h"
#include "user.h"

void record_process_info(char *command, char **args) {
    uint create_time, end_time, total_time;
    
    create_time = uptime(); // Record creation time

    // Execute the command using exec
    int pid = fork();
    if (pid == 0) {
        exec(command, args);
        printf(2, "Exec '%s' failed.\n", command);
        exit();
    } else if (pid > 0) {
        // Wait for the child process to finish
        wait();
    } else {
        printf(2, "Fork failed.\n");
        exit();
    }

    end_time = uptime(); // Record end time
    total_time = end_time - create_time; // Calculate total time

    // Display process information
    printf(1, "\nProcess Info:\n");
    printf(1, "  Creation Time: %d\n", create_time);
    printf(1, "  End Time: %d\n", end_time);
    printf(1, "  Total Time: %d\n", total_time);
}

int main(int argc, char *argv[]) {
    // Check if a command and arguments were provided
    if (argc < 2) {
        printf(2, "Usage: test <command> [args...]\n");
        exit();
    }

    // Record process information for the provided command and arguments
    record_process_info(argv[1], &argv[1]);

    exit();
}
