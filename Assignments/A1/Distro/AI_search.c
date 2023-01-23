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
% 1) Student Name: Richard (Rong Feng) Ye
% 2) Student Name: Samuel Steven Prokopchuk
%
% 1) Student number: 1005035230
% 2) Student number: 
% 
% 1) yerong1
% 2) UtorID
% 
% We hereby certify that the work contained here is our own
%
% Richard Rong Feng Ye             _____________________
% (sign with your name)            (sign with your name)
***********************************************************************/

#include "AI_search.h"

/***********************************************************************
 * Queue Data Structure and operations for BFS
***********************************************************************/
// Data structure to store a queue
typedef struct {
	int data[graph_size];
	int front, rear;
} Queue;

// Function to create an empty queue
Queue* createQueue()
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	queue->front = queue->rear = -1;

	return queue;
}

// Function to check if the queue is full
bool isFull(Queue* queue)
{
	return queue->rear == graph_size - 1;
}

// Function to check if the queue is empty
bool isEmpty(Queue* queue)
{
	return queue->front == -1;
}

// Function to add an element to the queue
void insert(Queue* queue, int element)
{
	if (isFull(queue))
	{
		printf("Queue overflow\n");
		return;
	}

	queue->rear++;
	queue->data[queue->rear] = element;

	// If this is the first element added to the queue
	if (queue->front == -1)
		queue->front = queue->rear;
}

// Function to remove an element from the queue
int extract(Queue* queue)
{
	if (isEmpty(queue))
	{
		printf("Queue underflow\n");
		return -1;
	}

	int element = queue->data[queue->front];
	queue->front++;

	// If this is the last element in the queue
	if (queue->front > queue->rear)
		queue->front = queue->rear = -1;

	return element;
}

/***********************************************************************
 * Stack Data Structure for DFS
***********************************************************************/
// Data structure to store a stack
typedef struct {
    int data[graph_size];
    int top;
} Stack;

// Function to create an empty stack
Stack* createStack()
{
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->top = -1;

    return stack;
}

// Function to check if the stack is full
bool isFull(Stack* stack)
{
    return stack->top == MAX_STACK_SIZE - 1;
}

// Function to check if the stack is empty
bool isEmpty(Stack* stack)
{
    return stack->top == -1;
}

// Function to insert an element to the stack
void insert(Stack* stack, int element)
{
    if (isFull(stack))
    {
        printf("Stack overflow\n");
        return;
    }

    stack->top++;
    stack->data[stack->top] = element;
}

// Function to extract an element from the stack
int extract(Stack* stack)
{
    if (isEmpty(stack))
    {
        printf("Stack underflow\n");
        return -1;
    }

    int element = stack->data[stack->top];
    stack->top--;

    return element;
}

/***********************************************************************
 * Priority Queue Data Structure for A*
***********************************************************************/
typedef struct {
    int data[graph_size];
    int cost[graph_size];
    int front, rear;
} PriorityQueue;

// Function to create an empty priority queue
PriorityQueue* createPriorityQueue()
{
    PriorityQueue* queue = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    queue->front = queue->rear = -1;

    return queue;
}

// Function to check if the priority queue is full
bool isFull(PriorityQueue* queue)
{
    return queue->rear == MAX_QUEUE_SIZE - 1;
}

// Function to check if the priority queue is empty
bool isEmpty(PriorityQueue* queue)
{
    return queue->front == -1;
}

// Function to swap two elements in the priority queue
void swap(PriorityQueue* queue, int i, int j)
{
    int temp = queue->data[i];
    queue->data[i] = queue->data[j];
    queue->data[j] = temp;

    int tempCost = queue->cost[i];
    queue->cost[i] = queue->cost[j];
    queue->cost[j] = tempCost;
}

