#include "headers.h"
#include "commands.h"
#include "piping.h"
#include "prompt.h"

extern int inp_ptr, outp_ptr;

void piper(char command[], char home_path[], char prev_path[], int pipe_count)
{
  char *token1 = strtok(command, "|");

  for (int i = 0; i <= pipe_count; i++)
  {
    int piper[2];

    pipe(piper);

    pid_t PID = fork();

    if (PID == 0)
    {
      close(piper[0]);

      dup2(piper[1], 1);

      if (i == pipe_count)
      {
        dup2(outp_ptr, 1);
      }

      command_exec(token1, home_path, prev_path);

      close(piper[1]);

      dup2(inp_ptr, 0);

      exit(0);
    }

    int pp_status = 0;

    waitpid(PID, &pp_status, WUNTRACED);

    close(piper[1]);

    if (i != pipe_count)
      dup2(piper[0], 0);

    close(piper[0]);

    token1 = strtok(NULL, "|");
  }
}