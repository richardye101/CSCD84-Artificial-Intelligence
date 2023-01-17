%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%5
%
%  CSC D84 - Artificial Intelligence - UTSC
%
%  This script is a gentle intro to Matlab/Octave, it's
%  here to help you quickly get up to speed in running,
%  understanding, and implementing simple scripts in
%  this language - Matlab is an incredibly powerful tool
%  for any tasks that require numerical computation,
%  Graphics is one such area, so take your time to
%  go through the examples here and to tweak them
%  and change them to ensure you understand how to
%  handle the fundamental components of Matlab.
%
%  How to use this script:
%
%  - Open it in your text editor (it should have
%    Matlab syntax highlighting built-in, for Linux,
%    use Kate)
%  - Read through the script, paying attention to what
%    each section is doing, and copy/paste the relevant
%    code onto a Matlab/Octave interactive terminal
%  - Make sure you understand what happened
%  - Tweak and change the commands for that section 
%    and make sure you understand how they work.
%
%    * DO NOT simply run the whole script *
%
%  Sections are labeled with a number and parenthesis
%
%  e.g. 
%
%  1) 
%
%  You can look for specific sections like that if you
%  want to tackle this script in different sessions.
%
%  This script: F. Estrada, Jun 2020
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%% Quick facts about Matlab/Ooctave
%
% - It's an interpreted language, like Python
% - You can do a lot of work from an interactive terminal,
%   we will do a lot of that this term.
% - You can write and run scripts just like you would in
%   Python. Your scripts can call as many functions in
%   other scripts as you like.
% - There are very complete, powerful libraries already
%   written and available for you to use. Including
%   all kinds of image processing, linear algebra, and
%   geometry.
% - It is designed and built for Math. That means that
%   implementing any math operations is simple and
%   looks pretty much as you would expect.
% - Octave is a free/open source implementation of the
%   Matlab language, and all the code we will use/write
%   in D84 works on Octave.
% - You can interface Matlab/Octave with C code! so 
%   for functions that require the highest speed, you
%   can write a C/C++ program that interfaces with your
%   matlab script. We won't do that in this course
%   though.
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% 1) - Starting up Matlab/Octave
%
% Firing up Octave/Matlab brings up a GUI that has several
% parts, including a directory view, command history, a
% menu, and the relevant part for us which is the command
% terminal - this looks similar to Wing IDE or any other
% IDE you have used before for Python.
%
% Feel free to use the GUI version. Personally, I like to
% just have the command terminal. You can get one by
% launching:
%
% >octave-cli
%
% or
%
% >matlab -nodesktop
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% 2) - Declaring variables in an interactive terminal
%
% Once you have Matlab/Octave running, try this:
% (copy/paste each command, one by one, into your terminal)

x=10;           % This is a scalar variable x
v=[1 2 3];      % This is a 3-element row vector whose entries are 1,2, and 3
w=[4            % This is a 3-element column vector whose entries are 4,5, and 6
   5
   6];
A=[1 2 3        % A 3x3 matrix
   4 5 6
   7 8 9];
   
    % Matrices can have any size and number of dimensions! %
   
% Notice the ';' at the end of each command? if's optional. If you add it,
% the command is executed without any output to the terminal, if you skip it,
% the result of the command is output to the terminal.
%
% Try typing
% >A
%
% You will get a printout of the values inside matrix A.
% 
% Nothing surprising up to this point!
%
% IMPORTANT THINGS TO NOTICE: Vectors can be row vectors or columm vectors
%   and the difference matters. All the rules of Linear Algebra operations
%   are respected, so you can expect 
%
%   A*w         % Works, A is a 3x3 matrix, w is a 3x1 column vector
%   A*v         % Gives you an error since v is a 1x3 row vector, can't do that!
%
% See for yourself:

A*w
A*v

% If you have a row vector, and need a column vector, or vice-versa, use the
% *transpose* operator  '  (a single apostrophe)

v
v'

% It works on matrices tool

A
A'

% At any point, you can check which variables are in your workspace
who

% 'ans' is a special variable that hold the result of the last interactive command.

% You can get more details:
whos

% Did you notice the data type?
%
% By default, all Matlab variables are *double precision floating point*
% By default, all Matlab variables are matrices! (a scalar is a 1x1 matrix,
%             vectors are either 1xn, or nx1 matrices)
% Other data types can be used, but you have to use type casting carefully

% 3) - Basic Matlab operations:
%
% All the basic linear algebra operations are built in, of course.
%
% scalar * vector
% scalar * matrix
% matrix * vector
% matrix * matrix

x*v
x*w
A*w
A*A

% Dot product
%
% Suppose I have two column vectors a, and b, and I want their dot
% product. 
%
% Because of linear algebra rules, we can do this dot product as
% a' * b

a=[1
   2
   3];
b=[4
   5
   6];
   
dot_prod=a' * b

% Cross product - built in as a functions

cross(a,b)

% That's (a x b), and it requires a and b to be 3-element vectors

% NOTE: Dimensions matter! you have to make sure the dimensions of the
%  matrices and vectors you're working with match as per the rules of
%  linear algebra operations. If they don't you'll get an error

