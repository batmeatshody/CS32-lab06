#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: childcreates <iterations>\n");
        return 1;
    }

    int iterations = strtol(argv[1], NULL, 10);

    for (int i = 0; i < iterations; i++) {
        fflush(stdout);
        pid_t child = fork();
        if (child < 0) {
            perror("fork");
            return 1;
        }

        printf("ppid = %d, pid = %d, i = %d\n", getppid(), getpid(), i);
        fflush(stdout);

        if (child > 0) {
            while (waitpid(child, NULL, 0) == -1) {
                if (errno == EINTR) {
                    continue;
                }
                perror("waitpid");
                return 1;
            }
            return 0;
        }
    }

    return 0;
}
