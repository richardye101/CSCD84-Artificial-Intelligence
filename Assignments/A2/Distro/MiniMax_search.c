/*
        CSC D84 - Unit 2 - MiniMax search and adversarial games

        This file contains stubs for implementing a MiniMax search
        procedure with alpha-beta pruning. Please read the assignment
        handout carefully - it describes the game, the data you will
        have to handle, and the search functions you must provide.

        Once you have read the handout carefully, implement your search
        code in the sections below marked with

        **************
        *** TO DO:
        **************

        Make sure to add it to your report.txt file - it will be marked!

        Have fun!

        DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
        management being done properly, and for memory leaks.

        Starter code: F.J.E. Sep. 15
*/

#include "MiniMax_search.h"
#include "board_layout.h"

UtilCache util_cache = {.cheeses = 0, .target_cheese = -1};

// BEGIN STRUCT HELPER FUNCTION DEFS
DequeItem *DequeItem_new(Cord cord) {
  DequeItem *deque_item = (DequeItem *)malloc(sizeof(DequeItem));
  assert(deque_item != NULL);
  deque_item->cord = cord;
  deque_item->prev = NULL;
  deque_item->next = NULL;
  return deque_item;
}

Deque *Deque_new(void) {
  Deque *deque = (Deque *)malloc(sizeof(Deque));
  assert(deque != NULL);
  deque->head = NULL;
  deque->tail = NULL;
  deque->size = 0;
  return deque;
}

void Deque_push_front(Deque *deque, Cord cord) {
  assert(deque != NULL);
  DequeItem *deque_item = DequeItem_new(cord);
  deque_item->next = deque->head;
  deque_item->prev = NULL;
  if (deque->tail == NULL) {
    deque->head = deque->tail = deque_item;
  } else {
    deque->head->prev = deque_item;
    deque->head = deque_item;
  }
  ++deque->size;
}

void Deque_push_back(Deque *deque, Cord cord) {
  assert(deque != NULL);
  DequeItem *deque_item = DequeItem_new(cord);
  deque_item->prev = deque->tail;
  deque_item->next = NULL;
  if (deque->head == NULL) {
    deque->head = deque->tail = deque_item;
  } else {
    deque->tail->next = deque_item;
    deque->tail = deque_item;
  }
  ++deque->size;
}

Cord Deque_pop_front(Deque *deque) {
  assert(deque != NULL);
  assert(deque->size != 0);
  Cord cord = deque->head->cord;
  DequeItem *deque_item = deque->head;
  if (deque->head == deque->tail) {
    deque->head = deque->tail = NULL;
  } else {
    deque->head = deque->head->next;
  }
  free(deque_item);
  --deque->size;
  return cord;
}

Cord Deque_pop_back(Deque *deque) {
  assert(deque != NULL);
  assert(deque->size != 0);
  Cord cord = deque->tail->cord;
  DequeItem *deque_item = deque->tail;
  if (deque->head == deque->tail) {
    deque->head = deque->tail = NULL;
  } else {
    deque->tail = deque->tail->prev;
  }
  free(deque_item);
  --deque->size;
  return cord;
}

void Deque_dtor(Deque *deque) {
  assert(deque != NULL);
  while (deque->size > 0) {
    Deque_pop_back(deque);
  }
  free(deque);
}
// BEGIN HELPER FUNCTION DEFS
Cord get_next_cord(Cord cord, int direction) {
  switch (direction) {
  case DIRECTION_UP:
    --cord.y;
    break;
  case DIRECTION_RIGHT:
    ++cord.x;
    break;
  case DIRECTION_DOWN:
    ++cord.y;
    break;
  case DIRECTION_LEFT:
    --cord.x;
    break;
  default:
    break;
  }
  return cord;
}
Cord index_to_cord(int index) {
  Cord cord;
  cord.x = index % size_X;
  cord.y = index / size_Y;
  return cord;
}
int cord_to_index(Cord cord) { return cord.x + cord.y * size_X; }
int is_index_valid(int index) { return 0 <= index && index < graph_size; }
int is_cord_valid(Cord cord) {
  return 0 <= cord.x && cord.x < size_X && 0 <= cord.y && cord.y < size_Y;
}
int equal_cords(Cord a, Cord b) { return a.x == b.x && a.y == b.y; }
int is_cord_in_cords(Cord cord, int cords[][2], int num_cords) {
  for (int i = 0; i < num_cords; ++i) {
    if (cord.x == cords[i][0] && cord.y == cords[i][1]) {
      return true;
    }
  }
  return false;
}

