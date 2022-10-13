/*
 * Copyright ©2020 Hal Perkins.  All rights reserved.  Permission is
 * hereby granted to students registered for University of Washington
 * CSE 333 for use solely during Fall Quarter 2020 for purposes of
 * the course.  No other use, copying, distribution, or modification
 * is permitted without prior written consent. Copyrights for
 * third-party components of this work must be honored.  Instructors
 * interested in reusing these course materials should contact the
 * author.
 */

// Feature test macro for strtok_r (c.f., Linux Programming Interface p. 63)
#define _XOPEN_SOURCE 600

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

#include "libhw1/CSE333.h"
#include "./CrawlFileTree.h"
#include "./DocTable.h"
#include "./MemIndex.h"

//////////////////////////////////////////////////////////////////////////////
// Helper function declarations, constants, etc
static void Usage(void);
static void ProcessQueries(DocTable *dt, MemIndex *mi);
static int GetNextLine(FILE *f, char **retstr);
static void LLNoOpFree(LLPayload_t freeme) { }

//////////////////////////////////////////////////////////////////////////////
// Main
int main(int argc, char **argv) {
  if (argc != 2) {
    Usage();
  }

  // Implement searchshell!  We're giving you very few hints
  // on how to do it, so you'll need to figure out an appropriate
  // decomposition into functions as well as implementing the
  // functions.  There are several major tasks you need to build:
  //
  //  - Crawl from a directory provided by argv[1] to produce and index
  //  - Prompt the user for a query and read the query from stdin, in a loop
  //  - Split a query into words (check out strtok_r)
  //  - Process a query against the index and print out the results
  //
  // When searchshell detects end-of-file on stdin (cntrl-D from the
  // keyboard), searchshell should free all dynamically allocated
  // memory and any other allocated resources and then exit.

  DocTable* dt;
  MemIndex* mi;
  char *dir = argv[1];
  printf("Indexing \'%s\'\n", dir);
  if (!CrawlFileTree(dir, &dt, &mi)) {
    Usage();
  } else {
    ProcessQueries(dt, mi);
  }
  MemIndex_Free(mi);
  DocTable_Free(dt);
  
  return EXIT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////////
// Helper function definitions

static void Usage(void) {
  fprintf(stderr, "Usage: ./searchshell <docroot>\n");
  fprintf(stderr,
          "where <docroot> is an absolute or relative " \
          "path to a directory to build an index under.\n");
  exit(EXIT_FAILURE);
}

static void ProcessQueries(DocTable *dt, MemIndex *mi) {
  char buffer[1024];
  while (true) {
    printf("enter query:\n");
    if (fgets(buffer, 1024, stdin) == NULL){
      printf("enter query:\n");
      continue;
    }
    if (feof(stdin)){
      break;
    }
    char* token = strtok(buffer, " ");
    char *query[1024];
    int count = 0;
    while (token != NULL) {
      query[count] = token;
      count++;
      token = strtok(NULL, " "); 
    }
    char *lastChar = strchr(query[count - 1], '\n');
    *lastChar = '\0';
    LinkedList* result =  MemIndex_Search(mi, query, count);
    if (LinkedList_NumElements(result) == 0 || result == NULL){
      LinkedList_Free(result, &LLNoOpFree);
      continue;
    }
    LLIterator* llitr = LLIterator_Allocate(result);
    while (LLIterator_Next(llitr)) {
      SearchResult* res;
      LLIterator_Get(llitr, (LLPayload_t *)&res);
      char *dir = DocTable_GetDocName(dt, res->docid);
      printf("  %s (%d)\n", dir, res->rank);
    }
    LinkedList_Free(result, &LLNoOpFree);
    LLIterator_Free(llitr);
  }
}