c=[1
   2];
   
A*c

% Built in vector/matrix functions
%
% Try these:
%   min() - Gives you the minimum entry in a vector, for matrices, it gives the min over a specified dimension
%   max() - Gives the max entry in a vector, of the max over a specified dimension for matrices
%   norm() - Gives you the magnitude or norm of a vector, or the matrix norm
%   sum() - Gives the sum of elements in a vector, or the sum of entries along a specified dimension of a matrix
%
% There is a large number of built-in functions, you can find help for any of these by typing
%
% >help sum
%
% We will cover more functions as we need them, for now, play a bit with the above ones which are 
% incredibly common.

v
min(v)
max(v)
sum(v)

A
min(A,[],1)       % Take the min along each column (dimension index 1 is columns)
min(A,[],2)       % Take the min along each row (dimension index 2 is rows)
                  % the [] is there because min(A,2) would give me the min of entries
                  % in A and the scalar value 2 (try it out!).

max(A,[],1)
max(A,[],2)

sum(A,1)          % Sum along columns
sum(A,2)          % Sum along rows 

% Element-wise operations
%
% Sometimes, we want to apply specific operations to each entry in a vector or matrix, to do this
% we use the '.' (dot) operator. 

v.^2        % Apply the squaring operation (^2) to each element in v
v.*5        % Multiply each entry in v by 5, same as v*5 because of linear algebra rules

A^3         % The result of A cubed
A.^3        % The result of taking the cube of each entry in A

v.*v        % Multiplying corresponding elements in v (in effect, squares each entry)

% Exercise:  
%
% We were given two vectors a, and b. We took the dot product of a and b as
%
% dot_prod= a' * b
%
% Can you find a way to do this using element-wise multiplication and the sum() function?

% 4) - Indexing
%
% VERY IMPORTANT: Matlab was designed long, long ago for doing Math. Mathematicians start all
%  their indexes at 1 (how many times have you seen a sum for i=1 to N?) so Matlab indexing
%  starts at 1 - This makes for a bit of a mental problem since we're used to indexing from
%  0 for Python, C, and any other common languages. 
%
%  So, don't forget! indexing in matlab is from 1 to N! 

A
A(1,2)                  % First index is the ROW, second index is the COLUMN
A(2,1)                  % Second row, first column

A(1:2,1:2)              % A slice with entries at indexes (1,2) for row, and (1,2) for column
A(2:3,:)                % A slice with entries at indexes(2,3) for row, and all columns
A(2:end,:)              % Sometimes we just want a slice from a given index to the end of the
                        % vector or matrix, the 'end' keyword is a convenient place holder so
                        % we don't have to remember what the size was! This command is the same
                        % as A(2:3,:)
                        
% Of course, you can always find out the size:
size(A)

A(:)                    % All the entries in A as a long column vector, look at how the elements
                        % are ordered: First column entries, then second column, then third, and
                        % so on.
                        

%%% 5) - Basic programming constructs in Matlab: Loops and Conditionals
%%%
%%% Matlab is very similar to C, the syntaz is a bit different but if you know how to write
%%% a C program to do what you want, you'll have no trouble writing a matlab probram that
%%% does what you want.
%%%
%%% DELIMITERS:  Matlab uses an explicit 'end' to indicate the end of a semantic block.
%%%              That means ALL loops, and ALL conditionals must have a matching 'end'
%%%              often you will get an error because Matlab went all the way to the end
%%%              of a file without finding the 'end' for some unit of code, so you should
%%%              always check this when your code is not running.

%%% FOR LOOPS:
%%%
%%% A basic for loop looks like this:
%%%
%%% for i=1:10
%%%     % Here some matlab code that's part of the loop
%%% end;
%%%
%%%   The loop above uses a counter named 'i' which will take values from 1 to 10
%%%   notice the syntax for the range of the loop is the same as that used for slicing
%%%
%%%   You can, of course, specify the increment, and that increment can be any real value
%%%   that you like, for example:
%%%
%%% for j=-5:.05:5
%%%    % Do something with j
%%% end;
%%%
%%%    The loop above uses a counter 'j' with values that go from -5 to 5 in increments
%%%    of .05
%%%
%%% Let's run a simple loop:

% Let's fill up a vector with odd numbers between 1 and 25
v=[];   % Initially empty vector
for i=1:2:25
 v(end+1)=i;    % Add the value of i to the vector after the currently last element of v
end;
v

%%% WHILE loops:
%%%
%%% Pretty much what you'd expect:
%%%
%%% while (condition)
%%%    % Some matlab code here
%%% end;
%%%
%%% The 'condition' is any logical statement that you would write in any other language.
%%% but please note:
%%%
%%% In Matlab, a logical AND is a single '&', and a logical OR is a single '|'. 
%%% logical NOT is '~', and logical XOR is 'xor'
%%%
%%% Let's see:

j=1;
w=[];
while (j~=25)       % NOT equal
  w(end+1)=j;
  j=j+2;
end;

