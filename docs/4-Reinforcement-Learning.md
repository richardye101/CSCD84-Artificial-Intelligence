We want to get around having a complicated utility function. To do this, we can _train_ our agent to learn a utility function. Afterward, we can then get the agent to _play_ the game/task they were trained to do.

## The setup

A problem characterized by a set of state variables, which constitute configurations $s$ contained in the entire state space $S$. *This state space contains end states, where the agent has solved the problem and states where the agent failed.*

We also need a set of all actions $A$ that agents can take.

Finally, we need a reward function/reinforcement signal that maps states $s$ to a reward/punishment. (The goal is to no have to engineer this reward function)

This assumes discrete time steps in this problem.

# Markov Decision Process (MDP)

We care about the immediate past and only that. We don't care about the longterm past.

**Setup:** The agent will learn a policy $\Pi$ that maps states $s$ to an optimal action $a$. We also assume the environment is stationary. 

**Why do we need to assume the environment is stationary?**
That is, the environment does not change. We need this because otherwise there will be a very very large state space that the agent will have to learn over, becoming infeasible.

>Thats not to say that we cannot have an environment that is non-stationary, it just means that it will be more difficult for the agent to learn.

## The Learning Policy

Goal: Maximize the expected sum
$$
\mathbb{E}\left[ \sum^{\infty}_{t=0}\gamma^{t}r_{t} \right] \quad \gamma \in [0,1)
$$

Where $r_{t}$ is the reward at a given time step, and $\gamma_{t}$ is a discount factor.

We don't like that infinite sum, so we can rewrite it.

Define a quantity $V^{*}(s) =$ value of state s $= \max_{\Pi}\mathbb{E}\left[ \sum^{\infty}_{t=0}\gamma^{t}r_{t} \right]$

>This is the max expected value of a given state $s$, indicating whether the state is good or not informing the quality.

It can be computed under the process:
- Given a state $s$ the agent will take an action $a$, receive a reward $r$, and end up at some state $s'$ . This can be used to compute $V^{*}(s)$
	- $<s,a,r,s'>$

Rewrite 
$$
\begin{aligned}
V^{*}(s) &= max_{a}\left( R(s,a) +\\
\gamma \sum_{s'\in S \text{ reachable from }s}T(s,a,s')V^{*}(s)\right)\\
\text{Where }& R(s,a) \text{ is the Reward from taking action a from state s}\\
&T(s,a,s') \text{is the Transition fnc: prob of ending up at s' from s under action }a\\
&V^{*}(s) \text{ is the value at the neighbour of } s
\end{aligned}
$$

Which gets rid of the infinite sum.

### Value Iteration
Initialize $V^{*}(s) = 0$, and a table $Q(s,a)=\emptyset$ 

Repeat until $\Pi$ is *good enough*.

```
for all s in S
	for all a in A
		Q(s,a) = R(s,a) + gamma \sum_s' T(s,a,s')V*(s')
		
	update V*(s) = max_a Q(s,a)
	update Pi(s) = argmax_a Q(s,a)
```

At 1st iteration, $V(s) \ne \emptyset$ for configs where we get some immediate $R(s,a)$.

How to stop:
We need to check for convergence of $V^{*}(s)$, but it could still have very small changes at every iteration. Instead, we can check for when the *policy* $\Pi$ has stopped changing.

This training can run offline (not during game time) so there are no time constraints.

After training, we get a look up table for actions to take, so given a state $s$ we select a few $a$ from $\Pi(s)$.

