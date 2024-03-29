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
          !gr[pos_to_index(mouse_pos[0], size_X)][action]) {
        continue;
      }
      double q_value = QTable[get_q_table_index(
          get_state_index(mouse_pos, cats, cheeses, size_X, graph_size),
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
    return get_random_legal_action(gr, mouse_pos, size_X, graph_size);
  }
}

double QLearn_reward(double gr[max_graph_size][4], int mouse_pos[1][2],
                     int cats[5][2], int cheeses[5][2], int size_X,
                     int graph_size) {
  /*
    This function computes and returns a reward for the state represented by
    the input mouse, cat, and cheese position.

    You can make this function as simple or as complex as you like. But it
    should return positive values for states that are favorable to the mouse,
    and negative values for states that are bad for the mouse.

    I am providing you with the graph, in case you want to do some processing
    on the maze in order to decide the reward.

    This function should return a maximim/minimum reward when the mouse
    eats/gets eaten respectively.
   */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  if (is_pos_in_poss(mouse_pos[0], cats)) {
    return -5.0;
  } else if (is_pos_in_poss(mouse_pos[0], cheeses)) {
    return 5.0;
  } else {
    return 0;
  }
}

void feat_QLearn_update(double gr[max_graph_size][4], double weights[25],
                        double reward, int mouse_pos[1][2], int cats[5][2],
                        int cheeses[5][2], int size_X, int graph_size) {
  /*
    This function performs the Q-learning adjustment to all the weights
    associated with your features. Unlike standard Q-learning, you don't
    receive a <s,a,r,s'> tuple, instead, you receive the current state (mouse,
    cats, and cheese potisions), and the reward associated with this action
    (this is called immediately after the mouse makes a move, so implicit in
    this is the mouse having selected some action)

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
    weights[i] += alpha * (reward + lambda * maxU - q_value) * features[i];
  }
}

int feat_QLearn_action(double gr[max_graph_size][4], double weights[25],
                       int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
                       double pct, int size_X, int graph_size) {
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
    // Explore
    return get_random_legal_action(gr, mouse_pos, size_X, graph_size);
  }
}

void evaluateFeatures(double gr[max_graph_size][4], double features[25],
                      int mouse_pos[1][2], int cats[5][2], int cheeses[5][2],
                      int size_X, int graph_size) {
  /*
   This function evaluates all the features you defined for the game
   configuration given by the input mouse, cats, and cheese positions. You are
   free to define up to 25 features. This function will evaluate each, and
   return all the feature values in the features[] array.

   Take some time to think about what features would be useful to have, the
   better your features, the smarter your mouse!

   Note that instead of passing down the number of cats and the number of
   cheese chunks (too many parms!) the arrays themselves will tell you what
   are valid cat/cheese locations.

   You can have up to 5 cats and up to 5 cheese chunks, and array entries for
   the remaining cats/cheese will have a value of -1 - check this when
   evaluating your features!
  */

  /***********************************************************************************************
   * TO DO: Complete this function
   ***********************************************************************************************/
  // Closest Cheese Dist Feature
  features[0] = get_scaled_closest_distance(gr, mouse_pos, cheeses, size_X, graph_size);
  // Closest Cat Dist Feature
  features[1] = get_scaled_closest_distance(gr, mouse_pos, cats, size_X, graph_size);
  // Avg Cat Dist Feature
  features[2] = avg_cat_feat(gr, mouse_pos, cats, size_X, graph_size);
  // Is dead_end Feature

  double best_angle = M_PI;
  for (int num_cheese = 0; cheeses[num_cheese][0] != -1; ++num_cheese) {
    best_angle = fmin(best_angle, angle(mouse_pos, cats, cheeses[num_cheese]));
  }
}

