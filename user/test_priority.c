#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/syscall.h"
#include "kernel/proc.h"

int main() {
    int i, pid;
    int priorities[] = {30, 50, 20, 70, 10}; // Example priorities

    for (i = 0; i < sizeof(priorities) / sizeof(priorities[0]); i++) {
        pid = fork();
        if (pid == 0) {
            // Child process
            int priority = priorities[i];
            printf("Setting priority for pid %d to %d\n", getpid(), priority);
            setpriority(priority);
            sleep(100); // Sleep for a while to allow other processes to run
            exit(0);
        } else if (pid > 0) {
            // Parent process
            wait(0);
        } else {
            // Fork failed
            printf("Fork failed\n");
            exit(1);
        }
    }

    exit(0);
}
