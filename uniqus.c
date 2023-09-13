#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{ 
    printf(1,"********************* Executing under User Space ******************************\n");
    int fd =-1;
    if (argc == 1) 
        uniq_us(0,"-g");
    else if (argc == 2)
    {
        if ((fd = open(argv[1], O_RDONLY)) < 0) {
            printf(2, "Error opening input file: %s\n", argv[1]);
            exit();
        }
        uniq_us(fd,"-g");
    }
    else if (argc == 3) {
        if (argv[1][0] == '-' && (argv[1][1]=='c' || argv[1][1]=='d' || argv[1][1]=='i')) {
            if ((fd = open(argv[2], O_RDONLY)) < 0) {
                printf(2, "Error opening input file: %s\n", argv[1]);
                exit();
             }
            uniq_us(fd,argv[1]);
        }
        else
            printf(2, "Error Invalid Option %s.  The valid Options are '-c' or '-d' or '-i' \n", argv[1]);       
    }
    exit();
}