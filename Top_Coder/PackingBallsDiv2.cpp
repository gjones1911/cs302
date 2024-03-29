/*
// BEGIN CUT HERE
// PROBLEM STATEMENT
// We have R red, G green, and B blue balls.
We want to divide them into as few packages as possible.
Each package must contain 1, 2, or 3 balls.
Additionally, each package must be either a "normal set" (all balls in the package have the same color), or a "variety set" (no two balls have the same color).
Compute and return the smallest possible number of packages.

DEFINITION
Class:PackingBallsDiv2
Method:minPacks
Parameters:int, int, int
Returns:int
Method signature:int minPacks(int R, int G, int B)


CONSTRAINTS
-R, G, and B will each be between 1 and 100, inclusive.


EXAMPLES

0)
4
2
4

Returns: 4

We have 4 red, 2 green, and 4 blue balls.
Clearly, we need at least four packages to store 10 balls.
One possibility of using exactly four packages looks as follows: RGB, RG, RR, BBB.
(I.e., the first package has 1 ball of each color, the second package has a red and a green ball, and so on.)

1)
1
7
1

Returns: 3

Here the only possible solution is to have one package with RGB and two packages with GGG each.

2)
2
3
5

Returns: 4



3)
78
53
64

Returns: 66



4)
100
100
100

Returns: 100



// END CUT HERE
*/

#line 73 "PackingBallsDiv2.cpp"
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>

using namespace std;

class PackingBallsDiv2 
{
	
	public:
	int minPacks(int R, int G, int B)
	 {
		 int nm, nm2;
int total = R+G+B;
		vector <int> v ;
		v.push_back(R);
		v.push_back(G);
		v.push_back(B);

		sort(v.begin(), v.end()) ;

		int pcknm;

		pcknm = v[0];

		for(int i =0; i < v.size();i++)
		{
			v[i] -= pcknm;
		}

int nm3;
		for(int i =0; i < v.size();i++)
		{
			 pcknm += v[i] /3;

			 v[i] = v[i] % 3;
		}

		
		sort(v.begin(), v.end()) ;

		if(v[1] == 0 && v[2] == 0 )
		{
			return pcknm;
		}
		else if(v[1] ==0 && v[2] > 0)
		{

			return pcknm + 1;
		
		}
		else if(v[1] == 1 && v[2] ==  1 )
		{
			


			return pcknm + 1;
		
		}
		else if(v[1] == 1 && v[2] ==  2 )
		{
			return pcknm +2;
		
		}
		else if(v[1] == 2 && v[2] ==  2 )
		{
			return pcknm +2;
		
		}
	}
};
