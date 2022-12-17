#include "headers.h"
#include "discover.h"

int command_level = 0;

struct stat st;

void dfs(char rel_name[], char abs_path[], char search_file[])
{
  DIR *directory_name;

  struct dirent *file_names;

  if ((directory_name = opendir(abs_path)) == NULL)
  {
    perror("Invalid directory name");

    return;
  }

  while ((file_names = readdir(directory_name)) != NULL)
  {
    char temp_rel_name[500], temp_abs_path[500];

    strcpy(temp_rel_name, rel_name);

    strcpy(temp_abs_path, abs_path);

    strcat(temp_rel_name, "/");

    strcat(temp_abs_path, "/");

    strcat(temp_rel_name, file_names->d_name);

    strcat(temp_abs_path, file_names->d_name);

    stat(temp_abs_path, &st);

    if ((command_level == 2 || command_level == 3) && S_ISDIR(st.st_mode))
    {
      if (strlen(search_file) == 0 || strcmp(search_file, file_names->d_name) == 0)
        printf("%s\n", temp_rel_name);
    }

    else if ((command_level == 1 || command_level == 3) && !S_ISDIR(st.st_mode))
      if (strlen(search_file) == 0 || strcmp(search_file, file_names->d_name) == 0)
        printf("%s\n", temp_rel_name);

    if (S_ISDIR(st.st_mode) && strncmp(file_names->d_name, ".", 1) != 0 && strncmp(file_names->d_name, "..", 2) != 0)
      dfs(temp_rel_name, temp_abs_path, search_file);
  }
}

void discover(char command[], char home_path[])
{
  command_level = 0;

  char *token = strtok(command, " \n\t"), directory[500] = "", search_file[500] = "";

  char cur_dir[500];

  getcwd(cur_dir, 500);

  token = strtok(NULL, " \n\t");

  if (token == NULL)
  {
    getcwd(directory, 500);
  }
  else
  {
    while (token != NULL)
    {
      if (strcmp(token, "-f") == 0)
        command_level |= 1;

      else if (strcmp(token, "-d") == 0)
        command_level |= 2;

      else if (strncmp("\"", token, 1) == 0)
      {
        for (int i = 1; i < strlen(token) - 1; i++)
          search_file[i - 1] = token[i];
      }

      else
      {
        if (token[0] == '/')
          strcpy(directory, token);

        else if (strcmp(token, ".") == 0)
          getcwd(directory, 500);

        else if (strcmp(token, "~") == 0)
          strcpy(directory, home_path);

        else if (strcmp(token, "..") == 0)
        {
          int idx = -1;

          for (int i = strlen(cur_dir) - 1; i >= 0; i--)
            if (cur_dir[i] == '/')
            {
              idx = i;
              break;
            }

          strncpy(directory, cur_dir, idx);
        }

        else
        {
          strcpy(directory, cur_dir);

          strcat(directory, "/");

          strcat(directory, token);
        }
      }

      token = strtok(NULL, " \n\t");
    }

    if (strcmp(directory, "") == 0)
      getcwd(directory, 500);
  }

  if (command_level == 0)
    command_level = 3;

  dfs(".", directory, search_file);
}
