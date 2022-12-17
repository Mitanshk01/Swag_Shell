#include "headers.h"
#include "tab_handle.h"
#include "prompt.h"

int count_possiblities = 0;

extern int outp_ptr, temp_tab;

struct stat st;

void searcher(char abs_path[], char search_buf[], char store_initial[])
{
  DIR *directory_name;

  struct dirent *file_names;

  if ((directory_name = opendir(abs_path)) == NULL)
  {
    printf("\n");

    printf(RED);
    perror("Invalid directory name");
    printf(RESET);

    return;
  }

  while ((file_names = readdir(directory_name)) != NULL)
  {
    char temp_path[400] = "";

    strcpy(temp_path, abs_path);

    strcat(temp_path, "/");

    strcat(temp_path, file_names->d_name);

    stat(temp_path, &st);

    if (strncmp(search_buf, file_names->d_name, strlen(search_buf)) != 0)
    {
      continue;
    }
    else
    {
      if (count_possiblities == 0)
      {
        count_possiblities++;
        strcpy(store_initial, file_names->d_name);
        if (S_ISDIR(st.st_mode))
          store_initial[strlen(file_names->d_name)] = '/';
        continue;
      }
      else if (count_possiblities == 1)
      {
        count_possiblities++;
        printf("\n");
        printf("%s\n", store_initial);
        fflush(stdout);
      }
      char temp_fl_nm[200] = "";
      strcpy(temp_fl_nm, file_names->d_name);
      if (S_ISDIR(st.st_mode))
        strcat(temp_fl_nm, "/");
      printf("%s", temp_fl_nm);
      for (int i = 0; i < strlen(temp_fl_nm); i++)
      {
        if (temp_fl_nm[i] != store_initial[i])
        {
          for (int j = i; j < strlen(temp_fl_nm); j++)
            store_initial[j] = '\0';
          break;
        }
      }
      printf("\n");
    }
  }
}

char *tab_handler(char command[], char home_path[])
{
  temp_tab = 0;

  char *store_initial;

  store_initial = (char *)malloc(sizeof(char) * 200);

  for (int i = 0; i < 200; i++)
    store_initial[i] = '\0';

  count_possiblities = 0;

  char directory[500] = "", search_file[500] = "";

  char cur_dir[500];

  int flg = 0;

  getcwd(cur_dir, 500);

  if (strlen(command) == 0)
  {
    getcwd(directory, 500);
  }

  else if (command[0] == '/')
  {
    strcpy(directory, command);
    flg = 1;
    if (stat(directory, &st) != -1)
    {
      if (S_ISDIR(st.st_mode))
        directory[strlen(directory)] = '/';
    }
  }

  else if (strcmp(command, "~") == 0)
    strcpy(directory, home_path);

  else if (strncmp(command, "~", 1) == 0)
  {
    strcpy(directory, home_path);

    strcat(directory, "/");

    for (int i = 2; i < strlen(command); i++)
      directory[strlen(directory)] = command[i];

    flg = 1;
  }

  else
  {
    strcpy(directory, cur_dir);

    strcat(directory, "/");

    strcat(directory, command);

    flg = 1;
  }

  char search_buf[100] = "", dir_buf[100] = "";

  if (flg)
  {
    int pos_bfr = -1;

    for (int i = strlen(directory) - 1; i >= 0; i--)
    {
      if (directory[i] == '/')
      {
        pos_bfr = i;
        break;
      }
    }

    strncpy(dir_buf, directory, pos_bfr + 1);

    for (int i = pos_bfr + 1; i < strlen(directory); i++)
      search_buf[i - pos_bfr - 1] = directory[i];
  }
  else
    strcpy(dir_buf, directory);

  temp_tab = strlen(search_buf);

  searcher(dir_buf, search_buf, store_initial);

  return store_initial;
}