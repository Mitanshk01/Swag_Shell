#include "headers.h"
#include "cd.h"

int cd(char command[], char home_path[], char prev_path[])
{
  char *token = strtok(command, " \n\t");

  char current_directory[500];

  getcwd(current_directory, 500);

  token = strtok(NULL, " \n\t");

  if (token == NULL)
    chdir(home_path);

  else
  {
    if (strcmp(token, ".") == 0)
    {
      token = strtok(NULL, " \n\t");

      if (token == NULL)
        chdir(current_directory);

      else
        printf("\x1b[31mcd: too many arguments");
    }

    else if (strcmp(token, "..") == 0)
    {
      token = strtok(NULL, " \n\t");

      if (token == NULL)
        chdir("..");

      else
        printf("\x1b[31mcd: too many arguments");
    }

    else if (strcmp(token, "-") == 0)
    {
      token = strtok(NULL, " \n\t");

      if (token == NULL)
      {
        if (strlen(prev_path) != 0)
        {
          chdir(prev_path);

          printf("%s\n", prev_path);
        }

        else
        {
          printf("\x1b[31mcd: OLDPWD not set\n");
          return -1;
        }
      }

      else
      {
        printf("\x1b[31mcd: too many arguments");
        return -1;
      }
    }

    else if (strcmp(token, "~") == 0)
    {
      token = strtok(NULL, " \n\t");

      if (token == NULL)
        chdir(home_path);

      else
      {
        printf("\x1b[31mcd: too many arguments");
        return -1;
      }
    }

    else if (strncmp(token, "~/", 2) == 0)
    {
      char temp_path_final[200] = "";

      strcpy(temp_path_final, token);

      token = strtok(NULL, " \n\t");
      if (token == NULL)
      {
        char temp_path[500] = "";
        strcpy(temp_path, home_path);
        for (int i = 1; i < strlen(temp_path_final); i++)
          temp_path[strlen(home_path) + i - 1] = temp_path_final[i];
        chdir(temp_path);
      }

      else
      {
        printf("\x1b[31mcd: too many arguments");
        return -1;
      }
    }

    else
    {
      if (chdir(token) == -1)
      {
        printf("\x1b[31mcd: %s: No such file or directory\n", token);
        return -1;
      }
    }
  }
  return 0;
}