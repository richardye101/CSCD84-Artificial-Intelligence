- [[#Review of lecture 1:|Review of lecture 1:]]
	- [[#Review of lecture 1:#UCS (Uniform Cost Search)|UCS (Uniform Cost Search)]]
- [[#Heuristic Search (A* Search)|Heuristic Search (A* Search)]]
	- [[#Heuristic Search (A* Search)#Conditions of Heuristic Search|Conditions of Heuristic Search]]
	- [[#Heuristic Search (A* Search)#Cost of traversal|Cost of traversal]]
		- [[#Cost of traversal#For the route navigation problem|For the route navigation problem]]
- [[#Negative Edge Weights|Negative Edge Weights]]
		- [[#Cost of traversal#Possible solution|Possible solution]]
		- [[#Cost of traversal#Bellman-Ford Algorithm|Bellman-Ford Algorithm]]

## Review of lecture 1:
Discussed states, represented as nodes, which correspond to configurations. The AI can travel between states along weighted edges.
	Right now we just consider the weighted edges and try to minimize the path to the goal state.

Recall BFS and DFS from B63
	Both explore all nodes $\mathcal{O}(N)$ , and all edges $\mathcal{O}(E)$ for a total of $\mathcal{O}(|N|+|E|)$ time

### UCS (Uniform Cost Search)
- Expand in order of increasing cost from the start state (S), using cost function $g(s)$ which is simply the distance from the start node to current node 
- Uses a priority queue
- It's Dijkstra's but stops when we reach a goal state, instead of calculating shortest path to every node in the graph
- All edges > 0. Will work with edge weight of 0, but realistically no action costs 0

- Will be in this weeks assignment

## Heuristic Search (A* Search)
- Heuristic simply means a guess
- Used to reduce the amount of search required to reach goal state

Heuristic Search, also called $A^*$ (A-star) search is a powerful and extensively used search algorithm that makes use of a heuristic function to guess which nodes in the search tree are likely to be closer to a goal state, so those nodes can be expanded first.

Because it is a guess, it can not be used exclusively to guide search expansion order (if you think about it, in order to know exactly how far each node is from the goal, we'd have to have completed the search process on the entire tree!). Instead, $A^*$ search uses the heuristic function in combination with the actual cost of getting to a node from the initial state (this value is updated as nodes are expanded, just like with UCS).
The heuristic cost used by A* search to determine expansion order is:

$$
f(n)=g(n)+h(n)
$$

Where $f(n)$ is the heuristic cost for some node $\mathbf{n}$, and it consists of $\mathbf{g}(\mathbf{n})$ - the actual cost of reaching node $\mathbf{n}$ from the initial node, and $\mathbf{h}(\mathbf{n})$ a **guess of the cost to get to [the/a] goal state from $n$**.
The $A^*$ search algorithm is identical to UCS, but we use the heuristic cost $h(n)$ to order nodes in the priority queue. Otherwise there's no change! So if you have implemented UCS, implementing A* is straightforward, you just need to add a suitable heuristic function.

In the example above, a good guess would be to use the Manhattan distance between a node and the goal. This happens to be a lucky guess for this problem because the Manhattan distance is the actual distance between two nodes - it is very often not the case that we can find such an appropriate guess, so we'll talk about what makes a good guess below. For now, let's see how things change once we introduce the heuristic cost for each node into the search process.


We try to create a heuristic cost that guesses the cost of the path from node N to G (goal state). In the city grid example, it can simply be the Manhattan Distance.

![[Obsidian-Attachments/Pasted image 20230116093020.png]]
This **isn't** trivial because streets may have conditions, such as speed limits, one way streets, etc. 

### Conditions of Heuristic Search
- $h(n)$ is admissible if
	- $h(n) \le h^*(n) \quad \forall n$
		- Where $h^*(n)$ is the true cost from $n$ to goal
	- $h(n) \ne 0 \quad \forall n$ 

- $h^*(n)$ is always admissible

- When constructiong a heuristic, need to prove admissibility and show that it **is not worse** than no heuristic (UCS). UCS guarantees optimality in terms of path, so huerisitc search may not always be optimal but being able to reduce complexity/algo time is important.

### Cost of traversal
$f(n) = g(n) + f(n)$
- $g(n)$ is the weight of the path, but in a navigation problem, the weights may change due to traffic. Therefore, it is very difficult to determine whether given path is optimal.
	- We can only solve based on the current information
	- Ex could update weights based on the past k drivers (Waze will estimate your travel time based on the speed of other drivers, so you may need to speed if you want to make that time)

#### For the route navigation problem
- Can create a heuristic based on:
	- increase if: 
		- euclidean distance is greater
		- there are traffic lights
	- decreasing if: 
		- path has higher speed
	- many other factors to consider for the heuristic

## Negative Edge Weights

Some graphs could have negative edge weights. 
- UCS/A* will no longer guarantee the optimal cost and path
	- But we may not care if we don't have the most optimal path
- Need to ensure there are no loops, or that the user cannot loop

#### Possible solution
Could add a $k$, where $k$ is the most negative edge
	This guarantees all edges are non-negative (or positive?)
	This will however change the optimal solution
We actually need to change the search algorithm

#### Bellman-Ford Algorithm
Start with $V[n] = \infty$ for all nodes $n$ where $V[n]$ is the cost to get to the $n$th node
```
let d(u) be the cost to get to node u from the source/start
let d(v) be the cost to get to node v from the source/start
let c(u,v) be the cost to get from node u to v

for E-1 iterations
	For each edge(u,v) in the graph
		if d(u) + c(u,v) < d(v)
			then d(v) = d(u) + c(u,v)
			
```
The algorithm looks at each edge, determines whether taking that edge to the ending node will make the total distance to that node shorter.
