// BEGIN CUT HERE
// PROBLEM STATEMENT
// This problem statement contains superscipts that may not display properly outside the applet.

You are given four ints A, B, C and D. Return "divisible" (quotes for clarity) if AB is divisible by CD. Return "not divisible" otherwise.

DEFINITION
Class:BigFatInteger2
Method:isDivisible
Parameters:int, int, int, int
Returns:string
Method signature:string isDivisible(int A, int B, int C, int D)


NOTES
-The return value is case-sensitive.
-Positive integer y divides a positive integer x if and only if there is a positive integer z such that y*z=x. In particular, for each positive integer x, both 1 and x divide x.


CONSTRAINTS
-A, B, C and D will each be between 1 and 1,000,000,000 (109), inclusive.


EXAMPLES

0)
6
1
2
1

Returns: "divisible"

Here, AB = 61 = 6 and CD = 21 = 2. 6 is divisible by 2.

1)
2
1
6
1

Returns: "not divisible"

2 is not divisible by 6.

2)
1000000000
1000000000
1000000000
200000000

Returns: "divisible"

Now the numbers are huge, but we can see that AB is divisible by CD from the fact that A=C and B>D.

3)
8
100
4
200

Returns: "not divisible"

We can rewrite 8100 as (23)100 = 2300.
Similarly, 4200 = (22)200 = 2400.
Thus, 8100 is not divisible by 4200.

4)
999999937
1000000000
999999929
1

Returns: "not divisible"

Here A and C are distinct prime numbers, which means AB cannot have C as its divisor.

5)
2
2
4
1

Returns: "divisible"



// END CUT HERE
#line 90 "BigFatInteger2.cpp"
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <map>

using namespace std;

class BigFatInteger2 
{
	
	public:
	string isDivisible(int A, int B, int C, int D)
	 {
		map <long long, long long> As;
		
		map <long long , long long> Bs;

		map <long long, long long>::iterator mit;

		map <long long, long long>::iterator rit;
		map <long long, long long>::iterator bit;
		long long anm, bnm;

		long long cnt = 0;

		const string yes = "divisible";
		const string no = "not divisible";

		bool ist = false;

		//loop through the A map collecting prime factors of 
	
		for(int i = 2; i*i <= A   ; i++)
		{  cnt = 0;

			while(A%i == 0)
			{
				A = A/i;
				cnt++;
				As.insert(make_pair(i,cnt));
			}

		}
		
		if(A != 1)
		{
			if(cnt == 0) As.insert(make_pair(A,1));
		}
		cnt = 0;
		for(int i = 2; i*i <= C   ; i++)
		{
			cnt = 0;
			while(C%i == 0)
			{
				C = C/i;
				cnt++;
				Bs.insert(make_pair(i,cnt));
			}

		}

		if(C != 1)
		{
			if(cnt == 0) Bs.insert(make_pair(C,1));

		}
		
		cout<<"A is\n";
		for(mit = As.begin();mit != As.end();mit++)
		{
			cout<<"{"<<mit->first<<" , "<<mit->second<<"}\n";
		}
		
		cout<<"B is\n";
		for(mit = Bs.begin();mit != Bs.end();mit++)
		{
			cout<<"{"<<mit->first<<" , "<<mit->second<<"}\n";
		}

	//	tit
		for(bit = Bs.begin();bit != Bs.end();bit++)
		{
			
		//	for(rit = As.begin();rit != As.end();rit++)
		//	{
				mit = As.find(bit->first);
				if(mit == As.end()) return no;

				if( !(mit->second > bit->second) ) return no;
		//	}
		}
	 		return yes;
	 }
};
