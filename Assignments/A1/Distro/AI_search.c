/*
	CSC D84 - Unit 1 - Search

	This file contains stubs for implementing the different search
	algorithms covered in the course. Please read the assignment
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

	Starter code: F.J.E., Jul. 15
	Updated: F.J.E., Jan. 18
*/

/**********************************************************************
% COMPLETE THIS TEXT BOX:
%
% 1) Student Name:		
% 2) Student Name:		
%
% 1) Student number:
% 2) Student number:
% 
% 1) UtorID
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% ____________________             _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/


#include "AI_search.h"

// BEGIN STRUCT HELPER FUNCTION DEFS
DequeItem *DequeItem_new(Cord cord) {
  DequeItem *deque_item = (DequeItem*)malloc(sizeof(DequeItem));
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

MinHeap *Heap_new(void) {
  MinHeap *min_heap = (MinHeap *)malloc(sizeof(MinHeap));
  assert(min_heap != NULL);
  min_heap->size = 0;
  return min_heap;
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

Cord Deque_pop_back(Deque* deque) {
  assert(deque != NULL);
  assert(deque->size != 0);
  Cord cord = deque->tail->cord;
  DequeItem* deque_item = deque->tail;
  if (deque->head == deque->tail) {
    deque->head = deque->tail = NULL;
  } else {
    deque->tail = deque->tail->prev;
  }
  free(deque_item);
  --deque->size;
  return cord;
}

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
DataStructure* DataStructure_new(int mode, HEURISTIC_FUNCTION heuristic) {
  DataStructure *data_structure =
      (DataStructure *)malloc(sizeof(DataStructure));
  assert(data_structure != NULL);
  data_structure->deque = Deque_new();
  data_structure->min_heap = MinHeap_new();
  data_structure->mode = mode;
  data_structure->heuristic = heuristic;
  return data_structure;
}
void DataStructure_insert(DataStructure *data_structure, Cord cord,
                          int priority) {
  // Note priority only matters if mode == MODE_A_STAR
  assert(data_structure != NULL);
  assert(data_structure->mode == MODE_BFS || data_structure->mode == MODE_DFS ||
         data_structure->mode == MODE_A_STAR);
  switch (data_structure->mode) {
    case MODE_BFS:
      // Stack
      Deque_push_back(data_structure->deque, cord);
      break;
    case MODE_DFS:
      // Queue
      Deque_push_back(data_structure->deque, cord);
      break;
    case MODE_A_STAR:
      // MinHeap
      MinHeap_insert(data_structure->min_heap, cord, priority);
      break;
    default:
      break;
  }
}
Cord DataStructure_pop(DataStructure *data_structure) {
  assert(data_structure != NULL);
  assert(data_structure->mode == MODE_BFS || data_structure->mode == MODE_DFS ||
         data_structure->mode == MODE_A_STAR);
  Cord cord;
  switch(data_structure->mode) {
    case MODE_BFS:
      // Stack
      cord = Deque_pop_back(data_structure->deque);
      break;
    case MODE_DFS:
      // Queue
      cord = Deque_pop_front(data_structure->deque);
      break;
    case MODE_A_STAR:
      // MinHeap
      cord = MinHeap_pop(data_structure->min_heap);
      break;
    default:
      break;
  }
  return cord;
}
int DataStructure_size(DataStructure* data_structure) {
  assert(data_structure != NULL);
  assert(data_structure->mode == MODE_BFS || data_structure->mode == MODE_DFS ||
         data_structure->mode == MODE_A_STAR);
  switch (data_structure->mode) {
    case MODE_BFS:
      // Stack
      return data_structure->deque->size;
    case MODE_DFS:
      // Queue
      return data_structure->deque->size;
    case MODE_A_STAR:
      // MinHeap
      return data_structure->min_heap->size;
    default:
      break;
  }
  return -1; // This will never run - but put here to make compiler happy.
}
void DataStructure_dtor(DataStructure* data_structure) {
  assert(data_structure != NULL);
  while (DataStructure_size(data_structure) > 0) {
    DataStructure_pop(data_structure);
  }
  free(data_structure->deque);
  free(data_structure->min_heap);
  free(data_structure);
}
// END STRUCT HELPER FUNCTION DEFS

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
void construct_path(int path[graph_size][2], int came_from[graph_size],
                    const Cord start, const Cord goal) {
  const int start_index = cord_to_index(start);
  const int goal_index = cord_to_index(goal);
  int index = goal_index;
  // find the length of the path
  int path_size;
  for (path_size = 1; index != start_index; ++path_size) {
    index = came_from[index];
  }
  index = goal_index;
  // construct the path from goal node to start node
  for (int i=path_size-1; i >= 0; --i) {
    const Cord cord = index_to_cord(index);
    path[i][0] = cord.x;
    path[i][1] = cord.y;
    index = came_from[index];
  }
  // UNCOMMNENT TO PRINT THE FULL PATH:
  // for (int i=0; i<path_size; ++i) {
  //   printf("(%d, %d), ", path[i][0], path[i][1]);
  // }
  // printf("\n");
  // printf("---------------------------------------------------\n");
}
// END HELPER FUNCTION DEFS

void search(double gr[graph_size][4], int path[graph_size][2],
            int visit_order[size_X][size_Y], int cat_loc[10][2], int cats,
            int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode,
            int (*heuristic)(int x, int y, int cat_loc[10][2],
                             int cheese_loc[10][2], int mouse_loc[1][2],
                             int cats, int cheeses, double gr[graph_size][4])) {
  /*
    This function is the interface between your solution for the assignment and
    the driver code. The driver code in AI_search_core_GL will call this
    function once per frame, and provide the following data

    Board and game layout:

         The game takes place on a grid of size 32x32, the file board_layout.h
    specifies the size and defines two constants 'size_X' and 'size_Y' for the
    horizontal and vertical size of the board, respectively. For our purposes,
    the grid of locations is represented by a graph with one node per grid
    location, so given the 32x32 cells, the graph has 1024 nodes.

         To create a maze, we connect cell locations in the grid in such a way
    that a) there is a path from any grid location to any other grid location
    (i.e. there are no disconnected subsets of nodes in the graph), and b) there
    are loops.

         Since each node represents a grid location, each node can be connected
    to up to 4 neighbours in the top, right, bottom, and left directions
    respectively:

                         node at (i,j-1)
                                 ^
                                 |
         (node at i-1, j) <- node at (i,j) -> node at (i+1, j)
                                 |
                                 v
                         node at (i,j+1)

         The graph is therefore stored as an adjacency list with size 1024 x 4,
    with one row per node in the graph, and 4 columns corresponding to the
    weight of an edge linking the node with each of its 4 possible neighbours in
    the order top, right, bottom, left (clockwise from top).

         Since all we care is whether nodes are connected. Weights will be
    either 0 or 1, if the weight is 1, then the neighbouring nodes are
    connected, if the weight is 0, they are not. For example, if

         graph[i][0] = 0
         graph[i][1] = 1
         graph[i][2] = 0
         graph[i][3] = 1

         then node i is connected to the right and left neighbours, but not to
    top or bottom.

         The index in the graph for the node corresponding to grid location
    (x,y) is

         index = x + (y*size_X) 		or in this case		index =
    x + (y*32)

         Conversely, if you have the index and want to figure out the grid
    location,

         x = index % size_X		or in this case		x = index % 32
         y = index / size_Y		or in this case		y = index / 32

         (all of the above are *integer* operations!)

         A path is a sequence of (x,y) grid locations. We store it using an
    array of size 1024 x 2 (since there are 1024 locations, this is the maximum
    length of any path that visits locations only once).

         Agent locations are coordinate pairs (x,y)

    Arguments:
                 gr[graph_size][4]   - This is an adjacency list for the maze
                 path[graph_size][2] - An initially empty path for your code to
    fill. In this case, empty means all entries are initially -1
                 visit_order[size_X][size_Y]	- An array in which your code
    will store the *order* in which grid locations were visited during search.
    For example, while doing BFS, the initial location is the start location,
    it's visit order is 1. Then the search would expand the immediate neighbours
    of the initial node in some order, these would get a visit order of 2, 3, 4,
    and 5 respectively, and so on.

                                                   This array will be used to
    display the search pattern generated by each search method.

                 cat_loc[10][2], cats   - Location of cats and number of cats
    (we can have at most 10, but there can be fewer). Only valid cat locations
    are 0 to (cats-1) cheese_loc[10][2], cheeses - Location and number of cheese
    chunks (again at most 10, but possibly fewer). Valid locations are 0 to
    (cheeses-1) mouse_loc[1][2] - Mouse location - there can be only one! mode -
    Search mode selection: mode = 0 	- BFS mode = 1	- DFS mode = 2	- A*

                 (*heuristic)(int x, int y, int cat_loc[10][2], int
    cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
                                 - This is a pointer to one of the heuristic
    functions you will implement, either H_cost() or H_cost_nokitty(). The
    driver in AI_search_core_GL will pass the appropriate pointer depending on
    what search the user wants to run.

                                   If the mode is 0 or 1, this pointer is NULL

                                   * How to call the heuristic function from
    within this function : *
                                         - Like any other function:
                                                 h = heuristic( x, y, cat_loc,
    cheese_loc, mouse_loc, cats, cheeses);

    Return values:
                 Your search code will directly update data passed-in as
    arguments:

                 - path[graph_size][2]	: Your search code will update this
    array to contain the path from the mouse to one of the cheese chunks. The
    order matters, so path[0][:] must be the mouse's current location,
    path[1][:] is the next move for the mouse. Each successive row will contain
                                           the next move toward the cheese, and
    the path ends at a location whose coordinates correspond to one of the
    cheese chunks. Any entries beyond that must remain set to -1
                 - visit_order[size_X][size_Y] 	:  Your search code will update
    this array to contain the order in which each location in the grid was
    expanded during search. This means, when that particular location was
    checked for being a goal, and if not a goal, had its neighbours added as
    candidates for future expansion (in whatever order is dictated by the search
    mode).

                                                    Note that since there are
    1024 locations, the values in this array must always be in [0, 1023]. The
    driver code will then display search order as a yellow-colored brightness
    map where nodes expanded earlier will look brighter.

                 * Your code MUST NOT modify the locations or numbers of cats
    and/or cheeses, the graph, or the location of the mouse - if you try, the
    driver code will know it *

                 That's that, now, implement your solution!
  */

  /********************************************************************************************************
   *
   * TO DO:	Implement code to carry out the different types of search
   *depending on the mode.
   *
   *		You can do this by writing code within this single function (and
   *being clever about it!) Or, you can use this function as a wrapper that
   *calls a different search function (BFS, DFS, A*) depending on the mode. Note
   *that in the latter case, you will have to inform your A* function somehow of
   *what heuristic it's supposed to use.
   *
   *		Visiting Order: When adding the neighbours of a node to your list of
   *candidates for expansion, do so in the order top, right, bottom, left.
   *
   *		NOTE: Your search functions should be smart enough to not choose a
   *path that goes through a cat! this is easily done without any heuristics.
   *
   *		How you design your solution is up to you. But:
   *
   *		- Document your implementation by adding concise and clear comments
   *in this file
   *		- Document your design (how you implemented the solution, and why)
   *in the report
   *
   ********************************************************************************************************/

  // Stub so that the code compiles/runs - The code below will be removed and
  // replaced by your code!

  Cord mouse_cord = (Cord){mouse_loc[0][0], mouse_loc[0][1]};
  int came_from[graph_size];
  memset(came_from, -1, sizeof(came_from));
  bool visited[graph_size];
  memset(visited, false, sizeof(visited));
  visited[cord_to_index(mouse_cord)] = true;
  DataStructure *data_structure = DataStructure_new(mode, heuristic);
  // Use 0 priority for starting node
  DataStructure_insert(data_structure, mouse_cord, 0);
  int visit_counter = 1;
  while (DataStructure_size(data_structure) > 0) {
    Cord cord = DataStructure_pop(data_structure);
    visit_order[cord.x][cord.y] = visit_counter;
    ++visit_counter;
    bool found_cheese = false;
    for (int cheese = 0; cheese < cheeses; ++cheese) {
      if (cord.x == cheese_loc[cheese][0] && cord.y == cheese_loc[cheese][1]) {
        // Found cheese - time step is done.
        found_cheese = true;
        construct_path(path, came_from, mouse_cord,
                       (Cord){cheese_loc[cheese][0], cheese_loc[cheese][1]});
        DataStructure_dtor(data_structure);
        return;
      }
    }
    for (int direction = 0; direction < 4; ++direction) {
      const Cord next_cord = get_next_cord(cord, direction);
      if (!is_cord_valid(next_cord) || visited[cord_to_index(next_cord)]) {
        continue;
      }
      bool is_cat = false;
      for (int cat = 0; cat < cats; ++cat) {
        if (equal_cords(next_cord, (Cord){cat_loc[cat][0], cat_loc[cat][1]})) {
          is_cat = true;
          break;
        }
      }
      // Ensure no wall and not a cat
      if (gr[cord_to_index(cord)][direction] && !is_cat) {
        visited[cord_to_index(next_cord)] = true;
        int h = (heuristic == NULL)
                    ? 0
                    : heuristic(next_cord.x, next_cord.y, cat_loc, cheese_loc,
                                mouse_loc, cats, cheeses, gr);
        DataStructure_insert(data_structure, next_cord, h);
        came_from[cord_to_index(next_cord)] = cord_to_index(cord);
      }
    }
  }
  DataStructure_dtor(data_structure);
  return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2],
           int mouse_loc[1][2], int cats, int cheeses,
           double gr[graph_size][4]) {
  /*
         This function computes and returns the heuristic cost for location x,y.
         As discussed in lecture, this means estimating the cost of getting from
     x,y to the goal. The goal is cheese. Which cheese is up to you. Whatever
     you code here, your heuristic must be admissible.

         Input arguments:

                 x,y - Location for which this function will compute a heuristic
     search cost cat_loc - Cat locations cheese_loc - Cheese locations mouse_loc
     - Mouse location cats - # of cats cheeses - # of cheeses gr - The graph's
     adjacency list for the maze

                 These arguments are as described in the search() function above
  */

  return (1);  // <-- Evidently you will need to update this.
}

int H_cost_nokitty(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. 

	However - this time you want your heuristic function to help the mouse avoid being eaten.
	Therefore: You have to somehow incorporate knowledge of the cats' locations into your
	heuristic cost estimate. How well you do this will determine how well your mouse behaves
	and how good it is at escaping kitties.

	This heuristic *does not have to* be admissible.

	Input arguments have the same meaning as in the H_cost() function above.
 */

 return(1);		// <-- Evidently you will need to update this.
}

