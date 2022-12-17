#include "prompt.h"
#include "headers.h"
#include "exit.h"
#include "history.h"
#include "background.h"
#include "commands.h"
#include "piping.h"
#include "tab_handle.h"

extern int count_possiblities;

int temp_tab = 0;

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
        die("tcsetattr");
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
        die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
        die("tcsetattr");
}

int time_fg = 0, inp_ptr, outp_ptr, com_lev, proc_cnt = 0, inv_command;

char home_path[500] = "", prev_path[500] = "", current_directory[500];

struct sigaction checker;

int main()
{
    signal(SIGINT, SIG_IGN);

    signal(SIGTSTP, SIG_IGN);

    signal(SIGTTIN, SIG_IGN);

    signal(SIGTTOU, SIG_IGN);

    checker.sa_handler = handle_dead;

    sigemptyset(&checker.sa_mask);

    checker.sa_flags = SA_NOCLDSTOP;

    inp_ptr = dup(0);

    outp_ptr = dup(1);

    printf("####\n");
    printf("####\n");
    printf("Please type \"EXIT\" to exit the shell\n");
    printf("####\n");
    printf("####\n");

    history = Create_History(20);

    getcwd(home_path, 500);

    FILE *fp = fopen("history.txt", "r");

    int ch = fgetc(fp);

    while (ch != EOF)
    {
        char inp[500] = "";

        int len = 0;

        while (ch != EOF)
        {
            inp[len++] = (char)ch;
            ch = fgetc(fp);
            if (inp[len - 1] == '\n')
                break;
        }

        push_back(history, inp);
    }

    fclose(fp);

    while (1)
    {
        prompt(home_path);

        enableRawMode();

        fflush(stdout);

        setbuf(stdout, NULL);

        char init_command[200] = "", inp_char;

        sigaction(SIGCHLD, &checker, NULL);

        int pt = 0, flag_tab = 0;

        while (read(STDIN_FILENO, &inp_char, 1) == 1)
        {
            if (iscntrl(inp_char))
            {
                if (inp_char == 10)
                {
                    printf("\n");
                    break;
                }
                else if (inp_char == 27)
                {
                    char buf[3];

                    buf[2] = 0;

                    if (read(STDIN_FILENO, buf, 2) == 2)
                    {
                        printf("\rarrow key: %s", buf);
                    }
                }
                else if (inp_char == 127)
                {
                    if (pt > 0)
                    {
                        if (init_command[pt - 1] == 9)
                        {
                            for (int i = 0; i < 7; i++)
                            {
                                printf("\b");
                            }
                        }
                        init_command[--pt] = '\0';
                        printf("\b \b");
                    }
                }
                else if (inp_char == 9)
                {
                    init_command[pt] = '\0';

                    int tmp_pos = 0;

                    for (int i = strlen(init_command) - 1; i >= 0; i--)
                        if (init_command[i] == ' ')
                        {
                            tmp_pos = i + 1;
                            break;
                        }

                    char buff[500] = "";

                    for (int i = tmp_pos; i < strlen(init_command); i++)
                        buff[i - tmp_pos] = init_command[i];

                    char *output_tab = malloc(sizeof(char) * 200);

                    output_tab = tab_handler(buff, home_path);

                    for (int j = temp_tab; j < strlen(output_tab); j++)
                    {
                        init_command[pt++] = output_tab[j];
                        if (count_possiblities == 1)
                        {
                            printf("%c", output_tab[j]);
                        }
                    }

                    if (init_command[pt - 1] != '/' && count_possiblities == 1)
                    {
                        init_command[pt++] = ' ';
                        printf("%c", init_command[pt - 1]);
                    }

                    if (count_possiblities > 1)
                    {
                        prompt(home_path);
                        printf("%s", init_command);
                    }
                }
                else if (inp_char == 4)
                {
                    printf("\n");
                    EXIT_SHELL();
                }
                else
                {
                    printf("%d\n", inp_char);
                }
            }
            else
            {
                init_command[pt++] = inp_char;
                printf("%c", inp_char);
            }
        }

        disableRawMode();

        if (strcmp(init_command, "EXIT") == 0)
        {
            disableRawMode();
            EXIT_SHELL();
        }

        getcwd(current_directory, 500);

        if (strlen(init_command) == 1)
        {
            signal(SIGCHLD, handle_dead);
            continue;
        }

        if (isempty(history))
        {
            push_back(history, init_command);
        }

        else if (strcmp(init_command, back(history)) != 0)
        {
            push_back(history, init_command);
        }

        char *temp_token;

        int num_com = 0;

        for (int i = 0; i < strlen(init_command); i++)
        {
            io_level = 0;

            com_lev = 0;

            int num = 0, pipe_count = 0, checker_loop = 0;

            char command[500] = "";

            inv_command = 0;

            for (int j = i; j < strlen(init_command); j++, i++)
            {
                checker_loop = 1;

                if (init_command[j] == '|')
                    pipe_count++;

                if (init_command[j] == ';')
                {
                    i++;
                    break;
                }

                else if (init_command[j] == '&')
                {
                    com_lev = 1;

                    i++;

                    break;
                }

                command[num++] = init_command[j];
            }

            if (checker_loop)
                i--;

            if (inv_command)
                continue;

            if (pipe_count)
            {
                piper(command, home_path, prev_path, pipe_count);
                continue;
            }

            command_exec(command, home_path, prev_path);

            dup2(inp_ptr, 0);

            dup2(outp_ptr, 1);
        }
    }
}