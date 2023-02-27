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

// BEGIN STRUCT HELPER FUNCTION DEFS
MinHeap* MinHeap_new(void) {
  MinHeap *min_heap = (MinHeap *)malloc(sizeof(MinHeap));
  assert(min_heap != NULL);
  min_heap->size = 0;
  return min_heap;
}

void MinHeap_insert(MinHeap* min_heap, Cord cord, int priority) {
  assert(min_heap != NULL);
  assert(min_heap->size != graph_size);
  int i;
  for (i = min_heap->size; i != 0 && min_heap->data[(i - 1) / 2].priority > priority;
       i = (i - 1) / 2) {
    min_heap->data[i] = min_heap->data[(i - 1) / 2];
  }
  min_heap->data[i].cord = cord;
  min_heap->data[i].priority = priority;
  ++min_heap->size;
}

Cord MinHeap_pop(MinHeap *min_heap) {
  assert(min_heap != NULL);
  assert(min_heap->size != 0);
  Cord cord = min_heap->data[0].cord;
  min_heap->data[0] = min_heap->data[min_heap->size - 1];
  --min_heap->size;
  for (int i = 0, l = 1, r = 2, next_i;
       l < min_heap->size &&
       min_heap->data[next_i =
                          (r >= min_heap->size || (min_heap->data[r].priority >
                                                   min_heap->data[l].priority))
                              ? l
                              : r]
               .priority < min_heap->data[i].priority;
       l = 2 * i + 1, r = 2 * i + 2, i = next_i) {
    HeapItem temp = min_heap->data[i];
    min_heap->data[i] = min_heap->data[next_i];
    min_heap->data[next_i] = temp;
  }
  return cord;
}

