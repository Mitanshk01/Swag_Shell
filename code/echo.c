#include "headers.h"
#include "echo.h"

void echo(char command[])
{
  char *token = strtok(command, " \n\t");
  token = strtok(NULL, " \n\t");
  while (token != NULL)
  {
    printf("%s ", token);
    token = strtok(NULL, " \n\t");
  }
  printf("\n");
}