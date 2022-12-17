#include "headers.h"
#include "background.h"

extern int proc_cnt, globalpid;

void back_g(char command[])
{
  pid_t bg_pid = fork();

  // Not making it to stop,check

  if (bg_pid != 0)
  {
    printf("[%d] %d\n", proc_cnt + 1, bg_pid);

    bg_store[proc_cnt].pid = bg_pid;

    bg_store[proc_cnt].proc_num = proc_cnt;

    bg_store[proc_cnt].stopped = 0;
  }

  else
  {
    setpgid(0, 0);

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
    {
      printf("\x1b[31m%s: command not found\n", command);
      exit(1);
    }
  }
}

void handle_dead()
{
  setbuf(stdout, NULL);
  int check = 0, status = 0;

  int temp_pid = waitpid(-1, &status, WNOHANG | WUNTRACED);

  for (int i = 0; i < proc_cnt; i++)
  {
    if (bg_store[i].pid == temp_pid)
    {
      printf("\n%s with pid = %d exited %s", bg_store[i].proc, bg_store[i].pid, WIFEXITED(status) ? "normally" : "abnormally");
      fflush(stdout);
      printf("\n");
      bg_store[i].stopped = 1;
    }
    if (temp_store[i].pid == temp_pid)
      temp_store[i].stopped = 1;

    if (bg_store[i].stopped != 1)
      check = 1;
  }

  if (!check)
    proc_cnt = 0;
}
