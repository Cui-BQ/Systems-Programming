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

#include <stdio.h>     // for printf, sscanf
#include <stdint.h>    // for int8_t, etc.
#include <inttypes.h>  // for extended integer print formats
#include <stdlib.h>    // for EXIT_SUCCESS / EXIT_FAILURE and exit

#include "ex01.h"      // defines symbol for array initial data

/************************************************************
 * This is a very skeletal skeleton.
 * Lines starting "// ++++" are messages trying to indicate what 
 * you should implement.
 *************************************************************/
int CopyAndSort(int8_t unsorted[], int8_t sorted[], uint32_t ARRAY_SIZE);
int InsertionSort(int8_t sorted[], int i, int8_t current, uint32_t ARRAY_SIZE);

int main(int argc, char **argv) {
  // Create the unsorted array
  // ++++ The array's size will be determined by the initialization data,
  // ++++ which has been put in a separate file to make it easy to change.
  // ++++ Change it.
  int8_t unsorted[] = INIT_DATA;  // see contents of ex01.h

  // ++++ Initialize ARRAY_SIZE to the number of elements in array unsorted.
  // ++++ Hint: use sizeof(), twice
  // ++++ Note: It doesn't make sense for code to change ARRAY_SIZE once
  // ++++ initialized.  The "const" in the declaration makes it harder
  // ++++ for some programmer modifying this code in the future to make
  // ++++ accidentially decided to change it.  It's kind of like "final"
  // ++++ in Java, except it's possible to get around it if you try.
  const uint32_t ARRAY_SIZE = sizeof(unsorted) / sizeof(unsorted[0]);

  // create the second array
  int8_t sorted[ARRAY_SIZE];

  // invoke CopyAndSort()
  if ( CopyAndSort(unsorted, sorted, ARRAY_SIZE) ) {
    fprintf(stderr, "CopyAndSort() returned failure\n");
    return EXIT_FAILURE;
  }

  // ++++ Print the sorted (second) array.
  for (int i = 0; i < ARRAY_SIZE; i++) {
    printf("%" PRIi8 " ", sorted[i]);
  }
  printf("\n");

  // All done
  return EXIT_SUCCESS;
}

int CopyAndSort(int8_t unsorted[], int8_t sorted[], uint32_t ARRAY_SIZE) {
  for (int i = 0; i < ARRAY_SIZE; i++) {
    int8_t current = unsorted[i];
    int ret = InsertionSort(sorted, i, current, ARRAY_SIZE);
    if (ret > 0) {
      return ret;
    }
  }
  return 0;
}

int InsertionSort(int8_t sorted[], int i, int8_t current, uint32_t ARRAY_SIZE) {
  if (i >= ARRAY_SIZE) {
    return 1;
  }
  if (current < -128 || current > 127) {
    return 2;
  }
  if (i == 0) {
    sorted[0] = current;
    return 0;
  } else {
    while (i > 0 && sorted[i-1] > current) {
      sorted[i] = sorted[i-1];
      i--;
    }
    sorted[i] = current;
    return 0;
  }
}