// Function to insert an element into the priority queue
void insert(PriorityQueue* queue, int element, int cost)
{
	// We've already checked if there is a wall before calling insert
	// We know the cost to get to any node is g(n) = 1
	// int cost = 1;
    if (isFull(queue))
    {
        printf("Priority queue overflow\n");
        return;
    }

    if (isEmpty(queue))
    {
        queue->front = queue->rear = 0;
        queue->data[queue->rear] = element;
		
		// Cost to get to first node is 0
        queue->cost[queue->rear] = 0;
        return;
    }

    queue->rear++;
    queue->data[queue->rear] = element;
    queue->cost[queue->rear] = cost;

    // Move the element to its correct position
    int i = queue->rear;
    while (i > 0 && queue->cost[i] < queue->cost[i - 1])
    {
        swap(queue, i, i - 1);
        i--;
    }
}

// Function to remove the element with the highest priority
int extract(PriorityQueue* queue)
{
    if (isEmpty(queue))
    {
        printf("Priority queue underflow\n");
        return -1;
    }

    int element = queue->data[queue->front];
    queue->front++;

    return element;
}


void search(double gr[graph_size][4], int path[graph_size][2], int visit_order[size_X][size_Y], int cat_loc[10][2],
 int cats, int cheese_loc[10][2], int cheeses, int mouse_loc[1][2], int mode,
  int (*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
  )
{
 /*
   This function is the interface between your solution for the assignment and the driver code. The driver code
   in AI_search_core_GL will call this function once per frame, and provide the following data
   
   Board and game layout:
	
	The game takes place on a grid of size 32x32, the file board_layout.h specifies the size and defines two
	constants 'size_X' and 'size_Y' for the horizontal and vertical size of the board, respectively. For our
	purposes, the grid of locations is represented by a graph with one node per grid location, so given
	the 32x32 cells, the graph has 1024 nodes. 

	To create a maze, we connect cell locations in the grid in such a way that a) there is a path from any
	grid location to any other grid location (i.e. there are no disconnected subsets of nodes in the graph),
	and b) there are loops.

	Since each node represents a grid location, each node can be connected to up to 4 neighbours in the
	top, right, bottom, and left directions respectively:

			node at (i,j-1)
				^
				|
	(node at i-1, j) <- node at (i,j) -> node at (i+1, j)
				|
				v
			node at (i,j+1)

	The graph is therefore stored as an adjacency list with size 1024 x 4, with one row per node in the
	graph, and 4 columns corresponding to the weight of an edge linking the node with each of its 4
	possible neighbours in the order top, right, bottom, left (clockwise from top).

	Since all we care is whether nodes are connected. Weights will be either 0 or 1, if the weight is
	1, then the neighbouring nodes are connected, if the weight is 0, they are not. For example, if

	graph[i][0] = 0 
	graph[i][1] = 1
	graph[i][2] = 0
	graph[i][3] = 1

	then node i is connected to the right and left neighbours, but not to top or bottom.

	The index in the graph for the node corresponding to grid location (x,y) is

	index = x + (y*size_X) 		or in this case		index = x + (y*32)

	Conversely, if you have the index and want to figure out the grid location,

	x = index % size_X		or in this case		x = index % 32
	y = index / size_Y		or in this case		y = index / 32

	(all of the above are *integer* operations!)

	A path is a sequence of (x,y) grid locations. We store it using an array of size	
	1024 x 2 (since there are 1024 locations, this is the maximum length of any
	path that visits locations only once).

	Agent locations are coordinate pairs (x,y)	

   Arguments:
		gr[graph_size][4]   - This is an adjacency list for the maze
		path[graph_size][2] - An initially empty path for your code to fill.
				      In this case, empty means all entries are initially -1
		visit_order[size_X][size_Y]	- An array in which your code will store the
						  *order* in which grid locations were 
						  visited during search. For example, while
						  doing BFS, the initial location is the
						  start location, it's visit order is 1.
						  Then the search would expand the immediate
						  neighbours of the initial node in some order,
						  these would get a visit order of 2, 3, 4, and
						  5 respectively, and so on.
					
						  This array will be used to display the search
						  pattern generated by each search method.

		cat_loc[10][2], cats   - Location of cats and number of cats (we can have at most 10,
					 but there can be fewer). Only valid cat locations are 0 to (cats-1)
		cheese_loc[10][2], cheeses - Location and number of cheese chunks (again at most 10,
					     but possibly fewer). Valid locations are 0 to (cheeses-1)
		mouse_loc[1][2] - Mouse location - there can be only one!
		mode - Search mode selection:
					mode = 0 	- BFS
					mode = 1	- DFS
					mode = 2	- A*

		(*heuristic)(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[10][2], int cats, int cheeses)
				- This is a pointer to one of the heuristic functions you will implement, either H_cost()
				  or H_cost_nokitty(). The driver in AI_search_core_GL will pass the appropriate pointer
				  depending on what search the user wants to run.
			
				  If the mode is 0 or 1, this pointer is NULL

				  * How to call the heuristic function from within this function : *
					- Like any other function:
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);

   Return values:
		Your search code will directly update data passed-in as arguments:

		- path[graph_size][2]	: Your search code will update this array to contain the path from
					  the mouse to one of the cheese chunks. The order matters, so
					  path[0][:] must be the mouse's current location, path[1][:]
					  is the next move for the mouse. Each successive row will contain
					  the next move toward the cheese, and the path ends at a location
					  whose coordinates correspond to one of the cheese chunks.
					  Any entries beyond that must remain set to -1
		- visit_order[size_X][size_Y] 	:  Your search code will update this array to contain the
						   order in which each location in the grid was expanded
						   during search. This means, when that particular location
						   was checked for being a goal, and if not a goal, had its
						   neighbours added as candidates for future expansion (in
						   whatever order is dictated by the search mode).

						   Note that since there are 1024 locations, the values in 
						   this array must always be in [0, 1023]. The driver code
						   will then display search order as a yellow-colored
						   brightness map where nodes expanded earlier will look
						   brighter.

		* Your code MUST NOT modify the locations or numbers of cats and/or cheeses, the graph,
	     	  or the location of the mouse - if you try, the driver code will know it *
			
		That's that, now, implement your solution!
 */

 /********************************************************************************************************
 * 
 * TO DO:	Implement code to carry out the different types of search depending on the
 *		mode.
 *
 *		You can do this by writing code within this single function (and being clever about it!)
 *		Or, you can use this function as a wrapper that calls a different search function
 *		 (BFS, DFS, A*) depending on the mode. Note that in the latter case, you will have
 *	    	  to inform your A* function somehow of what heuristic it's supposed to use.
 *
 *		Visiting Order: When adding the neighbours of a node to your list of candidates for
 *				expansion, do so in the order top, right, bottom, left.
 *
 *		NOTE: Your search functions should be smart enough to not choose a path that goes
 *		      through a cat! this is easily done without any heuristics.
 * 
 *		How you design your solution is up to you. But:
 *
 *		- Document your implementation by adding concise and clear comments in this file
 *		- Document your design (how you implemented the solution, and why) in the report
 *
 ********************************************************************************************************/

int start = mouse_loc[0][0] + (mouse_loc[0][1]*size_X);
switch (mode)
​{
    case 0: //BFS
		// Create a queue to store the nodes to be queued
		Queue* data_structure = createQueue();
		// insert the start node
		insert(data_structure, start);
      break;

    case 1: //DFS
		/*
		this is run every few time steps because if you don't 
		dfs will just go back and forth and not go anywhere
		*/
		

		// Create a stack to store the nodes to be queued
		Stack* data_structure = createStack();
		// insert the start node
		insert(data_structure, start);
      break;
    case 2: //A*
		// pick path based on cost + heuristic, store path cost as just path cost?

		// Create a stack to store the nodes to be queued
		PriorityQueue* data_structure = createPriorityQueue();	
		// insert the start node
		insert(data_structure, start, 0);
      break;
	
	/*
	Since the traversal and checking of cheese/cats/walls are all the same
	between each of the search methods, we can reuse that code for all three.
	The only thing that differs is the data structure used to store the data, 
	which is initialized in the above switch case block. This reduces the lines
	of code written and is easy to read and debug.
	*/

	// array to store where nodes came from, to create a path
	int cameFrom[graph_size];

	// Create an array to keep track of queued nodes
	bool visited[n];
	memset(visited, false, sizeof(visited));
	visited[start] = true;
	
	// track the order in which nodes were visited
	visit_order[mouse_loc[0][0]][mouse_loc[0][1]] = 1;
	// order for subsequent nodes
	int order = 2;

	// used to terminate the search when a piece of cheese is found
	bool found == false;
	// Loop until the data_structure is empty
	while (!isEmpty(data_structure) && !found)
	{
		// keep track of the current location
		int current = extract(data_structure);
		int cur_x = current % size_X;
		int cur_y = current / size_Y;

		// need to check if pixel is one of cheese_loc[10][2]
		for(int i = 0; i < cheeses; i++){
			int cheese_x = cheese_loc[i][0];
			int cheese_y = cheese_loc[i][1];
			if (current == (cheese_x + (cheese_y*size_X)) )
			{
				path = constructPath(cameFrom, cheese_x + (cheese_y*size_X), start);
				printf("Reached a cheese piece at %d, %d\n", cheese_x, cheese_y);
				found = true;
				break;
			}
		}

		// insert all unvisited neighbors of the current node
		for (int i = 0; i < 4; i++)
		{	
			int x,y;
			switch (i){
				case 0:
					x = cur_x;
					y = cur_y - 1;
					break;
				case 1:
					x = cur_x + 1;
					y = cur_y;
					break;
				case 2:
					x = cur_x;
					y = cur_y + 1;
					break;
				case 3:
					x = cur_x - 1;
					y = cur_y;
					break;
			}
			// check dimensions are in bounds
			if(x < 32 && x >= 0 && y >= 0 && y < 32)
			{
				int idx = x + (y*size_X);

				// check for cats
				bool is_cat = false;
				for(int i = 0; i < cats; i ++)
				{
					if (idx == (cat_loc[i][0] + (cat_loc[i][1]*size_X) ) )
					{
						is_cat = true;
						break;
					}
						
				}
				// queue if neighbour is not visited, no wall (weight != 0), no cat
				if (!visited[idx] && gr[current][i] && !is_cat) 
				{
					if(mode == 2){
						h = heuristic( x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);
						insert(data_structure, idx, 1 + h);
					}
					insert(data_structure, idx);
					// insert value into cameFrom array
					cameFrom[idx] = current;
					visited[idx] = true;
				}
			}
		}
		visit_order[x][y] = order;
		order++;
	}
}

 // Stub so that the code compiles/runs - The code below will be removed and replaced by your code!
//  path[0][0]=mouse_loc[0][0];
//  path[0][1]=mouse_loc[0][1];
//  path[1][0]=mouse_loc[0][0];
//  path[1][1]=mouse_loc[0][1];

 return;
}

int H_cost(int x, int y, int cat_loc[10][2], int cheese_loc[10][2], int mouse_loc[1][2], int cats, int cheeses, double gr[graph_size][4])
{
 /*
	This function computes and returns the heuristic cost for location x,y.
	As discussed in lecture, this means estimating the cost of getting from x,y to the goal. 
	The goal is cheese. Which cheese is up to you.
	Whatever you code here, your heuristic must be admissible.

	Input arguments:

		x,y - Location for which this function will compute a heuristic search cost
		cat_loc - Cat locations
		cheese_loc - Cheese locations
		mouse_loc - Mouse location
		cats - # of cats
		cheeses - # of cheeses
		gr - The graph's adjacency list for the maze

		These arguments are as described in the search() function above
 */

	// cost should be higher if closer to a cat
	// cost should be lower if close to cheese
	// cost should be lower for 

 return(1);		// <-- Evidently you will need to update this.
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

int [][] constructPath(int* cameFrom, int goal, int start)
{
    int current = goal;
    int reverse_path[graph_size];
	int path[graph_size][2];
    int pathIndex = 0;

    while (current != start)
    {
        reverse_path[pathIndex] = current;
        current = cameFrom[current];
		pathIndex++;
    }
    reverse_path[pathIndex] = start;

	for(i = pathIndex; i > -1; i--){
		path[i][0] = reverse_path[i] % size_X;
		path[i][1] = reverse_path[i] / size_Y;
	}
	return path;
    // printf("The path from start to goal is: ");
    // for (int i = pathIndex - 1; i >= 0; i--)
    //     printf("%d ", path[i]);
    // printf("\n");
}