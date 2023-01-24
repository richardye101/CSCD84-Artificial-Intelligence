/*
        CSC D84 - Unit 1 - Search

        This file contains the API function headers for your assignment.
        Please pay close attention to the function prototypes, and
        understand what the arguments are.

        Stubs for implementing each function are to be found in AI_search.c,
        along with clear ** TO DO markers to let you know where to add code.

        You are free to add helper functions within reason. But you must
        provide a prototype *in this file* as well as the implementation
        in the .c program file.

        Starter by: F.J.E., Jul. 2015
        Updated by: F.J.E., Jan. 2018
*/

#ifndef __AI_search_header

#define __AI_search_header

// C STL includes
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "board_layout.h"

// BEGIN STRUCT DEFS
// Deque will be used for both queue and stack
typedef struct Cord {
  int x, y;
} Cord;

typedef struct DequeItem {
  Cord cord;
  DequeItem *prev, *next;
} DequeItem;

} typedef struct Deque {
  DequeItem *head;
  DequeItem *tail;
  int size;
} Deque;

typedef struct HeapItem {
  Cord cord;
  double priority;
} HeapItem;

typedef struct Heap {
  HeapItem data[graph_size];
  int size;
} Heap;
// END STRUCT DEFS

// BEGIN STRUCT HELPER FUNCTION PROTOS
DequeItem *DequeItem_new(int x, int y);
Deque *Deque_new(void);
void Deque_push_front(Deque *deque, int x, int y);
void Deque_push_back(Deque *deque, int x, int y);
Cord Deque_pop_front(Deque *deque);
Cord Deque_pop_back(Deque *deque);
// END STRUCT HELPER FUNCTION PROTOS

// Function prototypes for D84 - Unit 1 - Search assignment solution

// BEGIN MISC HELPER FUNCTION PROTOS
void index_to_cord(int index, int *x, int *y);
int cord_to_index(int x, int y);
int is_cord_valid(int x, int y);
// END MISC HELPER FUNCTION PROTOS


void search(double gr[graph_size][4], int path[graph_size][2],
            int visit_order[size_X][size_Y], int cat_loc[10][2], int cats,
            int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode,
            int (*heuristic)(int x, int y, int cat_loc[10][2],
                             int cheese_loc[10][2], int mouse_loc[1][2],
                             int cats, int cheeses, double gr[graph_size][4]));
int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2],
           int mouse_loc[1][2], int cats, int cheeses,
           double gr[graph_size][4]);
int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2],
                   int mouse_loc[1][2], int cats, int cheeses,
                   double gr[graph_size][4]);

// If you need to add any function prototypes yourself, you can do so *below*
// this line.

#endif
