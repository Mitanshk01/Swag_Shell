#include "headers.h"
#include "commands.h"

extern int proc_cnt, com_lev;

void fg(int parameter)
{
  if (proc_cnt < parameter)
  {
    printf(RED);
    printf("Invalid job number\n");
    printf(RESET);

    return;
  }

  signal(SIGTTIN, SIG_IGN);
  signal(SIGTTOU, SIG_IGN);

  proc_cnt--;

  tcsetpgrp(0, bg_store[parameter - 1].pid);

  kill(bg_store[parameter - 1].pid, SIGCONT);

  waitpid(-1, NULL, WUNTRACED);

  tcsetpgrp(0, getpgid(0));

  signal(SIGTTOU, SIG_DFL);
  signal(SIGTTIN, SIG_DFL);
}