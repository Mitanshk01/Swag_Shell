#include "headers.h"
#include "pwd.h"

void pwd()
{
  char current_directory[500];

  getcwd(current_directory, 500);

  printf("%s\n", current_directory);
}