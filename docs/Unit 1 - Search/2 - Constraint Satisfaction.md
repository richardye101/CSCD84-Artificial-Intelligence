- [[#Scheduling courses example|Scheduling courses example]]
- [[#Backtracking Search|Backtracking Search]]
	- [[#Backtracking Search#Algorithm|Algorithm]]
			- [[#Worst case|Worst case]]
	- [[#Backtracking Search#Improving Backtracking search|Improving Backtracking search]]

- Need to listen to *I can't get no satisfcation*...
Constraint satisfaction deals with scheduling, such as timetable problems, operations, etc.
Represented as a graph, where edges represent contraints. There is a domain for variables as well.

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

### Improving Backtracking search 


