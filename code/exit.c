#include "headers.h"
#include "exit.h"

extern deque history;

void EXIT_SHELL()
{
  FILE *fnl_fp = fopen("history.txt", "w");

  while (!isempty(history))
  {
    char hist[500] = "";

    strcpy(hist, front(history));

    pop_front(history);

    if (hist[strlen(hist) - 1] != '\n')
      hist[strlen(hist)] = '\n';

    fprintf(fnl_fp, "%s", hist);
  }

  fclose(fnl_fp);

  exit(0);
}