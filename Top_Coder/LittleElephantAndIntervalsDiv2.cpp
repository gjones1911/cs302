// BEGIN CUT HERE
// PROBLEM STATEMENT
//
/*
Little Elephant from the Zoo of Lviv has some balls arranged in a row. Each ball can be painted in one of two possible colors: black or white. Initially all the balls are painted white. 




You are given an int M, which represents the number of balls in the row. 
The balls are numbered from left to right, starting from 1. 
You are also given two vector <int>s L and R. 
To repaint balls, Little Elephant wants to use a robot. 
The robot will paint the balls in several consecutive stages.
For each i, the i-th stage (1-based index) will look as follows:
First, the robot will choose one of the two colors: white or black.
Then, the robot will paint the balls with indices from L[i-1] to R[i-1], inclusive, using the chosen color.
(Painting a ball covers all previous layers of paint.)




Return the number of different colorings Little Elephant can get after the last stage. (Two colorings are considered different if there exists some ball that is white in one coloring and black in the other one).


DEFINITION
Class:LittleElephantAndIntervalsDiv2
Method:getNumber
Parameters:int, vector <int>, vector <int>
Returns:int
Method signature:int getNumber(int M, vector <int> L, vector <int> R)


CONSTRAINTS
-M will be between 1 and 100, inclusive. 
-L will contain between 1 and 10 elements, inclusive.
-R will contain the same number of elements as L.
-Each element of R will be between 1 and M, inclusive.
-i-th element of L will be between 1 and R[i], inclusive.


EXAMPLES

0)
4
{1, 2, 3}
{1, 2, 3}

Returns: 8

In the three stages the robot will choose the color for balls number 1, 2, and 3. The choices are independent of each other. The last, fourth ball will always remain white. Thus there are 2*2*2 = 8 different colorings.

1)
3
{1, 1, 2}
{3, 1, 3}

Returns: 4

In the first stage the robot colors all three balls. The color chosen for the first stage does not matter, because in the second stage the robot will repaint ball 1, and in the third stage it will repaint balls 2 and 3.

2)
100
{47}
{74}

Returns: 2



3)
100
{10, 20, 50}
{20, 50, 100}

Returns: 8



4)
42
{5, 23, 4, 1, 15, 2, 22, 26, 13, 16}
{30, 41, 17, 1, 21, 6, 28, 30, 15, 19}

Returns: 512


*/
// END CUT HERE
#line 90 "LittleElephantAndIntervalsDiv2.cpp"
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>
#include <set>


using namespace std;

class LittleElephantAndIntervalsDiv2 
{
	
	public:
	int getNumber(int M, vector <int> L, vector <int> R)
	 {

		set <string> hld;
		//int num;

		//string.replace(idx, n,n,char c)
		
		int size = L.size();

		vector <int> holder;
		holder.resize(size);
		
		//loop through shifting 1's into the i position
		for(int i = 0; i < (1<<size); i++)
		{
			int left = L[i]-1;
			int right = R[i]-1;
			int n = right - left + 1;

			string balls = string(M,'w');
			for(int j = 0; j < size ;j++)
			{
				int left = L[j]-1;
				int right = R[j]-1;
				int n = right - left + 1;
				if( (i &(1<<j)) )
				{
					balls.replace(left, n, n,'b'); 
				}
				else
				{
					balls.replace(left, n, n,'w'); 
				}

			}
				hld.insert(balls);
		}
			
		return hld.size();	
	}
};
