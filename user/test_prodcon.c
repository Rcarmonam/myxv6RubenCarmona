// test_prodcon.c
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void producer()
{
  while(1) {
//    printf("producer calling sem_wait\n");
    sem_wait(&buffer->free);
    sem_wait(&buffer->lock);
    if (buffer->num_produced >= MAX) {
        sem_post(&buffer->free);
        sem_post(&buffer->occupied);
        sem_post(&buffer->lock);
        exit(0);
    }
    buffer->num_produced++;
    printf("producer %d producing %d\n", getpid(), buffer->num_produced);
    buffer->buf[buffer->nextin++] = buffer->num_produced;

    buffer->nextin %= BSIZE;
    sem_post(&buffer->occupied);

    /* Now either buffer->occupied < BSIZE and buffer->nextin is the index
       of the next empty slot in the buffer, or
       buffer->occupied == BSIZE and buffer->nextin is the index of the
       next (occupied) slot that will be emptied by a consumer
       (such as buffer->nextin == buffer->nextout) */

    sem_post(&buffer->lock);
  }
}

void consumer()
{
  while(1) {
 //   printf("Consumer calling sem_wait\n");
    sem_wait(&buffer->occupied);
    sem_wait(&buffer->lock);
    if (buffer->num_consumed >= MAX) {
        sem_post(&buffer->occupied);
        sem_post(&buffer->free);
        sem_post(&buffer->lock);
        exit(0);
   }
    printf("consumer %d consuming %d\n", getpid(), buffer->buf[buffer->nextout]);
    buffer->total += buffer->buf[buffer->nextout++];
    buffer->nextout %= BSIZE;
    buffer->num_consumed++;
    sem_post(&buffer->free);

    /* now: either b->occupied > 0 and b->nextout is the index
       of the next occupied slot in the buffer, or
       b->occupied == 0 and b->nextout is the index of the next
       (empty) slot that will be filled by a producer (such as
       b->nextout == b->nextin) */

    sem_post(&buffer->lock);

  }
}
void run_test(int nproducers, int nconsumers) {
    int i;
    for (i = 0; i < nproducers; i++)
        if (!fork()) {
            producer();
            exit(0);
        }
    for (i = 0; i < nconsumers; i++)
        if (!fork()) {
            consumer();
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
    printf("Test 1:\n");
    run_test(1, 1);

    // Test 2: Multiple Producers and Consumers Test
    printf("Test 2:\n");
    run_test(3, 3);

    // Test 3: Priority-Based Synchronization Test
    printf("Test 3:\n");
    run_test(2, 2);

    // Test 4: Complex Real-World Scenario Test
    printf("Test 4:\n");
    run_test(4, 4);

    exit(0);
