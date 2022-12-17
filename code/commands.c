#include "headers.h"
#include "echo.h"
#include "pwd.h"
#include "cd.h"
#include "pinfo.h"
#include "ls.h"
#include "discover.h"
#include "foreground.h"
#include "background.h"
#include "commands.h"
#include "jobs.h"
#include "sig.h"
#include "fg.h"
#include "bg.h"

extern int com_lev, proc_cnt, io_level, inv_command, run_existing_bg;

void command_exec(char command[], char home_path[], char prev_path[])
{
  char temp_command[500] = "", current_directory[500], brk_command[500] = "";

  getcwd(current_directory, 500);

  int cnt = 0;

  for (int j = 0; j < strlen(command); j++)
  {
    int checker = 0;

    if (command[j] == '>')
    {
      checker = 1;

      if (command[j + 1] == '>')
      {
        int testbreak = 0;

        io_level |= 2;

        char temp_file[500] = "";

        for (int k = j + 2; k < strlen(command); k++)
        {
          if (command[k] != ' ')
            testbreak = 1;

          temp_file[k - j - 2] = command[k];

          if (testbreak && command[k] == ' ')
          {
            j = k - 1;
            break;
          }
          else if (k == strlen(command) - 1)
            j = k;
        }

        char *fltok = strtok(temp_file, " \n\t");

        append_fd = open(fltok, O_CREAT | O_WRONLY | O_APPEND, 0644);

        dup2(append_fd, 1);

        j++;
      }
      else
      {
        io_level |= 6;

        int testbreak = 0;

        char temp_file[500] = "";

        for (int k = j + 1; k < strlen(command); k++)
        {
          if (command[k] != ' ')
            testbreak = 1;

          temp_file[k - j - 1] = command[k];

          if (testbreak && command[k] == ' ')
          {
            j = k - 1;
            break;
          }
          else if (k == strlen(command) - 1)
            j = k;
        }

        char *fltok = strtok(temp_file, " \n\t");

        outp_fd = open(fltok, O_CREAT | O_WRONLY | O_TRUNC, 0644);

        dup2(outp_fd, 1);
      }
    }
    else if (command[j] == '<')
    {
      checker = 1;

      int testbreak = 0;

      io_level |= 1;

      char temp_file[500] = "";

      for (int k = j + 1; k < strlen(command); k++)
      {
        if (command[k] != ' ')
          testbreak = 1;

        temp_file[k - j - 1] = command[k];

        if (testbreak && command[k] == ' ')
        {
          j = k - 1;
          break;
        }
        else if (k == strlen(command) - 1)
          j = k;
      }

      char *fltok = strtok(temp_file, " \n\t");

      inp_fd = open(fltok, O_RDONLY, 0644);

      if (inp_fd == -1)
      {
        printf(RED);
        printf("Invalid Input file!\n");
        printf(RESET);
        inv_command = 1;
        return;
      }

      else
        dup2(inp_fd, 0);
    }

    if (!checker)
      temp_command[cnt++] = command[j];
  }

  char *token = strtok(command, " \n\t");

  if (token == NULL)
    return;

  if (strcmp("cd", token) == 0)
  {
    int ret_val = cd(temp_command, home_path, prev_path);
    if (ret_val != -1)
      strcpy(prev_path, current_directory);
  }

  // pwd command

  else if (strcmp("pwd", token) == 0)
  {
    pwd();
  }

  // echo command

  else if (strcmp("echo", token) == 0)
  {
    echo(temp_command);
  }

  // ls command
  else if (strcmp("ls", token) == 0)
  {
    ls(temp_command, home_path);
  }

  // pinfo command
  else if (strcmp("pinfo", token) == 0)
  {
    pinfo(temp_command, home_path);
  }

  // discover command

  else if (strcmp("discover", token) == 0)
  {
    discover(temp_command, home_path);
  }

  // history command

  else if (strcmp("history", token) == 0)
  {
    int history_num = 10;

    token = strtok(NULL, " \n\t");

    if (token != NULL)
    {
      printf("\x1b[31mhistory: too many arguments\n");

      return;
    }

    if (history_num >= history->numitems)
      for (int i = 0; i < history->numitems; i++)
        printf("%s", history->arr[(history->front + i) % 20]);

    else
      for (int i = history->numitems - 10; i < history->numitems; i++)
        printf("%s", history->arr[(history->front + i) % 20]);
  }

  // jobs

  else if (strcmp("jobs", token) == 0)
  {
    token = strtok(NULL, " \n\t");

    job_level = 0;

    while (token != NULL)
    {
      if (strcmp(token, "-r") == 0)
        job_level |= 1;
      else if (strcmp(token, "-s") == 0)
        job_level |= 2;
      else
      {
        job_level = -1;
        break;
      }

      token = strtok(NULL, " \n\t");
    }

    if (job_level == -1)
    {
      printf(RED);
      printf("Invalid Command!\n");
      printf(RESET);
      return;
    }
    else
    {
      if (!job_level)
        job_level = 3;

      jobs();
    }
  }

  // sig

  else if (strcmp("sig", token) == 0)
  {
    token = strtok(NULL, " \n\t");

    int comm = 0, pno = -1, signo = -1;

    while (token != NULL)
    {
      if (comm == 0)
        pno = atoi(token);

      else if (comm == 1)
        signo = atoi(token);

      token = strtok(NULL, " \n\t");

      comm++;
    }

    if (comm != 2)
    {
      printf(RED);
      printf("Invalid Command!\n");
      printf(RESET);

      return;
    }

    sig(pno, signo);
  }

  // fg

  else if (strcmp("fg", token) == 0)
  {

    token = strtok(NULL, " \n\t");

    int comm = 0, parameter = -1;

    while (token != NULL)
    {
      if (comm == 0)
        parameter = atoi(token);

      comm++;

      token = strtok(NULL, " \n\t");
    }
    if (comm != 1)
    {
      printf(RED);
      printf("Invalid Command!\n");
      printf(RESET);

      return;
    }

    fg(parameter);
  }

  // bg

  else if (strcmp("bg", token) == 0)
  {
    token = strtok(NULL, " \n\t");

    int comm = 0, parameter = -1;

    while (token != NULL)
    {
      if (comm == 0)
        parameter = atoi(token);

      comm++;

      token = strtok(NULL, " \n\t");
    }

    if (comm != 1)
    {
      printf(RED);
      printf("Invalid Command!\n");
      printf(RESET);

      return;
    }

    bg(parameter);
  }

  // foreground processing

  else if (com_lev == 0)
  {
    fore_g(temp_command);
  }

  // background processing

  else
  {
    char temp_comm_bg[500] = "";

    int pos = 0;

    while (temp_command[pos] == ' ')
      pos++;

    for (int k = pos; k < strlen(temp_command); k++)
      temp_comm_bg[k - pos] = temp_command[k];

    int setter = 0;

    if (run_existing_bg == -1)
      strcpy(bg_store[proc_cnt].proc, temp_comm_bg);
    else
    {
      strcpy(bg_store[run_existing_bg - 1].proc, temp_comm_bg);
      setter = 1;
    }

    back_g(temp_command);

    if (!setter)
      proc_cnt++;
  }

  if (io_level)
  {
    if (io_level == 1 || io_level == 3 || io_level == 7)
      close(inp_fd);

    if (io_level == 6 || io_level == 7)
      close(outp_fd);

    if (io_level == 2 || io_level == 3)
      close(append_fd);

    io_level = 0;
  }
}