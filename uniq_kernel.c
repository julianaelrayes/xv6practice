#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

int main(int argc, char *argv[])
{ 
 
    printf(1,"********************* Executing under Kernel Space ******************************\n");
    if (argc == 1) 
        uniq_kernel("stdin","x");
    else if (argc == 2)
        uniq_kernel(argv[1],"-g");
    else if (argc == 3) {
        if (argv[1][0] == '-' && (argv[1][1]=='c' || argv[1][1]=='d' || argv[1][1]=='i')) {
            uniq_kernel(argv[2],argv[1]);
        }
        else
            printf(2, "Error Invalid Option %s.  The valid Options are '-c' or '-d' or '-i' \n", argv[1]);   
    }
        
    
     exit();
}