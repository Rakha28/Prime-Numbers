#include <stdio.h>
#include <math.h>
#include <pthread.h>

// A function to check if a number is prime or not
int is_prime(int n) {
  // 0 and 1 are not prime
  if (n == 0 || n == 1) {
    return 0;
  }
  // 2 is the only even prime
  if (n == 2) {
    return 1;
  }
  // Even numbers are not prime
  if (n % 2 == 0) {
    return 0;
  }
  // Check for odd divisors up to the square root of n
  int limit = (int) sqrt(n);
  for (int i = 3; i <= limit; i += 2) {
    if (n % i == 0) {
      return 0;
    }
  }
  // If no divisor is found, n is prime
  return 1;
}

// A structure to store the arguments for each thread
struct thread_args {
   int low; // The lower bound of the range
   int high; // The upper bound of the range
   int count; // The prime number count within the range
};

// A function to find the prime number count within a range for each thread
void *prime_count(void *args) {
   // Cast the argument to a thread_args pointer
   struct thread_args *targs = (struct thread_args *) args;

   // Initialize the count to zero
   targs->count = 0;

   // Loop from low to high and check each number for primality
   for (int i = targs->low; i <= targs->high; i++) {
     if (is_prime(i)) {
       targs->count++;
     }
   }

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