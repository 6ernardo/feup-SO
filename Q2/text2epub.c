#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, const char *argv[])
{
    if (argc < 2)
    {
        printf("Invalid input\n");
        return EXIT_FAILURE;
    }

    for (int i = 1; i < argc; i++)
    {

        pid_t pid;
        pid = fork();

        if (pid == -1)
        {
            // fork error
            fprintf(stderr, "%s: isn't possible to fork: %s\n", argv[0], strerror(errno));
            continue;
        }
        else if (pid == 0) // child process
        {
            // pid_t
            const char *fileTxt = argv[i];
            char filename[strlen(fileTxt)];

            // txt to epub
            strcpy(filename, fileTxt);
            filename[strlen(filename) - 4] = '\0';
            strcat(filename, ".epub");

            // message [pidxxxx] converting filex.txt
            printf("[pid%i] converting %s ... \n", getpid(), fileTxt);

            // pandoc use
            execlp("pandoc", "pandoc", fileTxt, "-o", filename, (char *)0);
        }
        else // parent process
        {
            if (waitpid(pid, NULL, 0) == -1)
            {
                return EXIT_FAILURE;
            }
        }
    }
    char *zip[argc + 4];
    // zip
    zip[0] = malloc((strlen("zip") + 1) * sizeof(char));
    strcpy(zip[0], "zip");

    // ebooks.zip
    zip[1] = malloc((strlen("ebooks.zip") + 1) * sizeof(char));
    strcpy(zip[1], "ebooks.zip");

    for (int i = 0; i < argc - 1; i++)
    {
        char args[sizeof(argv[i + 1] + 1)] = {}, *dot;
        strcpy(args, argv[i + 1]);
        dot = strchr(args, '.'); // search the "." position
        if (dot != 0)
        {
            *dot = 0;
        }
        strcat(args, ".epub");                                  // append the .epub to the output file
        zip[i + 2] = malloc((strlen(args) + 1) * sizeof(char)); // allocate mem in array
        strcpy(zip[i + 2], args);
    }

    zip[argc + 2] = "--quiet"; // no warnings pandoc
    zip[argc + 3] = NULL;

    execvp(zip[0], zip);

    return EXIT_SUCCESS;
}