int loc_to_index(int loc[2]) { return loc[0] + loc[1] * size_X; }
void set_next_loc(int next_loc[2], int loc[2], int direction) {
  next_loc[0] = loc[0], next_loc[1] = loc[1];
  switch (direction) {
  case DIRECTION_UP:
    --next_loc[1];
    break;
  case DIRECTION_RIGHT:
    ++next_loc[0];
    break;
  case DIRECTION_DOWN:
    ++next_loc[1];
    break;
  case DIRECTION_LEFT:
    --next_loc[0];
    break;
  default:
    break;
  }
}

int is_loc_valid(int loc[2]) {
  return 0 <= loc[0] && loc[0] < size_X && 0 <= loc[1] && loc[1] < size_Y;
}

void precompute_cheese_distance(double gr[graph_size][4], int cheese_loc[10][2],
                                int cheeses) {
  Deque *deque = Deque_new();
  for (int cheese = 0; cheese < cheeses; ++cheese) {
    bool visited[graph_size];
    memset(util_cache.cheese_distance[cheese], INT_MAX,
           sizeof(util_cache.cheese_distance[cheese]));
    memset(visited, false, sizeof(visited));
    Cord cheese_cord = {cheese_loc[cheese][0], cheese_loc[cheese][1]};
    util_cache.cheese_distance[cheese][cord_to_index(cheese_cord)] = 0;
    visited[cord_to_index(cheese_cord)] = true;
    Deque_push_back(deque, cheese_cord);
    while (deque->size > 0) {
      Cord cord = Deque_pop_front(deque);
      for (int direction = 0; direction < 4; ++direction) {
        Cord next_cord = get_next_cord(cord, direction);
        if (!is_cord_valid(next_cord) || !gr[cord_to_index(cord)][direction] ||
            visited[cord_to_index(next_cord)]) {
          continue;
        }
        util_cache.cheese_distance[cheese][cord_to_index(next_cord)] =
            util_cache.cheese_distance[cheese][cord_to_index(cord)] + 1;
        visited[cord_to_index(next_cord)] = true;
        Deque_push_back(deque, next_cord);
      }
    }
  }
  Deque_dtor(deque);
}

// END HELPER FUNCTION DEFS

