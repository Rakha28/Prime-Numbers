#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define BITS_PER_BYTE 8
#define SET_BIT(A, k)(A[(k) / BITS_PER_BYTE] |= (1 << ((k) % BITS_PER_BYTE)))
#define CLEAR_BIT(A, k)(A[(k) / BITS_PER_BYTE] &= ~(1 << ((k) % BITS_PER_BYTE)))
#define TEST_BIT(A, k)(A[(k) / BITS_PER_BYTE] & (1 << ((k) % BITS_PER_BYTE)))

struct thread_args {
  int low;
  int high;
  int count;
};

void * sieve(void * args) {
  struct thread_args * targs = (struct thread_args * ) args;
  targs -> count = 0;
  int n = targs -> high;
  int limit = (int) sqrt(n);
  int size = (n + BITS_PER_BYTE - 1) / BITS_PER_BYTE; // number of bytes needed to store n bits
  unsigned char * is_prime = malloc(size); // allocate memory for n bits
  memset(is_prime, 255, size); // set all bits to 1
  CLEAR_BIT(is_prime, 0); // clear bit 0 (0 is not prime)
  CLEAR_BIT(is_prime, 1); // clear bit 1 (1 is not prime)
  for (int i = 2; i <= limit; i++) {
    if (TEST_BIT(is_prime, i)) { // if i is prime
      for (int j = i * i; j <= n; j += i) {
        CLEAR_BIT(is_prime, j); // clear all multiples of i
      }
    }
  }
  for (int i = targs -> low; i <= targs -> high; i++) {
    if (TEST_BIT(is_prime, i)) { // if i is prime
      targs -> count++; // increment count
    }
  }
  free(is_prime); // free memory
  return args;
}

int main() {
  int low = (int) 1E6;
  int high = (int) 2E6;
  int num_threads = 4;
  int size = (high - low + num_threads - 1) / num_threads;
  struct thread_args targs[num_threads];
  pthread_t threads[num_threads];
  for (int i = 0; i < num_threads; i++) {
    targs[i].low = low + i * size;
    targs[i].high = low + (i + 1) * size - 1;
    pthread_create( & threads[i], NULL, sieve, & targs[i]);
  }
  int total_count = 0;
  for (int i = 0; i < num_threads; i++) {
    void * result;
    pthread_join(threads[i], & result);
    struct thread_args * res_targs = (struct thread_args * ) result;
    total_count += res_targs -> count;
  }
  printf("The prime number count within range %d - %d using %d threads is: %d\n", low, high, num_threads, total_count);
  return 0;
}