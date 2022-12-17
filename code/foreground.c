#include "headers.h"
#include "foreground.h"
#include "commands.h"

extern int time_fg, proc_cnt;

extern int Ctrl_C, com_lev, Ctrl_Z;

extern char home_path[500], prev_path[500];

extern int is_fg;

char temp_com[500];

pid_t globalpid = -1;

void fore_g(char command[])
{
  double start_tm = time(NULL);

  pid_t child_pid = fork();

  globalpid = child_pid;

  strcpy(temp_com, command);

  signal(SIGTSTP, SIG_IGN);

  signal(SIGINT, SIG_DFL);

  if (child_pid == 0)
  {
    signal(SIGTSTP, SIG_DFL);

    setpgrp();

    char temp_tok[500], *arr_args[100];

    for (int i = 0; i < 100; i++)
      arr_args[i] = (char *)malloc(sizeof(char) * 100);

    char *ntok = strtok(command, " \n\t");

    int cnt_files = 1;

    strcpy(temp_tok, ntok);

    strcpy(arr_args[0], ntok);

    while ((ntok = strtok(NULL, " \n\t")) != NULL)
    {
      strcpy(arr_args[cnt_files], ntok);

      cnt_files++;
    }

    arr_args[cnt_files] = NULL;

    if (execvp(temp_tok, arr_args) == -1)
      printf("\x1b[31m%s: command not found\n", command);

    exit(0);
  }
  else
  {
    tcsetpgrp(STDIN_FILENO, globalpid);

    int status = 0;

    waitpid(child_pid, &status, WUNTRACED);

    if (WIFSTOPPED(status))
    {
      char temp_comm_bg[500];

      int pos = 0;

      while (temp_com[pos] == ' ')
        pos++;

      for (int k = pos; k < strlen(temp_com); k++)
        temp_comm_bg[k - pos] = temp_com[k];

      printf("[%d] %d\n", proc_cnt + 1, globalpid);

      bg_store[proc_cnt].pid = globalpid;

      bg_store[proc_cnt].proc_num = proc_cnt;

      strcpy(bg_store[proc_cnt].proc, temp_comm_bg);

      bg_store[proc_cnt].stopped = 0;

      proc_cnt++;
    }

    tcsetpgrp(STDIN_FILENO, getpid());

    double end_tm = time(NULL);

    time_fg = end_tm - start_tm;
  }
  globalpid = -1;
}