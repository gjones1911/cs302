// BEGIN CUT HERE
// PROBLEM STATEMENT
// Given a number x, we can define p(x) as the product of the digits of x.  We can then form a sequence x, p(x), p(p(x))...  The persistence of x is then defined as the index (0-based) of the first single digit number in the sequence.  For example, using 99, we get the sequence 99, 9*9 = 81, 8*1 = 8.  Thus, the persistence of 99 is 2.  You will be given n, and you must return its persistence.

DEFINITION
Class:PersistentNumber
Method:getPersistence
Parameters:int
Returns:int
Method signature:int getPersistence(int n)


CONSTRAINTS
-n will be between 0 and 2,000,000,000, inclusive.


EXAMPLES

0)
99

Returns: 2

The example from the problem statement.

1)
268

Returns: 4

The sequence here is 268, 96, 54, 20, 0.

2)
6

Returns: 0

6 is already a single-digit number.

3)
68889789

Returns: 3



4)
86898

Returns: 7



// END CUT HERE
#line 56 "PersistentNumber.cpp"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <vector>
#include <cstdio>

using namespace std;

class PersistentNumber 
{
	
	public:
	int getPersistence(int n)
	 {
		stringstream ss;
		int idx =1;
		int nm = 1;
		bool go = true;
		
		string holder;
		vector<string> dig;		
		ss<<n;
		ss>>holder;
		if(holder.size() == 1) return 0;
		ss.clear();
		ss.str("");
		while(go)
		{
		
			ss.clear();
				
			ss.str("");
			nm = 1;	
			for(int i =0; i < holder.size();i++)
			{
				nm *= holder.at(i) - '0';
				
			}
			stringstream ss;
				ss<<nm	;
				ss>>holder;
			if(holder.size() == 1)
			{
				go = false;
			}
			else
			{
				idx++;
			}
		
		//cout<<holder<<endl;
		}
		
		return idx;

	}
};
