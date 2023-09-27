#include "types.h"
#include "stat.h"
#include "user.h"

int
main(int argc, char *argv[])
{
  if(argc < 2) {
    printf(2, "Usage: new_wait command\n");
    exit();
  }

  int pid = fork();
  if(pid == -1) printf(2, "Fork Error\n");
  else if(pid == 0)
  {
    if(exec(argv[1], argv + 1) == -1) printf(2, "No such command\n");
    exit();
  }
  else
  {
    int create_time, stop_time, run_time;
    new_wait(&create_time, &stop_time, &run_time);
    printf(1, "\nCreation Time : %d\nStop Time : %d\nRun Time : %d\n", create_time, stop_time, run_time);
  }

  exit();
}

// whole file added ** changed