double Qsa(double weights[25], double features[25]) {
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

void maxQsa(double gr[max_graph_size][4], double weights[25],
            int mouse_pos[1][2], int cats[5][2], int cheeses[5][2], int size_X,
            int graph_size, double *maxU, int *maxA) {
  /*
    Given the state represented by the input positions for mouse, cats, and
    cheese, this function evaluates the Q-value at all possible neighbour
    states and returns the max. The maximum value is returned in maxU and the
    index of the action corresponding to this value is returned in maxA.

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
    double features[25];
    set_next_pos(next_mouse_pos[0], mouse_pos[0], action);
    if (!is_pos_valid(next_mouse_pos[0], size_X, size_Y) ||
        !gr[pos_to_index(mouse_pos[0], size_X)][action]) {
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
  memcpy(&next_pos[0], &pos[0], 2 * sizeof(int));
  switch (direction) {
  case UP:
    --next_pos[1];
    break;
  case DOWN:
    ++next_pos[1];
    break;
  case LEFT:
    --next_pos[0];
    break;
  case RIGHT:
    ++next_pos[0];
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

// Feature: Returns the avg distance to the cats from the mouse between [-1, 1]
double avg_cat_feat(double gr[max_graph_size][4], int mouse_pos[1][2],
                    int cats[5][2], int size_X, int graph_size) {
  // -1 is terrible, aka cats are right on top
  // 1 is great, aka cats are across the map from the mouse
  double max_dist =
      sqrt(pow((double)(size_X), 2) + pow((double)(graph_size / size_X), 2));
  double average_cat_distance = 0;
  int num_cat;
  for (num_cat = 0; cats[num_cat][0] != -1; ++num_cat) {
    average_cat_distance +=
        sqrt(pow((double)(mouse_pos[0][0] - cats[num_cat][0]), 2) +
             pow((double)(mouse_pos[0][1] - cats[num_cat][1]), 2));
    ;
  }
  // Get avg distance and normalize on max distance to [0,1]
  average_cat_distance /= num_cat * max_dist;
  // manipulate range to [-0.5, 0.5] *2 = [-1, 1]
  return (average_cat_distance - 0.5) * 2;
}

/* Feature: Returns the distance to the closest agent in agents from the mouse
 between [0, 1], Possible agents: Cat, Cheese*/
double get_scaled_closest_distance(double gr[max_graph_size][4], int mouse_pos[1][2],
                    int agents[5][2], int size_X, int graph_size) {
  if (is_pos_in_poss(mouse_pos[0], agents)) {
    return 0;
  }
  int size_Y = graph_size / size_X;
  bool seen[graph_size];
  memset(seen, false, sizeof(seen));
  Deque* deque = Deque_new();
  seen[pos_to_index(mouse_pos[0], size_X)] = true;
  Deque_push_front(deque, mouse_pos[0][0], mouse_pos[0][1], 0);
  while (deque->size > 0) {
    DequeItem deque_item = Deque_pop_back(deque);
    int pos[2] = {deque_item.x, deque_item.y};
    for (int direction=0; direction <4; ++direction) {
      int next_pos[2];
      set_next_pos(next_pos, pos, direction);
      if (!is_pos_valid(next_pos, size_X, size_Y) ||
          seen[pos_to_index(next_pos, size_X)] ||
          !gr[pos_to_index(pos, size_X)][direction]) {
        continue;
      }
      if (is_pos_in_poss(next_pos, agents)) {
        Deque_dtor(deque);
        return (double)(deque_item.distance + 1) / (double)graph_size;
      }
      seen[pos_to_index(next_pos, size_X)] = true;
      Deque_push_front(deque, next_pos[0], next_pos[1], deque_item.distance + 1);
    }
  }
  Deque_dtor(deque);
  return -1;
}

// Check if currenty at dead end
bool dead_end(double gr[max_graph_size][4], int mouse_pos[1][2], int size_X) {
  int walls = 0;
  for (int i = 0; i < 4; ++i) {
    walls += gr[pos_to_index(mouse_pos[0], size_X)][i];
  }
  return walls > 2;
}

// Define a function that calculates the angle between three points
double angle(int mouse_pos[1][2], int cats[5][2], int cheese[2]) {
  int num_c = 0, c_x = 0, c_y = 0;
  for (num_c = 0; cats[num_c][0] != -1; ++num_c) {
    c_x += cats[num_c][0];
    c_y += cats[num_c][1];
  }

  // Calculate the lengths of the sides
  double mx = ((double)c_x / (double)num_c) - mouse_pos[0][0];
  double my = ((double)c_y / (double)num_c) - mouse_pos[0][1];
  double cx = cheese[0] - mouse_pos[0][0];
  double cy = cheese[1] - mouse_pos[0][1];
  double angle = atan2(my, mx) - atan2(cy, cx);

  // Take acute angle
  return (angle > M_PI) ? (2 * M_PI - angle) : angle;
}

int get_random_legal_action(double gr[max_graph_size][4], int mouse_pos[1][2],
                            int size_X, int graph_size) {
  int size_Y = graph_size / size_X;
  int legal_actions[4];
  int num_legal_actions = 0;
  int next_mouse_pos[1][2];
  for (int action = 0; action < 4; ++action) {
    set_next_pos(next_mouse_pos[0], mouse_pos[0], action);
    if (!is_pos_valid(next_mouse_pos[0], size_X, size_Y) ||
        !gr[pos_to_index(mouse_pos[0], size_X)][action]) {
      continue;
    }
    legal_actions[num_legal_actions] = action;
    ++num_legal_actions;
  }
  return legal_actions[(int)get_random_uniform(0, num_legal_actions - EPSILON)];
}

DequeItem *DequeItem_new(int x, int y, int distance) {
  DequeItem *deque_item = (DequeItem*)malloc(sizeof(DequeItem));
  assert(deque_item != NULL);
  deque_item->x = x;
  deque_item->y = y;
  deque_item->distance = distance;
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

void Deque_push_front(Deque *deque, int x, int y, int distance) {
  assert(deque != NULL);
  DequeItem *deque_item = DequeItem_new(x, y, distance);
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

DequeItem Deque_pop_back(Deque* deque) {
  assert(deque != NULL);
  assert(deque->size != 0);
  DequeItem* deque_item = deque->tail;
  if (deque->head == deque->tail) {
    deque->head = deque->tail = NULL;
  } else {
    deque->tail = deque->tail->prev;
  }
  DequeItem res = *deque_item;
  free(deque_item);
  --deque->size;
  return res;
}

void Deque_dtor(Deque *deque) {
  assert(deque != NULL);
  while (deque->size > 0) {
    Deque_pop_back(deque);
  }
  free(deque);
}
