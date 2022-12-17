#include "headers.h"
#include "commands.h"

extern int proc_cnt, com_lev;

int run_existing_bg = -1;

void bg(int parameter)
{
  if (proc_cnt < parameter)
  {
    printf(RED);
    printf("Invalid job number\n");
    printf(RESET);

    return;
  }

  int r_val = kill(bg_store[parameter - 1].pid, SIGCONT);

  if (r_val < 0)
  {
    printf(RED);
    printf("Error!");
    printf(RESET);
  }
}