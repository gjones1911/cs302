/*
// BEGIN CUT HERE
// PROBLEM STATEMENT
// 
You've designed a computer and implemented all the common arithmetic operators: addition, subtraction, multiplication and integer division.  However, your budget was very limited, so you could only afford to place a single register in the computer.  The register can store any non-negative integer value.  Since there is only one register, there is no need to identify the store location or the operands of each operation or its result.  The programming language has four instructions: '+', '-', '*' and '/'.  Each instruction performs the corresponding operation using the value in the register as both its parameters.  It then stores the result in the same register, overwriting the previous content.

A program for your computer is a sequential list of zero or more instructions.  You want to show that, even with its limitations, your newly constructed computer is powerful.  You will be given two ints s and t.  Return the shortest program that finishes with a value of t in the register if it contained s before executing.  If there is more than one possible answer, return the one that comes earliest lexicographically.  If there is no program that can do the job, return ":-(" (quotes for clarity) instead.


DEFINITION
Class:OneRegister
Method:getProgram
Parameters:int, int
Returns:string
Method signature:string getProgram(int s, int t)


NOTES
-A string comes lexicographically earlier than another one of the same length if and only if it contains a symbol with a lower ASCII code in the first position at which they differ. The operators in ascending order of ASCII code are: '*', '+', '-' and '/'.
-If the division operation is used when the register contains a zero, the program will give an error and terminate with a zero value in the register.


CONSTRAINTS
-s and t will be between 1 and 1000000000 (10^9), inclusive.


EXAMPLES

0)
7
392

Returns: "+*+"

The program executes as follows:

 Reg | Ins | Res
-----+-----+-----
   7 |  +  |  14
  14 |  *  | 196
 196 |  +  | 392


1)
7
256

Returns: "/+***"



2)
4
256

Returns: "**"



3)
7
7

Returns: ""

A trivial program.

4)
7
9

Returns: ":-("

No solution.

5)
10
1

Returns: "/"



// END CUT HERE
//
*/
#line 85 "OneRegister.cpp"
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>
#include <queue>

using namespace std;

class OneRegister 
{
	
	public:
	string getProgram(unsigned long long  s, unsigned long long  t)
	 {
//		int i,j,k;

		unsigned long long ns = s;
		unsigned long long nt = t;

		string sdstr = ":-(";
		
		string nstr;

		string estr = "";

		if(ns == nt ) return estr;

		queue < pair< string,unsigned long long > > qq;

		qq.push(make_pair("*",ns*ns));
		qq.push(make_pair("+",ns+ns));
		qq.push(make_pair("/",ns/ns));

		pair <string, unsigned long long > stuff;

//		cout<<"the thing is : "<<qq.front().first<<" and "<<qq.front().second<<"\n"; 

		while(qq.size() > 0)
		{	
				stuff = qq.front();
			
				if(stuff.second == nt) return stuff.first;
				
				if( stuff.second * stuff.second  <= t && stuff.second != 1 )
				{
					nstr = stuff.first + "*";
					qq.push(make_pair(nstr, stuff.second *stuff.second));
				}
				if( stuff.second + stuff.second <= t )
				{
					nstr = stuff.first + "+";
					qq.push(make_pair(nstr, stuff.second + stuff.second));
				}

				qq.pop();

		}
		
		return sdstr;

	}
};
