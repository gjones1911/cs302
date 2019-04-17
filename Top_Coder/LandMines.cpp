/*
// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
We want to send a soldier into a mine field with a directional metal
detector. The field is an n x n grid, and the soldier will move horizontally (east
or west) or
vertically (north or south), never leaving the field.  His metal 
detector can be pointed in any of the cardinal directions (east, north, west,
or south) and will beep if it senses any mine in that direction, no matter
how far away the mine is. Only mines that are exactly in the row or column down
which the sensor is pointed
are sensed.

The soldier can move one square at a time in any of the four directions, 
pointing his sensor in various directions. But he will not move onto a square
if his sensor beeps when pointed in that direction, unless he has previously
visited that square. He will continue this
process, visiting and revisiting squares as needed, until he is sure that
he has visited every square that he can.
 
We want to see how well we will be
able to do using this type of sensor.  Create a class LandMines that 
contains a method numClear that takes the actual layout of mines in a
minefield as input and that returns the number of squares that the
soldier can safely visit.

The layout shows the mine field as a vector <string>, starting with the top row. 
'-' denotes a square with no mine and 'M' denotes a square that contains a mine. 
The soldier will always start in the northwest corner (the leftmost square in 
the top row), which will never contain a mine.



DEFINITION
Class:LandMines
Method:numClear
Parameters:vector <string>
Returns:int
Method signature:int numClear(vector <string> layout)


CONSTRAINTS
-layout contains between 2 and 50 elements inclusive.
-The length of each element of layout equals the number of elements in layout.
-Each character in layout will be a hyphen ('-') or uppercase 'M'.
-The first character in the first element of layout will be a hyphen.


EXAMPLES

0)
{"-M-",
 "---",
 "M--"}

Returns: 1

(IN ALL THE EXAMPLES, '.' DENOTES A SQUARE THAT CAN BE VISITED)

    .M-
    ---
    M--
 
The soldier cannot safely move in either direction from his
initial position.



1)
{"-M-",
 "-M-",
 "--M"}

Returns: 3


     .M-
     .M-
     .-M

The soldier can safely go south from his initial position, but cannot
head east from any of those locations.


2)
 {"--M-",
  "-MM-",
  "----",
  "----"}

Returns: 12


    .-M.
    .MM.
    ....
    ....

The soldier can go south, and then can move east along both of
the bottom rows. From the easternmost location on one of the bottom rows, he
can safely move north. But the second square in the top row can never safely
be visited.


3)
{"-----",
 "--M-M",
 "-----",
 "-M---",
 "---M-"}

Returns: 21


    .....
    ..M.M
    .....
    .M...
    ...M.
  
All of the squares without mines can safely be visited. For example, 
a way to 
visit the second square on the bottom row is as follows: go south 2, 
go east 2. 
From this location it is safe to go south since the
detector indicates no mines in that direction; go south 2. Then go west 1.
 

// END CUT HERE
*/
#line 131 "LandMines.cpp"
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>

using namespace std;

class node
{
	public:
		vector<int> adjl;

		bool visited;
};


class Landmine
{
	public:

		vector<node *> nv;
		int vstd;
		
		void dfs(node * n)
		{
			int i;

			if(n->visited == true) return;

			n->visited = true;

			//now go through its adj

			for(i = 0; i < n.adjl.size(); i++)
			{
				adjl[i] = coordinate;
				vstd++;
				dfs( nv[coordinate] );

			}
		}

};

class LandMines 
{

	public:
		int numClear(vector <string> layout)
		{
			int rows = layout.size();
			int col = layout[0].size();

			int i, j, k, l; 

			Landmine lm;

			lm.vstd = 0;

			node * np;

		//	vector <node *> nv; 

			for(int i = 0; i < row; i++)
			{

				for(int j = 0; j < col; j++)
				{
					np = new node; 
					np->visited = -1;
					lm.nv.push_back(np);
				}

			}




			for(int i = 0; i < row ; i++)
			{

			for(int j = 0 ; j < col; j++)
			{
		
				//get a node
				np = lm.nv[i*col+j ];

				//look to your right if possible
				for(int k = j+1; k < col && layout[i][k] != 'M' ; k++)
				{
				}

				//if i got to the end add this adjacency lists for both
				if( (k == col) ) 
				{
					np->adjl.push_back(i*col + j+1);
					
				}

				//look to your left if possible
				for(int k = j-1; k >= 0 && layout[i][k] != 'M' ; k++)
				{
				}

				
				if( k < 0 ) 
				{
					np->adjl.push_back(i*col + j-1);
				}


				//look up if possible 	
				for(int l = i-1; l >= 0 && layout[l][j] != 'M'; l--)
				{
				}


				//if i get here store the adjacent
				if( l < 0 ) 
				{
					np->adjl.push_back( (i-1)*col + j);
				}


				//look down if possible	
				for(int l = i+1; l < row && layout[l][j] != 'M'; l--)
				{
				}

				
				if( l == row ) 
				{
					np->adjl.push_back( (i-1)*col + j);
				}
				

			}

		}

		for(i = 0; i < lm.nv.size(); i++)
		{
			lm.dfs( lm.nv[i] );


		}

		return lm.vstd;
	}
};