%%% Matlab offers a number of useful functions to test conditions that come up frequently 
%%% in your coding. In particular, the 'find' function will come in handy often.
%%%
%%% 'find' takes a vector or matrix, and returns the indexes of elements matching some
%%% desired value.
%%%
%%% For example, we just created an array with odd numbers in 1-25. Let's use find to
%%% figure out where the odd numbers greater than 10 and less than 20 are:

i1=find(w>10);      % Find indexes of entries in w > 10, i1 is a vector of indexes into w
i2=find(w<20);      % Find indexes of entries in w < 20
id=intersect(i1,i2);    % Take the intersection. 'id' will be a vector with the indexes of
                        % the numbers we wanted.
w(id)                   % Let's print them out.

%%% NOTE: If you're looking carefully, you'll notice we're indexing a vector using another
%%%       vector! this is one of the most useful aspects of Matlab. We can in a single line
%%%       do computations or apply operations to arbitrary entries in vectors and matrices,
%%%       all we need is a vector with the indexes of the entries we want, and the rest
%%%       works just as it would with a single index.
%%%       (Of course when more than one vector or matrix is involved, you have to make sure
%%%       the dimensions work out for the operation you want to carry out).
%%%
%%% Here's the reference to matlab's logical operators:
%%%
%%% https://www.mathworks.com/help/matlab/logical-operations.html

%%% CONDITIONALS: 
%%%
%%% If statements are much like you'd expect:
%%%
%%% if (condition)
%%%    % Do something
%%% else
%%%    % Do something else
%%% end;
%%%
%%% Notice the 'if-else' block has its 'end' delimiter as expected. The only tricky bit
%%% is if you have nested if-else blocks - EACH 'if' in the nested structure must come
%%% with a matching 'end'.
%%%

i=rand();   % Get a random number
if (i<.5)
    if (i<.25)
        printf('i is less than .25\n');     % BTW, print statements and text strings use single ' in Matlab
    else
        printf('i is between .25 and .5\n');
    end;        % Matches the 'if (i<.25...
else
    if (i<.75)
        printf('i is between .5 and .75\n');
    else
        printf('i is greater than .75\n');
    end;        % Matches the 'if (i<.75...
end;    % Matches the 'if (i<.5...

%%% That's it for basic Matlab coding - we've only two other things to discuss and then
%%% you're ready for using Matlab for your Graphics course!

%%% 6) Plotting
%%%
%%% One big reason we'll be using Matlab is because plotting things is incredibly easy,
%%% and Matlab can plot make all kinds of interesting plots. If you've used Matplotlib
%%% in Python, you're about to discover how plotting is done *right*. 

%%% Opening a figure:

figure(1);      % Opens figure #1 (you can use any integer indentifier to distinguish different plots)
clf;            % clear the figure (in case something was on it already)
hold on;        % All plotting commands will be kept until cleared (otherwise only the last plotting command
                % will be visible).
title('My first Matlab plot!');

%%% Let's put something here.
x=[0:.1:2*pi];          % See what we did there? this gives us a vector of values in [0,2pi] with increments of .1
y=sin(x);               
z=cos(x);

plot(x,y,'-','color',[0 0 1]);         % Plot pairs of coordinates from x and y, the '-' is a line format specifier,
                                       % you can easily plot dashed, dotted, or continuous lines with markers.
                                       % the 'color' is an RGB triplet with values in [0,1].
plot(x,z,'.-','color',[0 1 0]);        % Look at the help for 'plot' to learn more about line format specifies and
                                       % the many options you have for plotting things.

xlabel('X');
ylabel('Sine and Cosine of X');

%%% Matlab supports 3D plots, surfaces, animation, and all kinds of nice things. We will learn
%%% more about those as we get into the course itself. 

%%% 7) Functions
%%%
%%% Most of the time, we'll work with code that is spread over many modules each of which does a specific thing.
%%% in matlab, this is accomplished by making your scripts into functions, which can be called from other
%%% scripts and functions to get work done.
%%%
%%% You've already used a number of matlab built-in functions in the code above, let's now see how 
%%% we would create a function ourselves.
%%%
%%% Please open the script 'sample_function.m'
%%%
%%% Read through it, then call it below with appropriate arguments to achieve the desired results!

vec1=[1:10];
vec2=[.1:.1:1];
[x,x2,x3]=sample_function(vec1,vec2);
[x' x2' x3']

%%% EXERCISE: Use the sample function to compute the sum of the sine and cosine of v, where v contains
%%% values in [0, 2pi] in increments of .1.
%%% Then plot (on the same graph) the sum, the squared sum, and the cubed sum.

%%% That's it for our intro to Matlab! we have covered all the basics, and we'll learn more about
%%% it as we go along. But if you went through the above, you should be able to follow the code 
%%% we'll be posting, and with a bit of work you'll be able to complete any parts we ask you to
%%% implement yourself! 

%%% You can take a well earned break, and do keep in mind why we're using this language:
%%%
%%% C-like structure and control flow
%%% Incredible flexibility for doing math-intensive code
%%% Your math code looks like math!
%%% Plotting is easy and you can build fairly complex plots in no time at all.
