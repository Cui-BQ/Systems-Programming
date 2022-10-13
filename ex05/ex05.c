#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef enum state_enum
  {
   state_none,
   state_a,
   state_e,
   state_i,
   state_o,
   state_u,
   NUM_STATES,
   state_done
  } State;

typedef enum char_class_enum
  {
   char_ws,
   char_a,
   char_e,
   char_i,
   char_o,
   char_u,
   char_non_ws,
   char_EOF,
   NUM_CHAR_CLASS
  } CharClass;

/* The "object" representing the state machine,
   except for the transition table */
typedef struct system_t
{
  State     current_state;
  char      next_char;
  CharClass next_char_class;
  char*     word;
  int       word_index;
  int       max_word_size;
} System;


/* Utility method: converts raw character to class */
CharClass classifyChar(char c)
{
  switch (tolower(c))
    {
    case EOF:  return char_EOF;
    case 'a':  return char_a;
    case 'e':  return char_e;
    case 'i':  return char_i;
    case 'o':  return char_o;
    case 'u':  return char_u;
    default:   if ( isspace(c) ) return char_ws;
    }
  return char_non_ws;
}

/* Utility method: sets currently accumulated word string to the empty string */
void System_resetWord(System *pSystem)
{
  pSystem->word_index = 0;
  // Move pSystem->next_char to a char_ws or char_EOF
  while (pSystem->next_char_class != char_ws && pSystem->next_char_class != char_EOF) {
    pSystem->next_char = getchar();
    pSystem->next_char_class = classifyChar(pSystem->next_char);
  }
}

/* Utility method: adds the last read character to the current word string */
void System_addchar(System *pSystem)
{   
  pSystem->word[pSystem->word_index++] = pSystem->next_char;

  // Double the array size if it's full.
  if (pSystem->word_index == pSystem->max_word_size) {
      pSystem->max_word_size *= 2;
      pSystem->word = (char *) realloc(pSystem->word, sizeof(char)*pSystem->max_word_size);
  }
}

/* Utility method: prints the current word */
void System_printWord(System *pSystem)
{
  pSystem->word[pSystem->word_index] = '\0';
  printf("%s\n", pSystem->word);
  pSystem->word_index = 0;
}

/* "Constructor" for a System structure */
void System_initialize(System *pSystem)
{
  pSystem->max_word_size = 2;
  pSystem->word = (char*)malloc(sizeof(char) * 2);
  pSystem->current_state = state_none;
  System_resetWord(pSystem);
}
  
/* This declares a function pointer type for functions that
   take a pointer to a System structure and return nothing. */
typedef void (*ActionFunction)(System*);

/* This declares a structure for the elements of the 
   transition table.  (This is a bit primitive because it
   allows only one action per transition.)  */
typedef struct transition_t {
  State next_state;
  ActionFunction action;
} Transition;

/* The transition table for the vowels state machine */
Transition transition_matrix[NUM_STATES][NUM_CHAR_CLASS] =
  {
   { // state_none
    {state_none, System_resetWord},            // char_ws
    {state_a,    System_addchar},  // char_a
    {state_none, System_resetWord},  // char_e
    {state_none, System_resetWord},  // char_i
    {state_none, System_resetWord},  // char_o
    {state_none, System_resetWord},  // char_u
    {state_none, System_addchar},  // char_non_ws
    {state_done, NULL },           // char_eof
   },
   { // state_a
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_e,    System_addchar},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_a,    System_addchar},
    {state_done, NULL}
   },
   { // state_e
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_i,    System_addchar},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_e,    System_addchar},
    {state_done, NULL}
   },
   { // state_i
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_o,    System_addchar},
    {state_none, System_resetWord},
    {state_i,    System_addchar},
    {state_done, NULL}
   },
   { // state_o
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_u,    System_addchar},
    {state_o,    System_addchar},
    {state_done, NULL}
   },
   {  // state_u
    {state_none, System_printWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_none, System_resetWord},
    {state_u,    System_addchar},
    {state_done, System_printWord}
   }
  };

/* The mainline: has the logic to read the input tokens and make
   state transitions.  This code is state machine independent --
   there's nothing in it that has to do with the vowels problem.  */

int main(int argc, char *argv[]) {
  System system;
  ActionFunction action;

  System_initialize(&system);

  while (system.current_state != state_done )
    {
      system.next_char = getchar();
      system.next_char_class = classifyChar(system.next_char);

      action = transition_matrix[system.current_state][system.next_char_class].action;
      if ( action != NULL )
        action(&system);

      system.current_state = transition_matrix[system.current_state][system.next_char_class].next_state;
    }
  free(system.word);
  return EXIT_SUCCESS;
}
