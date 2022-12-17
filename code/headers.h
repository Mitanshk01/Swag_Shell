#include <stdio.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/dir.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <libgen.h>
#include <grp.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
#include <ctype.h>
#include <termios.h>
#include "history.h"
#define RED "\x1b[31m"
#define GREEN "\x1b[32m"
#define YELLOW "\x1b[33m"
#define BLUE "\x1b[34m"
#define MAGENTA "\x1b[35m"
#define CYAN "\x1b[36m"
#define RESET "\x1b[0m"

deque history;

int io_level, inp_fd, outp_fd, append_fd, job_level;

typedef struct bg_struct
{
  char proc[100];
  int pid;
  int proc_num;
  int stopped;
} bg_struct;

bg_struct bg_store[1000], temp_store[1000];
