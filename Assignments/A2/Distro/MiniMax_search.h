/*
	CSC D84 - Unit 2 - MiniMax and adversarial games

	This file contains the API function headers for your assignment.
	Please pay close attention to the function prototypes, and
	understand what the arguments are.

	Stubs for implementing each function are to be found in MiniMax_search.c,
	along with clear ** TO DO markers to let you know where to add code.

	You are free to add helper functions within reason. But you must
	provide a prototype *in this file* as well as the implementation
	in the .c program file.

	Script by: F.J.E., Sep. 2015
*/

#ifndef __MiniMax_search_header

#define __MiniMax_search_header

// Generally needed includes
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <float.h>
#include <limits.h>

#include "board_layout.h"

// BEGIN DIRECTIONS
#define DIRECTION_UP 0
#define DIRECTION_RIGHT 1
#define DIRECTION_DOWN 2
#define DIRECTION_LEFT 3
// END DIRECTIONS

// BEGIN STRUCT DEFS

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

typedef struct CheeseDistance {
  int cheese_distance[10][graph_size];
  int cheeses;
} CheeseDistance;
// BEGIN STRUCT DEFS
//
// BEGIN STRUCT HELPER FUNCTION PROTOS
DequeItem *DequeItem_new(Cord cord);
Deque *Deque_new(void);
void Deque_push_front(Deque *deque, Cord cord);
void Deque_push_back(Deque *deque, Cord cord);
Cord Deque_pop_front(Deque *deque);
Cord Deque_pop_back(Deque* deque);
void Deque_dtor(Deque* deque);
// END STRUCT HELPER FUNCTION PROTOS

// BEGIN HELPER FUNCTION PROTOS
int loc_to_index(int loc[2]);
void set_next_loc(int next_loc[2], int loc[1][2], int direction);
int is_loc_valid(int loc[2]);
void precompute_cheese_distance(double gr[graph_size][4], int cheese_loc[10][2],
                                int cheeses);
// END HELPER FUNCTION PROTOS

// Function prototypes for D84 - Unit 2 - MiniMax assignment solution
double MiniMax(double gr[graph_size][4], int path[1][2],
               double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats,
               int cheese_loc[10][2], int cheeses, int mouse_loc[1][2],
               int mode,
               double (*utility)(int cat_loc[10][2], int cheese_loc[10][2],
                                 int mouse_loc[1][2], int cats, int cheeses,
                                 int depth, double gr[graph_size][4]),
               int agentId, int depth, int maxDepth, double alpha, double beta);

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2],
               int cats, int cheeses, int depth, double gr[graph_size][4]);

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2],
                     int cheese_loc[10][2], int cats, int cheeses);
// If you need to add any function prototypes yourself, you can do so *below* this line.


#endif

