%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% CSC D84 - Artificial Intelligence - Winter 2020
%
% This function will perform a graph search (UCS) on the
% given input graph.
%
% The graph is passed in as an adjacency list as defined in
% MARS_pathfinding.m (see that script for a description)
%
% You have to implement the search process. You're free to
% do this however you like (i.e. use recursion, or not,
% it's up to you). 
%
% Remember:
% For UCS you need to keep track of the current lowest cost
%  to nodes in the graph (initially this is unknown for all
%  nodes except for s, with cost zero)
% UCS expands next the UNEXPANDED node with lowest cost
%  NO LOOPS ARE ALLOWED - you should NEVER expand a node twice
% You need to keep track of the predecessor for each node
%  so you can build the path at the end.
%
% You can use arrays for the costs and predecessors, or you can use
% whatever other data structure you find helps you do your work.
%
% THE SEARCH FUNCTION MUST RETURN A PATH AS A SEQUENCE OF NODES 
% TO VISIT - the first entry in the path is the start node's index
% and the last entry is the goal node's index.
%
% You can prepend entries to the Path variable below like so:
% Path=[new_index Path];    <- To append, change the order!
%
% Input parameters:
%
% - A graph 'G' of size Nx4 where N is the number of nodes in the graph
% and edges are real valued (the function should not assume the 
% edges are positive, the absence of an edge between two nodes is
% indicated by the value 1e9).
%
% - An array Gid that gives the indexes of the neighbours for each
% node in the graph, as explained in MARS_pathfinding.m
%
% - The index of the start node 's', and the goal node 'g' (we are
% using this function to find a path between two nodes). UCS will
% return the lowest-cost path between 's' and 'g' if edge weights 
% are positive. If there are negative edges, the caveats we
% have discussed apply.
%
% Return values:
%
% The PATH that the function has found, it can be empty if there is no
% path between the nodes.
%
% Starter code: F. Estrada, Jan 2020.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [Path]=graph_search(G,Gid,s,g)

Path=[];

%% TO DO:
% Implement Uniform Cost Search here

return;
