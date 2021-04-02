/**
 * Juan David Torres
 * A01702686
 * Proyecto Final Mini Shell
 * Fecha: 30 Noviembre 2020
 **/

#include "header.h"

void shell_alone(char *programName)
{
    char *userLine, *tokenSingleCommand;
    int isNotQuit = 1;

    size_t sizeUserLine = sizeof(char) * MAXLENGTHLINE;
    //Memory allocation for User Line

    //Loop of shell
    while (isNotQuit)
    {
        userLine = malloc(sizeof(char) * MAXLENGTHLINE);
        if (userLine == NULL)
        {
            fprintf(stderr, "%s: Memory Allocation Error", programName);
            exit(-1);
        }

        //Memory allocation for the arguments of the command
        int argSize = 100;
        char **argsUser;
        argsUser = malloc(sizeof(char) * argSize);

        if (argsUser == NULL)
        {
            fprintf(stderr, "%s: Memory Allocation Error", programName);
            exit(-1);
        }

        fprintf(stderr, "shell> ");

        getline(&userLine, &sizeUserLine, stdin);
        if (strcmp(userLine, "\n") != 0)
        {

            userLine = strtok(userLine, "\n");
            if (strstr(userLine, "quit") != NULL)
            {
                isNotQuit = 0;
            }
            if (strstr(userLine, ";") != NULL)
            {
                //-----------------
                //Multiple commands
                //-----------------
                int counterMultipleCommands = 0;
                //Single command
                char *endFirst;
                char *commandMultipleCommands;

                //Arguments
                char *endSecond;
                char *singleCommand;
                int command_counterM = 0;
                commandMultipleCommands = strtok(userLine, ";");

                while (commandMultipleCommands != NULL)
                {
                    // printf("HHH%s",commandMultipleCommands);
                    int counterMultiple = 0;

                    if (strstr(commandMultipleCommands, "quit"))
                    {
                        isNotQuit = 1;
                        exit(0);
                    }

                    int pid;
                    if ((pid = fork()) < 0)
                    {
                        perror(programName);
                        exit(-1);
                    }
                    else if (pid == 0)
                    {
                        counterMultiple = 0;
                        singleCommand = strtok_r(commandMultipleCommands, " ", &endSecond);
                        argsUser[counterMultiple] = singleCommand;

                        while (singleCommand != NULL)
                        {
                            counterMultiple++;
                            singleCommand = strtok_r(NULL, " ", &endSecond);
                            argsUser[counterMultiple] = singleCommand;
                        }

                        argsUser[counterMultiple + 1] = (char *)0;
                        if (execvp(argsUser[0], argsUser) == -1)
                        {
                            fprintf(stderr, "%s: Warning! Error in the Command\n", programName);
                        }
                    }
                    else
                    {

                        command_counterM++;
                    }
                    counterMultipleCommands++;
                    commandMultipleCommands = strtok(NULL, ";");
                    while (command_counterM > 0)
                    {
                        wait(NULL);
                        command_counterM--;
                    }
                }
            }
            else
            {
                //-----------------
                // One Command only
                //-----------------
                int command_counter = 1;
                if (strlen(userLine) != 0 && isNotQuit)
                {
                    int counterSingleCommand = 0;
                    tokenSingleCommand = strtok(userLine, space);

                    if (strstr(tokenSingleCommand, "quit"))
                    {
                        isNotQuit = 0;
                        exit(0);
                    }
                    if (strstr(tokenSingleCommand, "CTRL-D") == NULL)
                    {

                        argsUser[counterSingleCommand] = tokenSingleCommand;
                        counterSingleCommand++;
                        while (tokenSingleCommand != NULL)
                        {
                            if (counterSingleCommand >= argSize)
                            {
                                argSize += 10;
                                argsUser = realloc(argsUser, sizeof(char) * argSize);
                                if (argsUser == NULL)
                                {
                                    fprintf(stderr, "%s: Memory Allocation Error\n", programName);
                                    exit(-1);
                                }
                            }

                            tokenSingleCommand = strtok(NULL, space);

                            if (tokenSingleCommand == NULL)
                            {
                                argsUser[counterSingleCommand] = (char *)0;
                            }
                            else
                            {
                                argsUser[counterSingleCommand] = tokenSingleCommand;
                            }
                            counterSingleCommand++;
                        }

                        int pid;
                        if ((pid = fork()) < 0)
                        {
                            perror(programName);
                            exit(-1);
                        }
                        else if (pid == 0)
                        {
                            if (execvp(argsUser[0], argsUser) == -1)
                            {
                                // fprintf(stderr,"%s%s",argsUser[0],argsUser[1]);
                                fprintf(stderr, "%s: Warning! Error in the Command\n", programName);
                            }
                        }
                        else
                        {
                            while (command_counter > 0)
                            {
                                wait(NULL);
                                command_counter--;
                            }
                        }
                    }
                    else
                    {
                        exit(0);
                    }
                }
            }

            free(userLine);
            free(argsUser);
        }
    }
}
int main(int argc, char *argv[])
{
    FILE *file;
    char *line_new;
    if (argc != 2 && argc != 1)
    {
        printf("usage: %s [filename]\n", argv[0]);
        return -1;
    }

    switch (argc)
    {

    case 1:
        shell_alone(argv[0]);
        break;

    case 2:

        if ((file = fopen(argv[1], "r")) == NULL)
        {
            perror(argv[0]);
            return -2;
        }
        char *line = NULL;
        size_t read;
        size_t len = 0;
        char newline[] = "\n";
        char semicolon[] = ";";
        int isNotQuit = 1;
        char *tokenSingleCommand;

        // commandMain = NULL;
        int counter_semicolon = 0;
        read = getline(&line, &len, file);
        printf("\nLine: %s\n", line);
        while (read != EOF && isNotQuit)
        {
            int argSize = 100;
            char **argsUser;
            argsUser = malloc(sizeof(char) * argSize);

            if (argsUser == NULL)
            {
                fprintf(stderr, "%s: Memory Allocation Error", argv[0]);
                exit(-1);
            }

            char *end_string;
            line_new = strtok_r(line, newline, &end_string);

            if (strstr(line_new, "quit") != NULL)
            {
                isNotQuit = 0;
                break;
            }
            //FILE MULTIPLE
            if (strstr(line_new, ";") != NULL)
            {
                //-----------------------------------------
                int counterMultipleCommands = 0;
                //Single command
                char *endFirst;
                char *commandMultipleCommands;

                //Arguments
                char *endSecond;
                char *singleCommand;
                int command_counterM = 0;
                commandMultipleCommands = strtok(line_new, ";");

                while (commandMultipleCommands != NULL)
                {
                    int counterMultiple = 0;
                    if (strstr(commandMultipleCommands, "quit") != NULL)
                    {
                        isNotQuit = 0;
                        return 0;
                    }

                    int pid;
                    if ((pid = fork()) < 0)
                    {
                        perror(argv[0]);
                        exit(-1);
                    }
                    else if (pid == 0)
                    {
                        counterMultiple = 0;
                        singleCommand = strtok_r(commandMultipleCommands, " ", &endSecond);
                        argsUser[counterMultiple] = singleCommand;

                        while (singleCommand != NULL)
                        {
                            if (counterMultiple >= argSize)
                            {
                                argSize += 10;
                                argsUser = realloc(argsUser, sizeof(char) * argSize);
                                if (argsUser == NULL)
                                {
                                    fprintf(stderr, "%s: Memory Allocation Error\n", argv[0]);
                                    exit(-1);
                                }
                            }
                            counterMultiple++;
                            singleCommand = strtok_r(NULL, " ", &endSecond);
                            argsUser[counterMultiple] = singleCommand;
                        }

                        argsUser[counterMultiple + 1] = (char *)0;
                        if (execvp(argsUser[0], argsUser) == -1)
                        {
                            fprintf(stderr, "%s: Warning! Error in the Command\n", argv[0]);
                        }
                    }
                    else
                    {

                        command_counterM++;
                    }
                    counterMultipleCommands++;
                    commandMultipleCommands = strtok(NULL, ";");
                    while (command_counterM > 0)
                    {
                        wait(NULL);
                        command_counterM--;
                    }
                }
            }
            /////FILE MULTIPLE
            else
            {
                int command_counter = 1;
                if (strlen(line_new) != 0 && isNotQuit)
                {
                    int counterSingleCommand = 0;
                    tokenSingleCommand = strtok(line_new, space);

                    argsUser[counterSingleCommand] = tokenSingleCommand;
                    counterSingleCommand++;
                    while (tokenSingleCommand != NULL)
                    {
                        if (counterSingleCommand >= argSize)
                        {
                            argSize += 10;
                            argsUser = realloc(argsUser, sizeof(char) * argSize);
                            if (argsUser == NULL)
                            {
                                fprintf(stderr, "%s: Memory Allocation Error\n", argv[0]);
                                exit(-1);
                            }
                        }

                        tokenSingleCommand = strtok(NULL, space);

                        if (tokenSingleCommand == NULL)
                        {
                            argsUser[counterSingleCommand] = (char *)0;
                        }
                        else
                        {
                            argsUser[counterSingleCommand] = tokenSingleCommand;
                        }
                        counterSingleCommand++;
                    }
                }
                int pid;
                if ((pid = fork()) < 0)
                {
                    perror(argv[0]);
                    exit(-1);
                }
                else if (pid == 0)
                {
                    if (execvp(argsUser[0], argsUser) == -1)
                    {
                        // fprintf(stderr,"%s%s",argsUser[0],argsUser[1]);
                        fprintf(stderr, "%s: Warning! Error in the Command\n", argv[0]);
                    }
                }
                else
                {
                    while (command_counter > 0)
                    {
                        wait(NULL);
                        command_counter--;
                    }
                }
            }

            read = getline(&line, &len, file);
            if (read != EOF)
            {
                printf("\nLine: %s\n", line);
            }

            free(argsUser);
        }
        fclose(file);

        break;
    default:
        printf("usage: %s [filename]\n", argv[0]);
        return -1;
        break;
    }

    return 0;
}
