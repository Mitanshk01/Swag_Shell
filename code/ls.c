#include "headers.h"
#include "ls.h"
#define sze 1000

void details_function(char file_path[], char file_name[])
{
  struct stat st;

  stat(file_path, &st);

  if (S_ISDIR(st.st_mode))
    printf("d");

  else
    printf("-");

  if (st.st_mode & S_IRUSR)
    printf("r");

  else
    printf("-");

  if (st.st_mode & S_IWUSR)
    printf("w");

  else
    printf("-");

  if (st.st_mode & S_IXUSR)
    printf("x");

  else
    printf("-");

  if (st.st_mode & S_IRGRP)
    printf("r");

  else

    printf("-");

  if (st.st_mode & S_IWGRP)
    printf("w");

  else
    printf("-");

  if (st.st_mode & S_IXGRP)
    printf("x");

  else
    printf("-");

  if (st.st_mode & S_IROTH)
    printf("r");

  else
    printf("-");

  if (st.st_mode & S_IWOTH)
    printf("w");

  else
    printf("-");

  if (st.st_mode & S_IXOTH)
    printf("x ");

  else
    printf("- ");

  printf("%li ", st.st_nlink);

  struct passwd *userid;

  struct group *grpid;

  userid = getpwuid(st.st_uid);

  grpid = getgrgid(st.st_gid);

  if (userid == NULL)
  {
    perror("USER_ID");
    printf("%d ", st.st_uid);
  }

  else
  {
    printf("%s ", userid->pw_name);
  }

  if (grpid == NULL)
  {
    perror("GROUP_ID");
    printf("%d ", st.st_gid);
  }
  else
  {
    printf("%s ", grpid->gr_name);
  }

  printf("%5ld ", st.st_size);

  struct tm *time_;

  time_t tme = st.st_mtime;

  char strng[500];

  time_ = localtime(&tme);

  strftime(strng, sizeof(strng), "%b %d %R", time_);

  printf("%s ", strng);

  if (S_ISDIR(st.st_mode))
    printf("\x1b[34m");

  else if (st.st_mode & S_IXOTH)
    printf("\x1b[32m");

  printf("%s", file_name);

  printf("\x1b[0m");

  printf("\n");
}

int string_comparator(const void *a, const void *b)
{
  const char **str1 = (const char **)a;
  const char **str2 = (const char **)b;
  return strcmp(*str1, *str2);
}

