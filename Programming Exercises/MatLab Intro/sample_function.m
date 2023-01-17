%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%  CSC D84 - Artificial Intelligence - UTSC
%%%
%%%  Introduction to Matlab - sample function.
%%%
%%%  You should have started with 'intro_to_matlab.m'. If you haven't gone through that
%%%  script, please do so now! this is the last part of the intro.
%%%
%%%  This is a simple function to show you how they work. There will be nothing surprising
%%%  to you in here - it's just a little bit of syntax specific to Matlab, but it works
%%%  just the same as in any other language you've used including Python and C.
%%%
%%%  This script: F. Estrada, Jul 2020
%%%
%%%  For a regular function, you'd have documentation explaining what it does, it looks
%%%  like this:
%%%
%%%  function [x, x2, x3]=sample_function(vec1, vec2)
%%%
%%%     'vec1' and 'vec2' can be scalars, vectors, or matrixes but MUST HAVE the same size
%%%
%%%     The function then computes and returns
%%%
%%%     x    % Sum of vec1 and vec2
%%%     x2   % Sum of the inputs, squared
%%%     x3   % Sum of the inputs, cubed
%%%
%%%   Example call:
%%%
%%%   vec1=[1:10];
%%%   vec2=[.1:.1:1];
%%%   [x,x2,x3]=sample_function(vec1,vec2);
%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function [x,x2,x3]=sample_function(vec1,vec2)

% The function declaration above works as follows:
% First we have the keyword 'function' this has to be the first thing in the script
% the rest looks like a regular function call. First you have the set of return values
% (in this case 'x', 'x2', and 'x3'. Matlab can obviously return more than one thing
% at a time. 
%
% Then you have the function's name which MUST MATCH the name of the .m file it's stored
% in. 
%
% And finally you have the input arguments separated by commas. Note there are no types
% specified in any of these things. So it is generally a good idea to let the user know
% what to expect. I have done that at the top, as you already found out!

% ALWAYS: Initialize the return values - that way you can't forget any of them. Usually it's
% enough to initialize them to empty vectors, but if you already know the size of the
% output, you should go ahead and use 'zeros' or 'ones' or any other function to
% pre-allocate and initialize these return values.
x=[];
x2=[];
x3=[];

% Let's do this.
%
% ALWAYS: check your inputs make sense!
%  in this case, we allow scalars, vectors, and matrices. But we have to check they're
%  the same size.

s1=size(vec1);      % Dimensions of s1
s2=size(vec2);      % Dimensions of s2

if (length(s1)~=length(s2))     % Different number of dimensions!
    printf('sample_function(): vec1 and vec2 MUST have the same size\n');
    return;             
end;

if (sum(s1-s2)~=0)              % Same number of dimensions but different sizes!
    printf('sample_function(): vec1 and vec2 MUST have the same size\n');
    return;             
end;

% Input looks reasonable
x=vec1+vec2;
x2=x.^2;
x3=x.*x.*x;

return;

% If you omit the 'return' at the end of the script it's ok - the function returns anyway.
