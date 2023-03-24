/*
        CSC D84 - Unit 3 - Reinforcement Learning

        This file contains the API function headers for your assignment.
        Please pay close attention to the function prototypes, and
        understand what the arguments are.

        Stubs for implementing each function are to be found in QLearn.c,
        along with clear ** TO DO markers to let you know where to add code.

        You are free to add helper functions within reason. But you must
        provide a prototype *in this file* as well as the implementation
        in the .c program file.

        Starter by: F.J.E., Jan. 2016
*/

#ifndef __QLearn_header

#define __QLearn_header

// Generally needed includes
// #include <malloc.h>
#include <assert.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define alpha .01 // Learning rate for standard Q-Learning
#define lambda .5 // Discount rate for future rewards
#define max_graph_size 32 * 32

#define numFeatures 3 // UPDATE THIS to be the number of features you have
#define numActions 4
#define BIG_DBL 1000000000000.0
#define EPSILON 1e-9

typedef struct DequeItem {
  int x;
  int y;
  int distance;
  struct DequeItem *prev, *next;
} DequeItem;

typedef struct Deque {
  DequeItem *head;
  DequeItem *tail;
  int size;
} Deque;

enum direction { UP = 0, RIGHT = 1, DOWN = 2, LEFT = 3 };

// Function prototypes for D84 - Unit 3 - Reinforcement Learning
void QLearn_update(int s, int a, double r, int s_new, double *QTable);
int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2],
                  int cats[5][2], int cheeses[5][2], double pct, double *QTable,
                  int size_X, int graph_size);
double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2],
                     int cats[5][2], int cheeses[5][2], int size_X,
                     int graph_size);

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25],
                        double reward, int mouse_pos[1][2], int cats[5][2],
                        int cheeses[5][2], int size_X, int graph_size);
int feat_QLearn_action(double gr[max_graph_size][4], double weights[25],
                       int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
                       double pct, int size_X, int graph_size);
void evaluateFeatures(double gr[max_graph_size][4], double features[25],
                      int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
                      int size_X, int graph_size);
double Qsa(double weights[25], double features[25]);
void maxQsa(double gr[max_graph_size][4], double weights[25],
            int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X,
            int graph_size, double *maxU, int *maxA);

void expensiveFeature1(double gr[max_graph_size][4],
                       int path[max_graph_size][2], int start_x, int start_y,
                       int (*goalCheck)(int x, int y, int pos[5][2]),
                       int pos[5][2], int s_type, int *l, int size_X);
int checkForGoal(int x, int y, int pos[5][2]);

// If you need to add any function prototypes yourself, you can do so *below*
// this line.

// BEGIN HELPER FUNCTION PROTOS
double get_random_uniform(double min, double max);
void set_next_pos(int next_pos[2], int pos[2], int direction);
bool is_pos_in_poss(int pos[2], int poss[][2]);
int get_state_index(int mouse_loc[1][2], int cats[5][2], int cheeses[5][2],
                    int size_X, int graph_size);
int get_q_table_index(int state, int action);
int pos_to_index(int pos[2], int size_X);
int is_pos_valid(int pos[2], int size_X, int size_Y);
double avg_cat_feat(double gr[max_graph_size][4], int mouse_pos[1][2],
                    int cats[5][2], int size_X, int graph_size);
double get_scaled_closest_distance(double gr[max_graph_size][4],
                                   int mouse_pos[1][2], int agents[5][2],
                                   int size_X, int graph_size);
bool dead_end(double gr[max_graph_size][4], int mouse_pos[1][2], int size_X);
double angle(int mouse_pos[1][2], int cats[5][2], int cheese[2]);
int get_random_legal_action(double gr[max_graph_size][4], int mouse_pos[1][2],
                            int size_X, int graph_size);
DequeItem *DequeItem_new(int x, int y, int distance);
Deque *Deque_new(void);
void Deque_push_front(Deque *deque, int x, int y, int distance);
DequeItem Deque_pop_back(Deque *deque);
void Deque_dtor(Deque *deque);
// END HELPER FUNCTION PROTOS

// Set random seed:
// srand((unsigned)time(NULL));

#endif // __QLearn_header
