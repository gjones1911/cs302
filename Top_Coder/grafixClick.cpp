// BEGIN CUT HERE
// PROBLEM STATEMENT
// 

Note: This problem statement includes an image that may
not appear if you are using a plugin. For best results, use the Arena
editor.




You have been hired to work on a graphics application called grafix. The
interface includes several buttons that the user can click on during 
a session. One of these is the Save button, which appears in the
application window as a rectangle composed of pixels. The location of
each pixel is given by a pair of integers that specify the pixel's row
number and column number, in that order. Such an integer pair is called
the window coordinates of a pixel. Rows are numbered from top to
bottom, and columns are numbered from left to right.




The top left corner of the Save button has window coordinates (20, 50),
meaning that it is a pixel occupying the 20th row from the top and the
50th column from the left. The bottom right corner of the Save button
has window coordinates (39, 99). If the user clicks her mouse on any
pixel within or on the border of the rectangle defined by these points,
she is considered to have activated the Save button. Below is a magnified
image of the Save button, showing the numbers of its rows and columns.





  






Given a sequence of mouse clicks, your job is to determine which ones have
activated the Save button. For each mouse click, the vector <int>
mouseRows contains the row number of its window coordinates, 
while the vector <int> mouseCols contains its column
number. The values in each vector <int> are in corresponding order,
so the nth element of mouseRows and the nth element
of mouseCols make up the window coordinates of the nth
mouse click. You are to return a vector <int> containing, in ascending 
order, the zero-based index of each mouse click that activates the
Save button.




DEFINITION
Class:grafixClick
Method:checkSaveButton
Parameters:vector <int>, vector <int>
Returns:vector <int>
Method signature:vector <int> checkSaveButton(vector <int> mouseRows, vector <int> mouseCols)


NOTES
-Window coordinates are not the same as Cartesian coordinates. Follow the definition given in the first paragraph of the problem statement.


CONSTRAINTS
-mouseRows contains between 1 and 50 elements, inclusive
-mouseRows and mouseCols each contain the same number of elements
-each element in mouseRows is between 0 and 399, inclusive
-each element in mouseCols is between 0 and 599, inclusive


EXAMPLES

0)
{20, 39, 100}
{99, 50, 200}

Returns: { 0,  1 }

The first click, with window coordinates (20, 99), falls on the top right corner of the Save button. The second click is on the bottom left corner at (39, 50). The third click has window coordinates (100, 200) and falls outside the button.

1)
{0, 100, 399}
{0, 200, 599}

Returns: { }

None of these clicks activate the Save button.

2)
{30}
{75}

Returns: { 0 }

Bull's-eye!

3)
{10, 20, 30, 30, 30, 30, 34, 35, 345}
{10, 20, 30, 50, 60, 80, 34, 35, 345}

Returns: { 3,  4,  5 }

4)
{57, 28, 18, 25, 36, 12, 33, 44, 13, 32,
 32, 51, 11, 27, 8, 51, 17, 34, 10, 16,
 47, 57, 20, 57, 32, 14, 13, 37, 10, 16,
 49, 37, 52, 8, 18, 44, 50, 43, 11, 1,
 21, 22, 17, 35, 28, 53, 56, 16, 11, 44}
{146, 22, 41, 88, 123, 99, 43, 110, 25, 64,
 141, 110, 70, 34, 99, 103, 60, 64, 142, 109,
 133, 144, 72, 68, 25, 32, 21, 140, 30, 105,
 32, 72, 114, 97, 35, 131, 103, 110, 133, 81,
 125, 36, 76, 78, 77, 47, 50, 94, 22, 20}

Returns: { 3,  9,  17,  22,  31,  43,  44 }

// END CUT HERE
#line 124 "grafixClick.cpp"
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>

using namespace std;

class grafixClick 
{
	
	public:
	vector <int> checkSaveButton(vector <int> mouseRows, vector <int> mouseCols)
	 {
		
	}
};