void MinHeap_dtor(MinHeap *min_heap) {
  free(min_heap);
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

// Return path length to closest cheese, putting into account walls and stuff.
int path_length(double gr[graph_size][4], int mouse_loc[2], int cheese_loc[2]) {
  Cord mouse_cord = {mouse_loc[0], mouse_loc[1]};
  int came_from[graph_size];
  bool visited[graph_size];
  memset(visited, false, sizeof(visited));

  MinHeap *min_heap = MinHeap_new();
  visited[cord_to_index(mouse_cord)] = true;
  MinHeap_insert(min_heap, mouse_cord, 0);

  while (min_heap->size > 0) {
    Cord cord = MinHeap_pop(min_heap);
    if (cord.x == cheese_loc[0] && cord.y == cheese_loc[1]) {
      // Found cheese - time step is done.
      const int kStartIndex = cord_to_index(mouse_cord);
      int goal_index = cord_to_index(cord);
      // find the length of the path
      int path_size;
      for (path_size = 0; goal_index != kStartIndex; ++path_size) {
        goal_index = came_from[goal_index];
      }
      MinHeap_dtor(min_heap);
      return path_size;
    }
    for (int direction = 0; direction < 4; ++direction) {
      const Cord kNextCord = get_next_cord(cord, direction);
      // Ensure valid, not visited, not wall, and not a cat.
      if (!is_cord_valid(kNextCord) || visited[cord_to_index(kNextCord)] ||
          !gr[cord_to_index(cord)][direction]) {
        continue;
      }
      visited[cord_to_index(kNextCord)] = true;
      came_from[cord_to_index(kNextCord)] = cord_to_index(cord);
      MinHeap_insert(min_heap, kNextCord, abs(kNextCord.x - mouse_loc[0]) + abs(kNextCord.y - mouse_loc[1]));
    }
  }
  MinHeap_dtor(min_heap);
  return graph_size; // Path can't be any farther than graph_size
}

// BEGIN HELPER FUNCTION DEFS
int loc_to_index(int loc[2]) { return loc[0] + loc[1] * size_X; }
void set_next_loc(int next_loc[2], int loc[1][2], int direction) {
  next_loc[0] = loc[0][0], next_loc[1] = loc[0][1];
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
   *		Note that if alpha-beta pruning is specified, you must keep track
   *of alphas and betas along the path.
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
  int checked = 0;
  if (depth >= maxDepth) {
    return utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses, depth, gr);
  } else if (agentId == 0) {
    // Mouse - maximizing agent
    double max_eval = DBL_MIN;
    int next_mouse_loc[1][2];
    for (int direction = 0; direction < 4; ++direction) {
      set_next_loc(next_mouse_loc[0], mouse_loc, direction);
      if (!is_loc_valid(next_mouse_loc[0]) || !gr[loc_to_index(mouse_loc[0])][direction]) {
        continue;
      }
      if (next_mouse_loc[0][0] == -1) {
        printf("Next mouse loc: %d %d\n", next_mouse_loc[0][0],
               next_mouse_loc[0][1]);
      }
      ++checked;
      const double kEval =
          checkForTerminal(next_mouse_loc, cat_loc, cheese_loc, cats, cheeses)
              ? utility(cat_loc, cheese_loc, next_mouse_loc, cats, cheeses,
                        depth, gr)
              : MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc,
                        cheeses, next_mouse_loc, mode, utility, (agentId + 1) % (1 + cats),
                        depth + 1, maxDepth, alpha, beta);
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
    if (depth == 0) {
      printf("Next step: %d %d | Num Cheeses: %d | Num checked locations: %d | Max Eval: %lf\n", path[0][0],
             path[0][1], cheeses, checked, max_eval);
    }
    if (checked == 0) {
      printf("CAt checked 0\n");
    }
    return max_eval;
  } else {
    // Cat - minimizing agent
    const int kCatIndex = agentId % cats;
    double min_eval = DBL_MAX;
    int prev_cat_loc[1][2] = {{cat_loc[kCatIndex][0], cat_loc[kCatIndex][1]}};
    for (int direction = 0; direction < 4; ++direction) {
      set_next_loc(cat_loc[kCatIndex], prev_cat_loc, direction);
      if (!is_loc_valid(cat_loc[kCatIndex]) || !gr[loc_to_index(prev_cat_loc[0])][direction]) {
        continue;
      }
      ++checked;
      min_eval =
          fmin(min_eval,
               checkForTerminal(mouse_loc, cat_loc, cheese_loc, cats, cheeses)
                   ? utility(cat_loc, cheese_loc, mouse_loc, cats, cheeses,
                             depth, gr)
                   : MiniMax(gr, path, minmax_cost, cat_loc, cats, cheese_loc,
                             cheeses, mouse_loc, mode, utility, (agentId + 1) % (1 + cats),
                             depth + 1, maxDepth, alpha, beta));
      cat_loc[kCatIndex][0] = prev_cat_loc[0][0];
      cat_loc[kCatIndex][1] = prev_cat_loc[0][1];
      if (mode == 1) {
        beta = fmin(beta, min_eval);
        if (beta <= alpha) {
          break;
        }
      }
    }
    if (checked == 0) {
      printf("CAt checked 0\n");
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
  double average_cat_angle=0;
  for (int cat=0; cat<cats; ++cat) {
    double dy = cat_loc[cat][0] - mouse_loc[0][0];
    double dx = cat_loc[cat][1] - mouse_loc[0][1];
    if (fabs(dx) < 0.01) {
      average_cat_angle += dy < 0 ? -M_PI / 2 : M_PI / 2;
    }
    average_cat_angle += atan(dy / dx);
  }
  int best_cheese = 0;
  double best_angle = average_cat_angle;
  for (int cheese=0; cheese<cheeses; ++cheese) {
    double angle;
    double dy = cheese_loc[cheese][0] - mouse_loc[0][0];
    double dx = cheese_loc[cheese][1] - mouse_loc[0][1];
    if (fabs(dx) < 0.01) {
      angle = dy < 0 ? -M_PI / 2 : M_PI / 2;
    } else {
      angle = atan(dy / dx);
    }
    if (fabs(angle - average_cat_angle) > fabs(best_angle - average_cat_angle)) {
      best_angle = angle;
      best_cheese = cheese;
    }
  }
  double closest_cat = graph_size;
  for (int cat = 0; cat < cats; ++cat) {
    closest_cat =
        fmin(closest_cat, pow(pow((double)(mouse_loc[0][0] - cat_loc[cat][0]), 2) +
                                  pow((double)(mouse_loc[0][1] - cat_loc[cat][1]), 2),
                              0.5));
  }
  double best_cheese_dist = path_length(gr, mouse_loc[0], cheese_loc[best_cheese]);
  // int best_cheese_dist = pow(mouse_loc[0][0] - cheese_loc[best_cheese][0], 2) +
  //                        pow(mouse_loc[0][1] - cheese_loc[best_cheese][1], 2);
  Cord mouse_cord = {mouse_loc[0][0], mouse_loc[0][1]};
  double res;
  if (is_cord_in_cords(mouse_cord, cat_loc, cats)) {
    res  = -2 * graph_size + depth;
  } else if (is_cord_in_cords(mouse_cord, cheese_loc, cheeses)) {
    if (cheeses == 1) {
      res =  3 * graph_size - depth;
    } else {
      res = 2 * graph_size - depth - 2 * best_cheese_dist + 10 * closest_cat;
    }
  } else if (best_cheese_dist < closest_cat && cheeses == 1) {
    res = 1.5 * (double)graph_size - depth - 2 * best_cheese_dist + 0.2 * closest_cat;
  } else {
    res = graph_size - depth - 2 * best_cheese_dist + 0.2 * closest_cat;
  }
    printf("%lf %lf %lf\n", best_cheese_dist, closest_cat, res);
  return res;
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
