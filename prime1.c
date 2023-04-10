#include <stdio.h>
#include <math.h>

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

// A function to find the prime number count within a range
int prime_count(int low, int high) {
  // Initialize the count to zero
  int count = 0;
  
   // Loop from low to high and check each number for primality
   for (int i = low; i <= high; i++) {
     if (is_prime(i)) {
       count++;
     }
   }
   // Return the count
   return count;
}

// A main function to test the program
int main() {
  
   // Define the range
   int low = (int)1E6;
   int high = (int)2E6;

   // Find and print the prime number count within the range
   int result = prime_count(low, high);
   printf("The prime number count within range %d - %d is: %d\n", low, high, result);

   // Exit the program
   return 0;
}