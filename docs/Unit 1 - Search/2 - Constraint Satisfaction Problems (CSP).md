- [[#Defining the problem|Defining the problem]]
- [[#Scheduling courses example|Scheduling courses example]]
- [[#Backtracking Search|Backtracking Search]]
	- [[#Backtracking Search#Algorithm|Algorithm]]
			- [[#Worst case|Worst case]]
	- [[#Backtracking Search#Improving Backtracking search|Improving Backtracking search]]
		- [[#Improving Backtracking search#1. Use Least Remaining Variables (LRV) rule|1. Use Least Remaining Variables (LRV) rule]]
		- [[#Improving Backtracking search#2. Use the largest number of active constraints|2. Use the largest number of active constraints]]
		- [[#Improving Backtracking search#3. Choose the least constraining value for the variable|3. Choose the least constraining value for the variable]]
- [[#Example:|Example:]]
	- [[#Example:#Need to formulate a connected graph|Need to formulate a connected graph]]
- [[#[Einstein's Puzzle](https://web.stanford.edu/~laurik/fsmbook/examples/Einstein%27sPuzzle.html)|[Einstein's Puzzle](https://web.stanford.edu/~laurik/fsmbook/examples/Einstein%27sPuzzle.html)]]


- Need to listen to *I can't get no satisfcation*...
Constraint satisfaction deals with scheduling, such as timetable problems, operations, etc.
Represented as a graph, where edges represent contraints. There is a domain for variables as well.

## Defining the problem
You have variables which could be discrete or categorical (makes up the domain, which should be finite)
Can use linear programming for random variable CSP
Edges represent nodes that have constraints

- Unary: (self edge) is weird?
- Binary: paris of variables
- Can have ternary and more

## Scheduling courses example
Constraints:

- Professor schedules
- Courses on the same level
- Courses which students are likely to take in the same term
- Lecture hall availability

Sudoku is a Constraint Satisfcation problem

## Backtracking Search
A form of DFS 

Will use a country example.
Domain: `{r,g,b,y}`

### Algorithm
- Start with empty assignment of all variables (countries), or initial config
- Choose a random variable (country) and assign a value from the domain that doesn't break constraints
	- Ex. Assign the colour blue to Portugal
- Repeat with different variable
	- Ex. Assign the colour red to Spain
- If a variable is reached where there is no solution, then you must **backtrack**.
	- Go back to the previous variable, and reassign a new value from domain and try again

##### Worst case
Will try every possible combination and it could take very long
Can be $O(d^n)$ time complexity, with $n$ variables, and a domain of size $d$.

### Improving Backtracking search 

#### 1. Use Least Remaining Variables (LRV) rule
Choose the next variable with the fewest possible values left
- This ensures that we fulfill as many variables as possible
	- If we don't, then our tree could be very wide and that would result in a very high number of nodes

#### 2. Use the largest number of active constraints
Only use this rule if **LRV** doesn't reduce the search time, or all the variables have the same number of constraints? (#2 is a tie-breaker rule)

Choose the variable participating in the largest number of active constraints. This reduces the domain for all the nodes connected to the given node.

An active constraint is between variables that have yet to be assigned.

#### 3. Choose the least constraining value for the variable
How to choose a value for the variable we've chosen to process:

- If we have two nodes/variables which both share an edge:
	- S can be assigned ${R,G,B}$ and F can be assigned ${R,G}$
	- We choose $B$  for S because its the value that is the least likely to constrain F's domain.

# Quiz next week up to here

## Example: 

- 5 houses **in a row** are a street
	- In each house lives a person with the following domains:
- Nationalities: ${China, British, Thai, Canadian, Indian}$
- Each person has a different favorite drink: ${Tea, Coffee, Monster, Tequila, Beer}$
- Each person has a different pet: ${Snake, Tarantula, Capybara, Cat, Fish}$
- Each person watches a different show: ${Wednesday, The\ Last\ of\ Us, Blue\ Lock, Jack\ Ryan, Arcane}$
- Each house has a different colour: ${R, G, B, W, Y}$
- The ORDER of the houses matter (they must be in a row): ${1,2,3,4,5}$

Cannot use rule 1, as each user has the same amount of variables to choose from
Cannot use rule 2, as everybody constrains everybody at the start
Cannot use rule 3, as each person has the same domains

### Need to formulate a connected graph
- We can draw a fully connected graph of the 5 houses

## [Einstein's Puzzle](https://web.stanford.edu/~laurik/fsmbook/examples/Einstein%27sPuzzle.html)

Let us assume that there are five houses of different colors next to each other on the same road. In each house lives a man of a different nationality. Every man has his favorite drink, his favorite brand of cigarettes, and keeps pets of a particular kind.

1.  The Englishman lives in the red house.
2.  The Swede keeps dogs.
3.  The Dane drinks tea.
4.  The green house is just to the left of the white one.
5.  The owner of the green house drinks coffee.
6.  The Pall Mall smoker keeps birds.
7.  The owner of the yellow house smokes Dunhills.
8.  The man in the center house drinks milk.
9.  The Norwegian lives in the first house.
10.  The Blend smoker has a neighbor who keeps cats.
11.  The man who smokes Blue Masters drinks bier.
12.  The man who keeps horses lives next to the Dunhill smoker.
13.  The German smokes Prince.
14.  The Norwegian lives next to the blue house.
15.  The Blend smoker has a neighbor who drinks water.

The question to be answered is: **Who keeps fish?**
