#include "headers.h"
#include "jobs.h"

extern int proc_cnt;

int struct_comparator(const void *a, const void *b)
{
  const bg_struct temp1 = *(bg_struct *)a;
  const bg_struct temp2 = *(bg_struct *)b;

  return strcmp(temp1.proc, temp2.proc);
}

void jobs()
{
  if (proc_cnt == 0)
    return;

  for (int i = 0; i < proc_cnt; i++)
  {
    temp_store[i].pid = bg_store[i].pid;

    strcpy(temp_store[i].proc, bg_store[i].proc);

    temp_store[i].proc_num = bg_store[i].proc_num;

    temp_store[i].stopped = bg_store[i].stopped;
  }

  qsort(temp_store, proc_cnt, sizeof(bg_struct), struct_comparator);

  for (int i = 0; i < proc_cnt; i++)
  {
    if (!temp_store[i].stopped)
    {
      int garbvar1;

      char temp_path[500], garbvar2[100], process_status;

      sprintf(temp_path, "/proc/%d/stat", temp_store[i].pid);

      FILE *fl = fopen(temp_path, "r");

      fscanf(fl, "%d %s %c", &garbvar1, garbvar2, &process_status);

      if (job_level >= 2)
      {
        printf("[%d] %s %s [%d]\n", temp_store[i].proc_num + 1, process_status == 'T' ? "Stopped" : "Running", temp_store[i].proc, temp_store[i].pid);
      }
      else if (job_level == 1)
      {
        printf("[%d] %s %s [%d]\n", temp_store[i].proc_num + 1, process_status == 'T' ? "Stopped" : "Running", temp_store[i].proc, temp_store[i].pid);
      }
    }
  }
}