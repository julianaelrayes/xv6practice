#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

#define BUF_SIZE 80
#define O_RDONLY  0x000
int
strcmp(const char *p, const char *q)
{
  while(*p && *p == *q)
    p++, q++;
  return (uchar)*p - (uchar)*q;
}

char*
strcpy(char *s, const char *t)
{
  char *os;

  os = s;
  while((*s++ = *t++) != 0)
    ;
  return os;
}
int readline(struct inode *ip, char *buf,int startPosition, int max_len) {
    int total = readi(ip,buf,startPosition,BUF_SIZE);
    //cprintf("Total Bytes Read:%d\n",total);
    if (total < 0)
       return -1;
    if (total < sizeof(buf)) 
        buf[total] = '\0';
    int index=0;
    for (; index < strlen(buf);index++)
      if (buf[index] == '\n') { 
          buf[index] = '\0';
          break;
      }

    return index;
}
void toLower(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] >= 'A' && str[i] <= 'Z') {
            // Convert uppercase character to lowercase by adding ASCII offset
            str[i] = str[i] + ('a' - 'A');
        }
    }
}
int getline_array(int offset, char *data, char *line) {
    int arrayIndex = 0;
    int index = offset;
    
    while (data[index] != '\0' && data[index] != '\n') {
        line[arrayIndex] = data[index];
        arrayIndex++;
        index++;
    }
    
    if (data[index] == '\n') {
        // Include the newline character in the line
        line[arrayIndex] = data[index];
        arrayIndex++;
        index++;
    }
    
    line[arrayIndex] = '\0'; // Null-terminate the line
    if (data[index] == '\0') {
        return -1; // Signal that there are no more lines
    }
    
    return index; // Move index to the next character
}

int sys_uniq_ks(void) {
  char *filename;
  char *flag;
  
  if(argstr(0, &filename) < 0 || argstr(1, &flag) < 0 )
    return -1;

  int lineCount=1;
  int bytesRead = 0;
  int  indexCounter=0;

  char current_line[BUF_SIZE]="";
  char previous_line[BUF_SIZE]="";
  char previous_line_1[80]="";
  char current_line_1[80]="";
  //cprintf("%s\t%s\n",filename,flag); 
  if (strcmp(flag,"x") == 0) {
      char buffer[1024];
      char c;
      struct file *f = myproc()->ofile[0]; // Access stdin
     
      int inbytes = -1;
      int i = 0;
      while ( (inbytes = fileread(f, &c,1)) > 0) 
      {
        if (inbytes < 0)
          break;
        else
          buffer[i++] = c;  
      }
      buffer[i] = '\0';
      //cprintf("%s\n\n************************************** Parsing**********************\n",buffer);
      int offset = 0;
      
      //char current_line[BUF_SIZE]="";
      //char previous_line[BUF_SIZE]="";
      //char previous_line_1[80]="";
      //char current_line_1[80]="";

      while ((offset = getline_array(offset,buffer,current_line)) > 0) 
      {
          if (offset < 0) {
            break;
          }
          
          if (strcmp(previous_line,"") == 0) 
          {
              strcpy(previous_line,current_line); 
              lineCount=1;
          } 
          else if (strcmp(current_line, previous_line) != 0) 
          {
              cprintf("%s\n",previous_line);
              strcpy(previous_line,current_line);
              lineCount=1;
          }  
          else 
              lineCount = lineCount + 1;
          offset++;
      }
      if (strcmp(previous_line,"") != 0 ) 
      {
          cprintf("%s\n",previous_line);
      }
      else 
      {
          cprintf("\n");
      } 
  }
  else 
  {
      begin_op();
          struct inode *ip;
          if ((ip=namei(filename)) == 0) {
            end_op();
            cprintf("File Not Found ....\n");
            return -1; //File Not Found
          }
          ilock(ip);
              
              while ( (bytesRead = readline(ip,current_line ,indexCounter,BUF_SIZE)) > 0) 
              {
                  if (bytesRead < 0)
                    break;
                  indexCounter = indexCounter + bytesRead + 1;
                  //cprintf("Index Counter : %d\n",indexCounter);
                  if (strcmp(previous_line,"") == 0) {
                      strcpy(previous_line,current_line); 
                      lineCount=1;
                  } 
                  else if (strcmp(flag,"-i") == 0) {
                      strcpy(current_line_1,current_line); 
                      strcpy(previous_line_1,previous_line); 
                      toLower(current_line_1);
                      toLower(previous_line_1);
                      if (strcmp(current_line_1, previous_line_1) != 0) {
                          cprintf("%s\n",previous_line);
                          strcpy(previous_line,current_line);
                      }
                  }
                  else if (strcmp(current_line, previous_line) != 0) {
                      if (strcmp(flag,"-c") == 0) 
                          cprintf("%d\t%s\n",lineCount,previous_line);
                      else if ( (strcmp(flag,"-d") == 0 && lineCount >1 ) ||  (strcmp(flag,"-g") == 0))
                          cprintf("%s\n",previous_line);

                      strcpy(previous_line,current_line);
                      lineCount=1;
                  }  
                  else 
                      lineCount = lineCount + 1;
              }

              if (strcmp(previous_line,"") != 0 ) 
              {
                  if (strcmp(flag,"-c") == 0)
                      cprintf("%d\t%s\n",lineCount,previous_line);
                  else if ( (strcmp(flag,"-d") == 0 && lineCount >1 ) 
                        ||  (strcmp(flag,"-i") == 0) ||  (strcmp(flag,"-g") == 0))
                      cprintf("%s\n",previous_line);
              }
            else 
            {
                cprintf("\n");
            }
          iunlock(ip);
      end_op();
  }
  return 0;
}


//********************************************Above Methods are added for Assignment 611 ********************************************
int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}
