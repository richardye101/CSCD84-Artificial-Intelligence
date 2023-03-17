/*
        CSC D84 - Unit 3 - Reinforcement Learning

        This file contains stubs for implementing the Q-Learning method
        for reinforcement learning as discussed in lecture. You have to
        complete two versions of Q-Learning.

        * Standard Q-Learning, based on a full-state representation and
          a large Q-Table
        * Feature based Q-Learning to handle problems too big to allow
          for a full-state representation

        Read the assignment handout carefully, then implement the
        required functions below. Sections where you have to add code
        are marked

        **************
        *** TO DO:
        **************

        If you add any helper functions, make sure you document them
        properly and indicate in the report.txt file what you added.

        Have fun!

        DO NOT FORGET TO 'valgrind' YOUR CODE - We will check for pointer
        management being done properly, and for memory leaks.

        Starter code: F.J.E. Jan. 16
*/

#include "QLearn.h"

void QLearn_update(int s, int a, double r, int s_new, double *QTable) {
  /*
    This function implementes the Q-Learning update as stated in Lecture. It
    receives as input a <s,a,r,s'> tuple, and updates the Q-table accordingly.

    Your work here is to calculate the required update for the Q-table entry
    for state s, and apply it to the Q-table

    The update involves two constants, alpha and lambda, which are defined in
    QLearn.h - you should not have to change their values. Use them as they are.

    Details on how states are used for indexing into the QTable are shown
    below, in the comments for QLearn_action. Be sure to read those as well!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  int next_mouse_pos[1][2];
  double best_q_value = -BIG_DBL;
  for (int action = 0; action < 4; ++action) {
    set_next_pos(next_mouse_pos, mouse_pos, action);
    if (!is_pos_valid(next_mouse_pos[0], size_X, size_Y) ||
        !gr[pos_to_index(next_mouse_pos[0], size_X)]) {
      continue;
    }
    best_q_value = fmax(best_q_value, QTable[get_q_table_index(s_new, action)]);
  }
  assert(best_q_value != -BIG_DBL);
  QTable[get_q_table_index(s, a)] +=
      alpha * (r + lambda * (best_q_value - QTable[get_q_table_index(s, a)]));
}

int QLearn_action(double gr[max_graph_size][4], int mouse_pos[1][2],
                  int cats[5][2], int cheeses[5][2], double pct, double *QTable,
                  int size_X, int graph_size) {
  /*
     This function decides the action the mouse will take. It receives as inputs
     - The graph - so you can check for walls! The mouse must never move through
   walls
     - The mouse position
     - The cat position
     - The chees position
     - A 'pct' value in [0,1] indicating the amount of time the mouse uses the
   QTable to decide its action, for example, if pct=.25, then 25% of the time
   the mouse uses the QTable to choose its action, the remaining 75% of the time
   it chooses randomly among the available actions.

     Remember that the training process involves random exploration initially,
   but as training proceeds we use the QTable more and more, in order to improve
   our QTable values around promising actions.

     The value of pct is controlled by QLearn_core_GL, and increases with each
   round of training.

     This function *must return* an action index in [0,3] where
        0 - move up
        1 - move right
        2 - move down
        3 - move left

     QLearn_core_GL will print a warning if your action makes the mouse cross a
   wall, or if it makes the mouse leave the map - this should not happen. If you
   see a warning, fix the code in this function!

   The Q-table has been pre-allocated and initialized to 0. The Q-table has
   a size of

        graph_size^3 x 4

   This is because the table requires one entry for each possible state, and
   the state is comprised of the position of the mouse, cat, and cheese.
   Since each of these agents can be in one of graph_size positions, all
   possible combinations yield graph_size^3 states.

   Now, for each state, the mouse has up to 4 possible moves (up, right,
   down, and left). We ignore here the fact that some moves are not possible
   from some states (due to walls) - it is up to the QLearn_action() function
   to make sure the mouse never crosses a wall.

   So all in all, you have a big table.

   For example, on an 8x8 maze, the Q-table will have a size of

       64^3 x 4  entries

       with

       size_X = 8		<--- size of one side of the maze
       graph_size = 64		<--- Total number of nodes in the graph

   Indexing within the Q-table works as follows:

     say the mouse is at   i,j
         the cat is at     k,l
         the cheese is at  m,n

     state = (i+(j*size_X)) + ((k+(l*size_X))*graph_size) +
   ((m+(n*size_X))*graph_size*graph_size)
     ** Make sure you undestand the state encoding above!

     Entries in the Q-table for this state are

     *(QTable+(4*state)+a)      <-- here a is the action in [0,3]

     (yes, it's a linear array, no shorcuts with brackets!)

     NOTE: There is only one cat and once cheese, so you only need to use
   cats[0][:] and cheeses[0][:]
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  if (get_random_uniform(0, 1) <= pct) {
    // Exploit
    int size_Y = graph_size / size_X;
    int best_action = -1;
    double best_q_value = -BIG_DBL;
    for (int action = 0; action < numActions; ++action) {
      int next_mouse_pos[1][2];
      set_next_pos(next_mouse_pos[0], mouse_pos[0], action);
      if (!is_pos_valid(next_mouse_pos[0], size_X, size_Y) ||
          !gr[pos_to_index(next_mouse_pos[0], size_X)]) {
        continue;
      }
      double q_value = QTable[get_q_table_index(
          get_state_index(next_mouse_pos, cats, cheeses, size_X, graph_size),
          action)];
      if (q_value > best_q_value) {
        best_q_value = q_value;
        best_action = action;
      }
    }
    assert(0 <= best_action && best_action < numActions);
    return best_action;
  } else {
    // Explore
    return (int)get_random_uniform(0, 4 - EPSILON);
  }
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2],
                     int cats[5][2], int cheeses[5][2], int size_X,
                     int graph_size) {
  /*
    This function computes and returns a reward for the state represented by the
    input mouse, cat, and cheese position.

    You can make this function as simple or as complex as you like. But it
    should return positive values for states that are favorable to the mouse,
    and negative values for states that are bad for the mouse.

    I am providing you with the graph, in case you want to do some processing on
    the maze in order to decide the reward.

    This function should return a maximim/minimum reward when the mouse
    eats/gets eaten respectively.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  if (is_pos_in_poss(mouse_pos[0], cats)) {
    return -graph_size;
  } else if (is_loc_in_locs(mouse_pos[0], cheeses, 1)) {
    return graph_size;
  } else {
    if (deadEnd(gr, mouse_pos, size_X)) {
      return -3;
    }
    return (0); // <--- of course, you will change this as well!
  }
}

void feat_QLearn_update(double gr[max_graph_size][4],
                        double weights[numFeatures], double reward,
                        int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
                        int size_X, int graph_size) {
  /*
    This function performs the Q-learning adjustment to all the weights
    associated with your features. Unlike standard Q-learning, you don't receive
    a <s,a,r,s'> tuple, instead, you receive the current state (mouse, cats, and
    cheese potisions), and the reward associated with this action (this is
    called immediately after the mouse makes a move, so implicit in this is the
    mouse having selected some action)

    Your code must then evaluate the update and apply it to the weights in the
    weight array.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double maxU; // maxQsa value
  int maxA;    // maxQsa action, not required here?
  double features[numFeatures];
  double q_value;

  maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU,
         &maxA);
  evaluateFeatures(gr, features, mouse_pos, cats, cheeses, size_X, graph_size);
  q_value = Qsa(weights, features);

  for (int i = 0; i < numFeatures; ++i) {
    weights[i] += alpha * (reward + lambda * (maxU - q_value)) * features[i];
  }
}

int feat_QLearn_action(double gr[max_graph_size][4],
                       double weights[numFeatures], int mouse_pos[1][2],
                       int cats[5][2], int cheeses[5][2], double pct,
                       int size_X, int graph_size) {
  /*
    Similar to its counterpart for standard Q-learning, this function returns
    the index of the next action to be taken by the mouse.

    Once more, the 'pct' value controls the percent of time that the function
    chooses an optimal action given the current policy.

    E.g. if 'pct' is .15, then 15% of the time the function uses the current
    weights and chooses the optimal action. The remaining 85% of the time, a
    random action is chosen.

    As before, the mouse must never select an action that causes it to walk
    through walls or leave the maze.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  if (get_random_uniform(0, 1) <= pct) {
    // Exploit
    double maxU; // not required here
    int maxA;
    maxQsa(gr, weights, mouse_pos, cats, cheeses, size_X, graph_size, &maxU,
           &maxA);
    return maxA;
  } else {
    return (int)get_random_uniform(0, 4 - EPSILON);
  }
}

void evaluateFeatures(double gr[max_graph_size][4],
                      double features[numFeatures], int mouse_pos[1][2],
                      int cats[5][2], int cheeses[5][2], int size_X,
                      int graph_size) {
  /*
   This function evaluates all the features you defined for the game
   configuration given by the input mouse, cats, and cheese positions. You are
   free to define up to 25 features. This function will evaluate each, and
   return all the feature values in the features[] array.

   Take some time to think about what features would be useful to have, the
   better your features, the smarter your mouse!

   Note that instead of passing down the number of cats and the number of cheese
   chunks (too many parms!) the arrays themselves will tell you what are valid
   cat/cheese locations.

   You can have up to 5 cats and up to 5 cheese chunks, and array entries for
   the remaining cats/cheese will have a value of -1 - check this when
   evaluating your features!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  // Avg Cat Dist Feature
  features[0] = avg_cat_feat(gr, features, mouse_pos, cats, size_X, graph_size);
  // Closest Cat Dist Feature
  features[1] = closest_dist(gr, features, mouse_pos, cats, size_X, graph_size);
  // Closest Cheese Dist Feature
  features[2] =
      closest_dist(gr, features, mouse_pos, cheeses, size_X, graph_size);
}

double Qsa(double weights[numFeatures], double features[numFeatures]) {
  /*
    Compute and return the Qsa value given the input features and current
    weights
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  double res = 0;
  for (int i = 0; i < numFeatures; ++i) {
    res += weights[i] * features[i];
  }
  return res;
}

void maxQsa(double gr[max_graph_size][4], double weights[numFeatures],
            int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X,
            int graph_size, double *maxU, int *maxA) {
  /*
    Given the state represented by the input positions for mouse, cats, and
    cheese, this function evaluates the Q-value at all possible neighbour states
    and returns the max. The maximum value is returned in maxU and the index of
    the action corresponding to this value is returned in maxA.

    You should make sure the function does not evaluate moves that would make
    the mouse walk through a wall.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  *maxU = -BIG_DBL;
  *maxA = -1;
  int size_Y = graph_size / size_X;
  int next_mouse_pos[1][2];
  for (int action = 0; action < numActions; ++action) {
    double features[numFeatures];
    set_next_pos(next_mouse_pos[0], mouse_pos[0], action);
    if (!is_pos_valid(next_mouse_pos[0], size_X, size_Y) ||
        !gr[pos_to_index(next_mouse_pos[0], size_X)]) {
      continue;
    }
    evaluateFeatures(gr, features, next_mouse_pos, cats, cheeses, size_X,
                     graph_size);
    double q_value = Qsa(weights, features);
    if (q_value > *maxU) {
      *maxU = q_value;
      *maxA = action;
    }
  }
  assert(0 <= *maxA && *maxA < numActions);
}

/***************************************************************************************************
 *  Add any functions needed to compute your features below
 *                 ---->  THIS BOX <-----
 * *************************************************************************************************/
// Return random uniform double value between min and max
double get_random_uniform(double min, double max) {
  assert(min < max);
  double res = min + (max - min) * drand48();
  // Ensure not outside of range by some floating point precision epsilon.
  return (res < min) ? min : ((res > max) ? max : res);
}

// Sets pos to correct value if step taken in direction direction.
void set_next_pos(int next_pos[2], int pos[2], int direction) {
  assert(0 <= direction && direction < 4);
  memcpy(next_pos, pos, sizeof(pos));
  switch (direction) {
  case UP:
    --pos[1];
    break;
  case DOWN:
    ++pos[1];
    break;
  case LEFT:
    --pos[0];
    break;
  case RIGHT:
    ++pos[0];
    break;
  default:
    break;
  }
}

// Return if poss contains a position equal to pos, false otherwise.
bool is_pos_in_poss(int pos[2], int poss[5][2]) {
  for (int i = 0; i < 5; ++i) {
    if (poss[i][0] == -1) {
      return false;
    } else if (pos[0] == poss[i][0] && pos[1] == poss[i][1]) {
      return true;
    }
  }
  return false;
}

// Determines if mouse on cheese?
bool is_loc_in_locs(int loc[2], int locs[][2], int num_locs) {
  // @Sam to-do
}

// Return the state index given the provided entity locations.
int get_state_index(int mouse_loc[1][2], int cats[5][2], int cheeses[5][2],
                    int size_X, int graph_size) {
  return (mouse_loc[0][0] + (mouse_loc[0][1] * size_X)) +
         graph_size * (cats[0][0] + (cats[0][1] * size_X)) +
         graph_size * graph_size * (cheeses[0][0] + (cheeses[0][1] * size_X));
}

// Return the q table index provided the entity locations and action.
int get_q_table_index(int state, int action) {
  assert(0 <= action && action < 4);
  return 4 * state + action;
}

int pos_to_index(int pos[2], int size_X) { return pos[0] + pos[1] * size_X; }

int is_pos_valid(int pos[2], int size_X, int size_Y) {
  return 0 <= pos[0] && pos[0] < size_X && 0 <= pos[1] && pos[1] < size_Y;
}

bool deadEnd(double gr[max_graph_size][4], int mouse_pos[1][2], int size_X) {
  int walls = 0;
  for (int i = 0; i < 4; ++i) {
    walls += gr[pos_to_index(mouse_pos[0], size_X)][i];
  }
  if (walls > 2) {
    return (true);
  }
  return (false);
}

// Avg Cat Dist Feature
double avg_cat_feat(double gr[max_graph_size][4], double features[numFeatures],
                    int mouse_pos[1][2], int cats[5][2], int size_X,
                    int graph_size) {
  // -1 is terrible, aka cats are right on top
  // 1 is great, aka cats are across the map from the mouse
  double max_dist = pow(
      pow((double)(size_X), 2) + pow((double)(graph_size / size_X), 2), 0.5);
  double average_cat_distance = 0;
  int num_cat = 0;
  while (cats[num_cat][0] != -1) {
    const double kCatDistance =
        pow(pow((double)(mouse_pos[0][0] - cats[num_cat][0]), 2) +
                pow((double)(mouse_pos[0][1] - cats[num_cat][1]), 2),
            0.5);
    average_cat_distance += kCatDistance;
    ++num_cat;
  }
  // Get avg distance and normalize on max distance to [0,1]
  average_cat_distance /= num_cat * max_dist;
  // manipulate range to [-0.5, 0.5] *2 = [-1, 1]
  return (average_cat_distance - 0.5) * 2;
}

// Closest Cat Dist Feature and Closest Cheese Dist Feature
double closest_dist(double gr[max_graph_size][4], double features[numFeatures],
                    int mouse_pos[1][2], int agents[5][2], int size_X,
                    int graph_size) {
  // -1 is terrible, aka cats are right on top
  // 1 is great, aka cats are across the map from the mouse
  double max_dist = pow(
      pow((double)(size_X), 2) + pow((double)(graph_size / size_X), 2), 0.5);
  double closest_dist = max_dist;
  int num_agent = 0;
  while (agents[num_agent][0] != -1) {
    double dist =
        pow(pow((double)(mouse_pos[0][0] - agents[num_agent][0]), 2) +
                pow((double)(mouse_pos[0][1] - agents[num_agent][1]), 2),
            0.5);
    closest_dist = fmin(dist, closest_dist);
    ++num_agent;
  }
  closest_dist /= max_dist;
  return (closest_dist - 0.5) * 2;
}