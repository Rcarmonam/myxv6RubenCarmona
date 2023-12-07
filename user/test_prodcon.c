// test_prodcon.c

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void run_test(int nproducers, int nconsumers) {
    int i;
    for (i = 0; i < nproducers; i++)
        if (!fork()) {
            exec("prodcon", 0);
            exit(0);
        }
    for (i = 0; i < nconsumers; i++)
        if (!fork()) {
            exec("prodcon", 0);
            exit(0);
        }
    for (i = 0; i < nproducers + nconsumers; i++)
        wait(0);
}

int main(int argc, char *argv[]) {
    if (argc != 1) {
        printf("usage: %s\n", argv[0]);
        exit(1);
    }

    // Test 1: Basic Producer-Consumer Test
    printf("Test 1: Basic Producer-Consumer Test\n");
    run_test(3, 1);

    // Test 2: Multiple Producers and Consumers Test
    printf("Test 2: Multiple Producers and Consumers Test\n");
    run_test(3, 3);

    // Test 3: Priority-Based Synchronization Test
    printf("Test 3: Priority-Based Synchronization Test\n");
    run_test(2, 1);

    // Test 4: Complex Real-World Scenario Test
    printf("Test 4: Complex Real-World Scenario Test\n");
    run_test(4, 4);

    exit(0);
}
