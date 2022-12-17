#include "headers.h"
#include "pinfo.h"

void pinfo(char command[], char home_path[])
{
  char *token = strtok(command, " \n\t");

  token = strtok(NULL, " \n\t");

  int pid = getpid();

  int npid = getpid();

  if (token != NULL)
    pid = atoi(token);

  struct rusage vmem_usage;

  // Handle for different pid

  char exec_path[500] = "", temp_path[500] = "", vm_path[500] = "", p_path[500], act_path[500];

  sprintf(temp_path, "/proc/%d/exe", pid);

  sprintf(vm_path, "/proc/%d/statm", pid);

  sprintf(act_path, "/proc/%d/stat", npid);

  FILE *fl = fopen(vm_path, "r");

  long mem_size;

  int garbvar1, garbvar3, proc_grp, act_grp;

  char garbvar2[1000], process_status, garbvar4;

  sprintf(p_path, "/proc/%d/stat", pid);

  FILE *fl1 = fopen(p_path, "r");

  fscanf(fl1, "%d %s %c %d %d %d %d %d", &garbvar1, garbvar2, &process_status, &garbvar1, &act_grp, &garbvar3, &garbvar3, &proc_grp);

  fscanf(fl, "%ld", &mem_size);

  readlink(temp_path, exec_path, 500);

  printf("pid : %d\n", pid);

  if (process_status == 'Z')
    printf("process Status:Z\n");

  else if (process_status == 'S')
  {
    if (proc_grp == act_grp)
      printf("process Status:S\n");
    else
      printf("process Status:S+\n");
  }
  else if (process_status == 'R')
  {
    if (proc_grp == act_grp)
      printf("process Status:R\n");
    else
      printf("process Status:R+\n");
  }
  else
    printf("process Status:%c\n", process_status);

  printf("memory : %ld {Virtual Memory}\n", mem_size);

  if (strncmp(home_path, exec_path, strlen(home_path)) != 0)
    printf("executable path : %s\n", exec_path);

  else
  {
    char final_exec_path[500] = "";

    final_exec_path[0] = '~';

    for (int i = strlen(home_path); i < strlen(exec_path); i++)
      final_exec_path[i - strlen(home_path) + 1] = exec_path[i];

    printf("executable path : %s\n", final_exec_path);
  }
}