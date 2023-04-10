#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

// A structure to store the arguments for each thread
struct thread_args {
   int low; // The lower bound of the range
   int high; // The upper bound of the range
   int count; // The prime number count within the range
};

// A function to create a boolean array of size n+1 and mark all primes up to n
bool* sieve(int n) {
  // Allocate memory for the array and initialize all elements to true
  bool* is_prime = malloc((n+1) * sizeof(bool));
  for (int i = 0; i <= n; i++) {
    is_prime[i] = true;
  }
  // Mark 0 and 1 as not prime
  is_prime[0] = false;
  is_prime[1] = false;
  // Loop from 2 to the square root of n and mark multiples of each number as not prime
  int limit = (int) sqrt(n);
  for (int i = 2; i <= limit; i++) {
    if (is_prime[i]) {
      for (int j = i * i; j <= n; j += i) {
        is_prime[j] = false;
      }
    }
  }
  // Return the array
  return is_prime;
}

// A function to find the prime number count within a range for each thread
void *prime_count(void *args) {
   // Cast the argument to a thread_args pointer
   struct thread_args *targs = (struct thread_args *) args;

   // Initialize the count to zero
   targs->count = 0;

   // Create a boolean array of size high+1 and mark all primes up to high
   bool* is_prime = sieve(targs->high);

   // Loop from low to high and check each number for primality using the array
   for (int i = targs->low; i <= targs->high; i++) {
     if (is_prime[i]) {
       targs->count++;
     }
   }

   // Free the memory allocated for the array
   free(is_prime);

   // Return the pointer to the thread_args structure
   return args;
}

// A main function to test the program
int main() {

   // Define the range and the number of threads
   int low = (int)1E6;
   int high = (int)2E6;
   int num_threads = 4;

   // Calculate the size of each sub-range for each thread
   int size = (high - low + num_threads -1) / num_threads;

   // Create an array of thread_args structures for each thread
   struct thread_args targs[num_threads];

   // Create an array of pthread_t variables for each thread
   pthread_t threads[num_threads];

   // Loop through each thread and create it with its corresponding arguments
   for (int i = 0; i < num_threads; i++) {
      // Set the lower and upper bounds of the sub-range for this thread
      targs[i].low = low + i * size;
      targs[i].high = low + (i +1) * size -1;

      // Create the thread and pass it its arguments as a void pointer
      pthread_create(&threads[i], NULL, prime_count, &targs[i]);
   }

   // Initialize the total count to zero
   int total_count =0;

    // Loop through each thread and wait for it to finish and get its result 
    for (int i =0; i < num_threads; i++) {
       // Join the thread and get its returned value as a void pointer 
       void *result;
       pthread_join(threads[i], &result);

       // Cast the result to a thread_args pointer 
       struct thread_args *res_targs = (struct thread_args *) result;

       // Add the count of this thread to the total count 
       total_count += res_targs->count;
    }

    // Print the total prime number count within the range 
    printf("The prime number count within range %d - %d using %d threads is: %d\n", low, high, num_threads, total_count);

    // Exit the program 
    return 0;
}