// insert the start node
insert(data_structure, start);
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
bool visited[graph_size];
memset(visited, false, sizeof(visited));
visited[start] = true;

// track the order in which nodes were visited
visit_order[mouse_loc[0][0]][mouse_loc[0][1]] = 1;
// order for subsequent nodes
int order = 2;
// used to terminate the search when a piece of cheese is found
bool found == false;
// Loop until the data_structure is empty
while (!isEmpty(data_structure) && !found) {
  // keep track of the current location
  int current = extract(data_structure);
  int cur_x = current % size_X;
  int cur_y = current / size_Y;
  // need to check if pixel is one of cheese_loc[10][2]
  for (int i = 0; i < cheeses; i++) {
    int cheese_x = cheese_loc[i][0];
    int cheese_y = cheese_loc[i][1];
    if (current == (cheese_x + (cheese_y * size_X))) {
      path = constructPath(cameFrom, cheese_x + (cheese_y * size_X), start);
      printf("Reached a cheese piece at %d, %d\n", cheese_x, cheese_y);
      found = true;
      break;
    }
  }
  
  // insert all unvisited neighbors of the current node
  for (int i = 0; i < 4; i++) {
    int x, y;
    switch (i) {
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
    if (x < 32 && x >= 0 && y >= 0 && y < 32) {
      int idx = x + (y * size_X);
      // check for cats
      bool is_cat = false;
      for (int i = 0; i < cats; i++) {
        if (idx == (cat_loc[i][0] + (cat_loc[i][1] * size_X))) {
          is_cat = true;
          break;
        }
      }
      // queue if neighbour is not visited, no wall (weight != 0), no cat
      if (!visited[idx] && gr[current][i] && !is_cat) {
        if (mode == 2) {
          h = heuristic(x, y, cat_loc, cheese_loc, mouse_loc, cats, cheeses);
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

/*
THe function to construct a path
*/

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