double MiniMax(double gr[graph_size][4], int path[1][2],
               double minmax_cost[size_X][size_Y], int cat_loc[10][2], int cats,
               int cheese_loc[10][2], int cheeses, int mouse_loc[1][2],
               int mode,
               double (*utility)(int cat_loc[10][2], int cheese_loc[10][2],
                                 int mouse_loc[1][2], int cats, int cheeses,
                                 int depth, double gr[graph_size][4]),
               int agentId, int depth, int maxDepth, double alpha,
               double beta) {
  /*
    This function is the interface between your solution for the assignment and
    the driver code. The driver code in MiniMax_search_core_GL will call this
    function once per frame, and provide the following data

    Board and game layout:

         Exactly the same as for Assignment 1 - have a look at your code if you
    need a reminder of how the adjacency list and agent positions are stored.

         Note that in this case, the path will contain a single move - at the
    top level, this function will provide an agent with the 'optimal' mini-max
    move given the game state.


    IMPORTANT NOTE: Mini-max is a recursive procedure. This function will need
    to fill-in the mini-max values for all game states down to the maximum
    search depth specified by the user. In order to do that, the function needs
    to be called with the correct state at each specific node in the mini-max
                    search tree.

                    The game state is composed of:

                         * Mouse, cat, and cheese positions (and number of cats
    and cheeses)

                    At the top level (when this function is called by the
    mini-max driver code), the game state correspond to the current situation of
    the game. But once you start recursively calling this function for lower
    levels of the search tree the positions of agents will have changed.

                    Therefore, you will need to define local variables to keep
    the game state at each node of the mini-max search tree, and you will need
    to update this state when calling recursively so that the search does the
    right thing.

                    This function *must check* whether:
                         * A candidate move results in a terminal configuration
    (cat eats mouse, mouse eats cheese) at which point it calls the utility
    function to get a value
                         * Maximum search depth has been reached
    (depth==maxDepth), at which point it will also call the utility function to
    get a value
                         * Otherwise, call recursively using the candidate
    configuration to find out what happens deeper into the mini-max tree.

    Arguments:
                 gr[graph_size][4]   		- This is an adjacency list for
    the maze
                 path[1][2] 			- Your function will return the
    optimal mini-max move in this array. minmax_cost[size_X][size_Y]	- An
    array in which your code will store the minimax value for maze locations
    expanded by the search *when called for the mouse, not for the cats!*

                                                   This array will be used to
    provide a visual display of minimax values during the game.

                 cat_loc[10][2], cats   - Location of cats and number of cats
    (we can have at most 10, but there can be fewer). Only valid cat locations
    are 0 to (cats-1) cheese_loc[10][2], cheeses - Location and number of cheese
    chunks (again at most 10, but possibly fewer). Valid locations are 0 to
    (cheeses-1) mouse_loc[1][2] - Mouse location - there can be only one! mode -
    Search mode selection: mode = 0 	- No alpha-beta pruning mode = 1
    - Alpha-beta pruning

                 (*utility)(int cat_loc[10][2], int cheese_loc[10][2], int
    mouse_loc[1][2], int cats, int cheeses, int depth, double
    gr[graph_size][4]);
                                 - This is a pointer to the utility function
    which returns a value for a specific game configuration

                                    NOTE: Unlike the search assignment, this
    utility function also gets access to the graph so you can do any processing
    that requires knowledge of the maze for computing the utility values.

                                   * How to call the utility function from
    within this function : *
                                         - Like any other function:
                                                 u = utility(cat_loc,
    cheese_loc, mouse_loc, cats, cheeses, depth, gr);

                 agentId: Identifies which agent we are doing MiniMax for.
    agentId=0 for the mouse, agentId in [1, cats] for cats. Notice that
    recursive calls to this function should increase the agentId to reflect the
    fact that the next level down corresponds to the next agent! For a game with
    two cats and a mouse, the agentIds for the recursion should look like 0, 1,
    2, 0, 1, 2, ...

                 depth: Current search depth - whether this is a MIN or a MAX
    node depends both on depth and agentId.

                 maxDepth: maximum desired search depth - once reached, your
    code should somehow return a minimax utility value for this location.

                 alpha. beta: alpha and beta values passed from the parent node
    to constrain search at this level.

    Return values:
                 Your search code will directly update data passed-in as
    arguments:

                 - Mini-Max value	: Notice this function returns a double
    precision number. This is the minimax value at this level of the tree. It
    will be used as the recursion backtracks filling-in the mini-max values back
                                           from the leaves to the root of the
    search tree.

                 - path[1][2]		: Your MiniMax function will return the
    location for the agent's next location (i.e. the optimal move for the
    agent).
                 - minmax_cost[size_X][size_Y] 	:  Your search code will update
    this array to contain the minimax value for locations that were expanded
    during the search. This must be done *only* for the mouse.

                                                    Values in this array will be
    in the range returned by your utility function.

                 * Your code MUST NOT modify the locations or numbers of cats
    and/or cheeses, the graph, or the location of the mouse - if you try, the
    driver code will know it *

                 That's that, now, implement your solution!
  */

  /********************************************************************************************************
   *
   * TO DO:	Implement code to perform a MiniMax search. This will involve a
   *limited-depth BFS-like expansion. Once nodes below return values, your
   *function will propagate minimax utilities as per the minimax algorithm.
   *
   *		Note that if alpha-beta pruning is specified, you must keep
   *track of alphas and betas along the path.
   *
   *		You can use helper functions if it seems reasonable. Add them to
   *the MiniMax_search.h file and explain in your code why they are needed and
   *how they are used.
   *
   *		Recursion should appear somewhere.
   *
   *		MiniMax cost: If the agentId=0 (Mouse), then once you have a
   *MiniMax value for a location in the maze, you must update minmax_cost[][]
   *for that location.
   *
   *		How you design your solution is up to you. But:
   *
   *		- Document your implementation by adding concise and clear
   *comments in this file
   *		- Document your design (how you implemented the solution, and
   *why) in the report
   *
   ********************************************************************************************************/

  // Stub so that the code compiles/runs - This will be removed and replaced by
  // your code!

  // TODO(@Sam): Figure out how assigning "path" works
  // path[0][0] = mouse_loc[0][0];
  // path[0][1] = mouse_loc[0][1];
  if (mouse_loc[0][0] == -1 && mouse_loc[0][1] == -1) {
    return 0;
  }
  if (depth >= maxDepth) {
    return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
  } else if (agentId == 0) {
    // Mouse - maximizing agent
    double max_eval = -BIG_DBL;
    int next_mouse_loc[1][2];
    for (int direction = 0; direction < 4; ++direction) {
      set_next_loc(next_mouse_loc[0], mouse_loc[0], direction);
      if (!is_loc_valid(next_mouse_loc[0]) ||
          !gr[loc_to_index(mouse_loc[0])][direction]) {
        continue;
      }
      const double kEval =
          checkForTerminal(next_mouse_loc, cat_loc, cheese_loc, cats, cheeses)
              ? utility(cat_loc, cheese_loc, next_mouse_loc, cats, cheeses,
                        depth, gr)
              : MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc,
                        cheeses, next_mouse_loc, mode, utility,
                        (agentId + 1) % (1 + cats), depth + 1, maxDepth, alpha,
                        beta);
      minmax_cost[next_mouse_loc[0][0]][next_mouse_loc[0][1]] = kEval;
      if (kEval > max_eval) {
        max_eval = kEval;
        if (depth == 0) {
          path[0][0] = next_mouse_loc[0][0];
          path[0][1] = next_mouse_loc[0][1];
        }
      }
      if (mode == 1) {
        alpha = fmax(alpha, max_eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return max_eval;
  } else {
    // Cat - minimizing agent
    const int kCatIndex = agentId % cats;
    double min_eval = BIG_DBL;
    int next_cat_loc[10][2];
    memcpy(&next_cat_loc[0][0], &cat_loc[0][0], sizeof(next_cat_loc));
    for (int direction = 0; direction < 4; ++direction) {
      set_next_loc(next_cat_loc[kCatIndex], cat_loc[kCatIndex], direction);
      if (!is_loc_valid(next_cat_loc[kCatIndex]) ||
          !gr[loc_to_index(cat_loc[kCatIndex])][direction]) {
        continue;
      }
      min_eval = fmin(
          min_eval,
          checkForTerminal(mouse_loc, next_cat_loc, cheese_loc, cats, cheeses)
              ? utility(next_cat_loc, cheese_loc, mouse_loc, cats, cheeses,
                        depth, gr)
              : MiniMax(gr, path, minmax_cost, next_cat_loc, cats, cheese_loc,
                        cheeses, mouse_loc, mode, utility,
                        (agentId + 1) % (1 + cats), depth + 1, maxDepth, alpha,
                        beta));
      if (mode == 1) {
        beta = fmin(beta, min_eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    return min_eval;
  }
}

double utility(int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2],
               int cats, int cheeses, int depth, double gr[graph_size][4]) {
  /*
         This function computes and returns the utility value for a given game
     configuration. As discussed in lecture, this should return a positive value
     for configurations that are 'good' for the mouse, and a negative value for
     locations that are 'bad' for the mouse.

         How to define 'good' and 'bad' is up to you. Note that you can write a
     utility function that favours your mouse or favours the cats, but that
     would be a bad idea... (why?)

         Input arguments:

                 cat_loc - Cat locations
                 cheese_loc - Cheese locations
                 mouse_loc - Mouse location
                 cats - # of cats
                 cheeses - # of cheeses
                 depth - current search depth
                 gr - The graph's adjacency list for the maze

                 These arguments are as described in A1. Do have a look at your
     solution!
  */
  if (util_cache.cheeses != cheeses) {
    memset(util_cache.been_at, false, sizeof(util_cache.been_at));
    precompute_cheese_distance(gr, cheese_loc, cheeses);
    util_cache.cheeses = cheeses;
    util_cache.target_cheese = -1;
  }
  if (util_cache.target_cheese == -1) {
    int farthest_cat_distance = -10000;
    util_cache.target_cheese = 0;
    for (int cheese = 0; cheese < cheeses; ++cheese) {
      for (int cat = 0; cat < cats; ++cat) {
        if (util_cache.cheese_distance[cheese][loc_to_index(cat_loc[cat])] >
            farthest_cat_distance) {
          farthest_cat_distance =
              util_cache.cheese_distance[cheese][loc_to_index(cat_loc[cat])];
          util_cache.target_cheese = cheese;
        }
      }
    }
  }
  double average_cat_distance = 0;
  for (int cat = 0; cat < cats; ++cat) {
    const double kCatDistance =
        pow(pow((double)(mouse_loc[0][0] - cat_loc[cat][0]), 2) +
                pow((double)(mouse_loc[0][1] - cat_loc[cat][1]), 2),
            0.5);
    average_cat_distance += kCatDistance / ((double)cats);
  }
  Cord mouse_cord = {mouse_loc[0][0], mouse_loc[0][1]};
  if (is_cord_in_cords(mouse_cord, cat_loc, cats)) {
    return -graph_size;
  } else if (is_cord_in_cords(mouse_cord, cheese_loc, cheeses)) {
    return graph_size - depth;
  }
  return graph_size / 2 -
         util_cache.cheese_distance[util_cache.target_cheese]
                                   [loc_to_index(mouse_loc[0])] +
         average_cat_distance;
}

int checkForTerminal(int mouse_loc[1][2], int cat_loc[10][2],
                     int cheese_loc[10][2], int cats, int cheeses) {
  /*
    This function determines whether a given configuration constitutes a
    terminal node. Terminal nodes are those for which:
      - A cat eats the mouse
      or
      - The mouse eats a cheese

    If the node is a terminal, the function returns 1, else it returns 0
  */

  // Check for cats having lunch
  for (int i = 0; i < cats; i++)
    if (mouse_loc[0][0] == cat_loc[i][0] && mouse_loc[0][1] == cat_loc[i][1])
      return (1);

  // Check for mouse having lunch
  for (int i = 0; i < cheeses; i++)
    if (mouse_loc[0][0] == cheese_loc[i][0] &&
        mouse_loc[0][1] == cheese_loc[i][1])
      return (1);

  return (0);
}
