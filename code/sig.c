#include "headers.h"
#include "sig.h"

extern int proc_cnt;

void sig(int pno, int signo)
{
  if (proc_cnt < pno)
  {
    printf(RED);
    printf("Invalid job number\n");
    printf(RESET);

    return;
  }

  int r_val = kill(bg_store[pno - 1].pid, signo);

  if (r_val < 0)
  {
    printf(RED);
    printf("Error!");
    printf(RESET);
  }
}