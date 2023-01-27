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
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "board_layout.h"

// BEGIN MODES
#define MODE_BFS 0
#define MODE_DFS 1
#define MODE_A_STAR 2
// END MODES

// BEGIN DIRECTIONS
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
// END DIRECTIONS

// BEGIN STRUCT DEFS
typedef int (*HEURISTIC_FUNCTION)(int x, int y, int cat_loc[10][2],
                                  int cheese_loc[10][2], int mouse_loc[1][2],
                                  int cats, int cheeses,
                                  double gr[graph_size][4]);

// Deque will be used for both queue and stack
typedef struct Cord {
  int x, y;
} Cord;

typedef struct DequeItem {
  Cord cord;
  struct DequeItem *prev, *next;
} DequeItem;

typedef struct Deque {
  DequeItem *head;
  DequeItem *tail;
  int size;
} Deque;

typedef struct HeapItem {
  Cord cord;
  int priority;
} HeapItem;

typedef struct MinHeap {
  HeapItem data[graph_size];
  int size;
} MinHeap;

typedef struct DataStructure {
  Deque *deque;
  MinHeap *min_heap;
  int mode;
  HEURISTIC_FUNCTION heuristic;
} DataStructure;
// END STRUCT DEFS

// BEGIN STRUCT HELPER FUNCTION PROTOS
DequeItem *DequeItem_new(Cord cord);
Deque *Deque_new(void);
MinHeap *Heap_new(void);
void Deque_push_front(Deque *deque, Cord cord);
void Deque_push_back(Deque *deque, Cord cord);
Cord Deque_pop_front(Deque *deque); 
Cord Deque_pop_back(Deque* deque);
MinHeap* MinHeap_new(void);
void MinHeap_insert(MinHeap *min_heap, Cord cord, int priority);
Cord MinHeap_pop(MinHeap *min_heap);
DataStructure *DataStructure_new(int mode, HEURISTIC_FUNCTION heuristic);
void DataStructure_insert(DataStructure *data_structure, Cord cord,
                          int priority);
Cord DataStructure_pop(DataStructure *data_structure);
int DataStructure_size(DataStructure* data_structure);
void DataStructure_dtor(DataStructure* data_structure);
// END STRUCT HELPER FUNCTION PROTOS

// Function prototypes for D84 - Unit 1 - Search assignment solution

// BEGIN MISC HELPER FUNCTION PROTOS
Cord get_next_cord(Cord cord, int direction);
Cord index_to_cord(int index);
int cord_to_index(Cord cord);
int is_index_valid(int index);
int is_cord_valid(Cord cord);
int equal_cords(Cord a, Cord b);
void construct_path(int path[graph_size][2], int came_from[graph_size],
                    Cord start, Cord goal);
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