void ls(char command[], char home_directory[])
{
  struct stat st;

  int inp_file_num = 0;

  char *token, *garb = command;

  char current_directory[500] = "";

  getcwd(current_directory, 500);

  // 0 for ls
  // 1 for ls -a
  // 2 for ls -l
  // 3 for ls -al

  int command_level = 0, num_files = 0;

  char *files[1000], *actual_names[1000];

  for (int i = 0; i < sze; i++)
  {
    files[i] = malloc(sizeof(char) * 500);
    actual_names[i] = malloc(sizeof(char) * 500);
  }

  while ((token = strtok_r(garb, " \t\n", &garb)))
  {
    if (strcmp(token, "ls") == 0)
      continue;
    else if (strcmp(token, "-a") == 0)
      command_level |= 1;
    else if (strcmp(token, "-l") == 0)
      command_level |= 2;
    else if (strcmp(token, "-al") == 0 || strcmp(token, "-la") == 0)
      command_level |= 3;
    else
    {
      inp_file_num++;

      char file_path_info[500] = "", temp_name[500] = "";

      strcpy(temp_name, token);

      if (strcmp(token, ".") == 0)
        getcwd(file_path_info, 500);

      else if (strcmp(token, "..") == 0)
      {
        int num_back = 0;
        for (int i = strlen(current_directory) - 1; i >= 0; i--)
        {
          if (current_directory[i] == '/')
          {
            num_back = i;
            break;
          }
        }
        strncpy(file_path_info, current_directory, num_back);
      }

      else if (token[0] == '/')
      {
        strcpy(file_path_info, token);
      }

      else if (strncmp(token, "~", 1) == 0)
      {
        strcpy(file_path_info, current_directory);

        int init_size = strlen(file_path_info);

        for (int j = 1; j < strlen(token); j++)
          file_path_info[init_size + j - 1] = token[j];
      }

      else
      {
        strcpy(file_path_info, current_directory);

        strcat(file_path_info, "/");

        int init_size = strlen(file_path_info);

        for (int j = 0; j < strlen(token); j++)
          file_path_info[init_size + j] = token[j];
      }

      if (stat(file_path_info, &st) == -1)
      {
        printf("\x1b[31mls: cannot access '%s': No such file or directory\n", temp_name);
      }

      else
      {
        strcpy(files[num_files], file_path_info);
        strcpy(actual_names[num_files], temp_name);
        num_files++;
      }
    }
  }

  if (num_files == 0 && inp_file_num == 0)
  {

    char *files_array[1000];

    for (int i = 0; i < sze; i++)
    {
      files_array[i] = (char *)malloc(sizeof(char) * 300);
    }

    DIR *directory_name;

    struct dirent *file_names;

    int cnt_files = 0;

    directory_name = opendir(current_directory);

    while ((file_names = readdir(directory_name)) != NULL)
    {

      strcpy(files_array[cnt_files], file_names->d_name);

      cnt_files++;
    }

    qsort(files_array, cnt_files, sizeof(char *), string_comparator);

    if (command_level <= 1)
      for (int i = 0; i < cnt_files; i++)
      {
        if (strncmp(files_array[i], ".", 1) || command_level == 1)
        {
          stat(files_array[i], &st);

          if (S_ISDIR(st.st_mode))
            printf(BLUE);

          else if (st.st_mode & S_IXOTH)
            printf(GREEN);

          else
            printf("\x1b[0m");

          printf("%s\n", files_array[i]);

          printf("\x1b[0m");
        }
      }
    else
    {
      int num_block = 0;

      for (int i = 0; i < cnt_files; i++)
        if (strncmp(files_array[i], ".", 1) || command_level == 3)
        {
          stat(files_array[i], &st);
          num_block += st.st_blocks;
        }

      printf("total %d\n", num_block / 2);

      for (int i = 0; i < cnt_files; i++)
      {
        char temp_path_n[500];

        strcpy(temp_path_n, current_directory);

        strcat(temp_path_n, "/");

        strcat(temp_path_n, files_array[i]);

        if (strncmp(files_array[i], ".", 1) || command_level == 3)
          details_function(temp_path_n, files_array[i]);
      }
    }
  }

  for (int i = 0; i < num_files; i++)
  {
    struct stat st;

    stat(files[i], &st);

    if (num_files != 1)
      printf("%s:\n", files[i]);

    if (S_ISDIR(st.st_mode))
    {
      char *files_array[1000];

      for (int j = 0; j < sze; j++)
      {
        files_array[j] = (char *)malloc(sizeof(char) * 300);
      }

      DIR *directory_name;

      struct dirent *file_names;

      int cnt_files = 0;

      directory_name = opendir(files[i]);

      while ((file_names = readdir(directory_name)) != NULL)
      {

        strcpy(files_array[cnt_files], file_names->d_name);

        cnt_files++;
      }

      qsort(files_array, cnt_files, sizeof(char *), string_comparator);

      if (command_level <= 1)
        for (int j = 0; j < cnt_files; j++)
        {
          struct stat new_st;

          char temp_nm_fl[500];

          strcpy(temp_nm_fl, files[i]);

          strcat(temp_nm_fl, "/");

          strcat(temp_nm_fl, files_array[j]);

          stat(temp_nm_fl, &new_st);

          // printf("%s\n", files_array[j]);

          if (strncmp(files_array[j], ".", 1) || command_level == 1)
          {
            if (S_ISDIR(new_st.st_mode))
            {
              printf(BLUE);
            }

            else if (new_st.st_mode & S_IXOTH)
              printf(GREEN);

            else
              printf(RESET);

            printf("%s\n", files_array[j]);

            printf(RESET);
          }
        }
      else
      {
        int num_block = 0;

        for (int j = 0; j < cnt_files; j++)
        {
          char temp_path_n[500];
          strcpy(temp_path_n, files[i]);
          strcat(temp_path_n, "/");
          strcat(temp_path_n, files_array[j]);
          stat(temp_path_n, &st);
          if (strncmp(files_array[j], ".", 1) || command_level == 3)
          {
            num_block += st.st_blocks;
          }
        }
        printf("total %d\n", num_block / 2);
        for (int j = 0; j < cnt_files; j++)
        {
          char temp_path_n[500];
          strcpy(temp_path_n, files[i]);
          strcat(temp_path_n, "/");
          strcat(temp_path_n, files_array[j]);
          if (strncmp(files_array[j], ".", 1) || command_level == 3)
          {
            details_function(temp_path_n, files_array[j]);
          }
        }
      }
    }
    else
    {
      if (command_level <= 1)
      {
        stat(files[i], &st);

        if (S_ISDIR(st.st_mode))
          printf(BLUE);

        else if (st.st_mode & S_IXOTH)
          printf(GREEN);

        printf("%s\n", files[i]);

        printf(RESET);
      }
      else
      {
        char temp_path_n[500];
        strcpy(temp_path_n, files[i]);
        if (files[i][0] != '/')
        {
          strcat(temp_path_n, "/");
          strcat(temp_path_n, files[i]);
        }
        details_function(temp_path_n, actual_names[i]);
      }
    }

    if (i != num_files - 1)
      printf("\n");
  }
}