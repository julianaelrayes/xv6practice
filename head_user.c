#include "types.h"
#include "stat.h"
#include "user.h"

#define DEFAULT_NLINES 14

void head(int fd, int nlines) {
    char buf[512];
    int n = -1;
    int lines = 0;

    while (lines < nlines && (n = read(fd, buf, sizeof(buf))) > 0) {
        for (int i = 0; i < n; i++) {
            if (buf[i] == '\n') {
                lines++;
                if (lines == nlines) {
                    break;
                }
            }
            printf(1, "%c", buf[i]);
        }
    }

    // Add an extra newline if the specified number of lines is reached
    printf(1,"\n");
   
}

int main(int argc, char *argv[]) {
    int nlines = DEFAULT_NLINES;
    int fd;
	printf(1,"********************* Head Command is getting Executed in User Space ******************************\n");
    if (argc > 1 && argv[1][0] == '-') {
        nlines = atoi(&argv[1][1]);
        if (nlines <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[1]);
            exit();
        }
    }

    if (argc == 1 || (argc == 2 && argv[1][0] == '-')) {
        head(0, nlines); // Read from standard input
        exit();
    }

    for (int i = 1; i < argc; i++) {
        if (argv[i][0] != '-' && (fd = open(argv[i], 0)) < 0) {
            printf(2, "head: cannot open %s\n", argv[i]);
            exit();
        }

        if (argc > 2 && argv[i][0] != '-') {
            if (i > 1) {
                printf(1, "\n"); // Print a newline before the content of the second and subsequent files
            }
            printf(1, "==> %s <==\n", argv[i]);
        }

        if (argv[i][0] != '-') {
            head(fd, nlines);
            close(fd);
        }
    }

    exit();
}