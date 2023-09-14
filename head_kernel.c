#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

#define DEFAULT_NLINES 14

int main(int argc, char *argv[])
{ 
    int nlines   = DEFAULT_NLINES;
    int lineType = 0;       //Standard Input
    char *filename="";
    printf(1,"********************* Head Command is getting Executed in Kernal Space ******************************\n");
    //printf(1,"%d\n",get_lines("OS611Example.txt",10,1));
    if (argc == 1) {
        nlines = DEFAULT_NLINES;
    }
    else if (argc == 2)
    {
        if (argv[1][0] == '-') {
            nlines = atoi(&argv[1][1]);
            if (nlines <= 0) {
                printf(2, "Invalid number of lines: %s\n", argv[1]);
                exit();  
            }
        }
        else {
            filename = argv[1];
            lineType = 1;
        }
    }
    else if (argc == 3)
    {
        nlines = atoi(&argv[1][1]);
        if (nlines <= 0) {
            printf(2, "Invalid number of lines: %s\n", argv[1]);
            exit();  
        }
        filename = argv[2];
        lineType = 1;
    }
   
    //printf(1,"%s\t%d\t%d\n",filename,nlines,lineType);
    head_kernel(filename,nlines,lineType);
   
    exit();
}