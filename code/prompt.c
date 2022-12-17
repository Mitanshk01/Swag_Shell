#include "prompt.h"
#include "headers.h"
#include "background.h"
extern int time_fg, inp_ptr, outp_ptr;

extern struct sigaction checker;

void prompt(char home_path[])
{
    // Making the Shell prompt

    dup2(inp_ptr, 0);
    dup2(outp_ptr, 1);

    struct utsname usr_name;

    char current_directory[500] = "", direct[200] = "", command[200] = "";

    getcwd(current_directory, 500);

    if (strncmp(current_directory, home_path, strlen(home_path)) == 0)
    {
        direct[0] = '~';
        for (int i = strlen(home_path); i < strlen(current_directory); i++)
            direct[i - strlen(home_path) + 1] = current_directory[i];
    }
    else
        strcpy(direct, current_directory);

    if (uname(&usr_name) == -1)
    {
        perror("uname");
        exit(-1);
    }

    char user_name[100], system_name[100];

    // If error in getting user_name,exit the program
    if (getlogin_r(user_name, 100) == -1)
    {
        perror("user_name");
        exit(-1);
    }

    if (gethostname(system_name, 100) == -1)
    {
        perror("system_name");
        exit(-1);
    }

    // signal(SIGCHLD, handle_dead);

    // struct sigaction oldp;

    printf("\x1b[32m");
    printf("\r<%s@%s:", user_name, system_name);
    printf("\x1b[34m");
    printf("%s", direct);

    if (time_fg >= 1)
    {
        printf(" took %d seconds", time_fg);
        time_fg = 0;
    }
    printf(">");
    printf("\x1b[0m");
}
