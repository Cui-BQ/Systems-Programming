/*
 * Copyright ©2021 John Zahorjan.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Winter Quarter 2021 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

 /*
  * Boqiang Cui
  * boqiang@uw.edu
  */

#include <stdio.h>   // for printf, sscanf
#include <stdlib.h>  // for EXIT_SUCCESS / EXIT_FAILURE and exit
#include <stdint.h>  // for uint32_t
#include <string.h>  // for strtol

// Print out the usage of the program
void Usage(void);

// Return the nth term in the Nilakantha series.
double NthNilakantha_term(uint32_t n);

int main(int argc, char **argv) {
  // Make sure the user provided us with a single command line argument
  // and exit if not.
  if (argc != 2) {
    Usage();
    exit(EXIT_FAILURE);
  }

  // Try to convert the command line argument to an integer >= 0
  // and exit if unable.
  char *arg_end;
  int num_terms = strtol(argv[1], &arg_end, 10);
  if (num_terms < 0 || arg_end != argv[1] + strlen(argv[1])) {
    Usage();
    exit(EXIT_FAILURE);
  }

  // Calculate and print the estimate.
  double estimate = 3.0;
  /* Your code using NthNilakantha_term() here.
     (You must implement the NthNilakantha_term function */
  for (int i = 1; i <= num_terms; i++) {
    estimate += NthNilakantha_term(i);
  }
  printf("Our estimate of Pi is %0.20f\n", estimate);

  // Quit
  return EXIT_SUCCESS;
}

double NthNilakantha_term(uint32_t n) {
  double ret = 4.0;
  if (n % 2 != 1) {
    ret = -4.0;
  }

  ret = ret / ((2*n) * (2*n+1) *(2*n+2));
  return ret;
}

void Usage(void) {
  fprintf(stderr,
          "usage: ./ex0 n, where n>=0.  Prints pi estimated " \
          "to n terms of the Nilakantha series.\n");
}
