/*
// BEGIN CUT HERE
// PROBLEM STATEMENT
// My friend has a "bigital" clock. It displays 12-hour times in the 
form hh:mm:ss, using 6 columns of 1 watt light bulbs.
Each column displays its value in binary, where a light is off
to indicate a 0 and is on to indicate a 1. The clock can display all
12-hour times, from 01:00:00 to 12:59:59.

For example, 03:22:59 requires 8 lights to be on and looks like this:

    0	0   0	0   0	1
    0	0   0	0   1	0
    0	1   1	1   0	0
    0	1   0	0   1	1

Given tStart and tEnd, return the total number of kilowatt
hours used by the clock in displaying all the times from tStart to tEnd,
inclusive. The start and end time are less than 12 hours apart. 


DEFINITION
Class:Bigital
Method:energy
Parameters:string, string
Returns:double
Method signature:double energy(string tStart, string tEnd)


NOTES
-A kilowatt hour is the energy used when using 1000 watts for an hour.
-A return value with either an absolute or relative error of less than 1.0E-9 is considered correct.


CONSTRAINTS
-tStart and tEnd will each contain exactly 8 characters.
-tStart and tEnd will each be in the form hh:mm:ss, where hh is between 01 and 12, inclusive, mm is between 00 and 59, inclusive, and ss is between 00 and 59, inclusive.


EXAMPLES

0)
"12:00:00"
"12:00:00"

Returns: 5.555555555555555E-7




    Two bulbs are on for just one second. This require 2 watt seconds
   which (60 seconds per minute, 60 minutes per hour, 1000 watts per 
   kilowatt) is .000000555555555 kilowatt hours of energy.



1)
"12:59:59"
"01:00:00"

Returns: 3.0555555555555556E-6



    Here the clock is running for 2 seconds, with lots of lights on for
   the first second and only 1 on for the second second.


2)
 "12:01:00"
"12:00:00"

Returns: 0.08392277777777778



    This is almost the full 12 hours.


// END CUT HERE
#line 81 "Bigital.cpp"
*/
#include <string>
#include <vector>
#include <iostream>
#include<algorithm>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <cmath>


using namespace std;

class Bigital 
{
	
	public:
	double energy(string tStart, string tEnd)
	 {
		stringstream ss;

		int num = 0 ;

		string hldr;
		tStart.replace(2,1," ");
		tStart.replace(5,1," ");

		tEnd.replace(2,1," ");
		tEnd.replace(5,1," ");

		ss.str(tStart);

		while(ss>>hldr)
		{
			num = 0;
			cout<<"holder is 0 "<<hldr[0]<<"\n";
			cout<<"holder is 1 "<<hldr[1]<<"\n";
			num += digit2watts(hldr[0]);
			num += digit2watts(hldr[1]);

			cout<<"The number is:"<<num<<"\n";
		}


		
		int h0,h,m0,m,s0,s;

		double rv;

		string huh = sec2str(3665);

		return rv;

	}

	int digit2watts( char c)
	{
		int n = c - '0';

		int count = 0;

		for(int i = 0; i < 4 ;i++)
		{
				if(n & (1 << i)) count++; 
		}

		return count;
	}

	string sec2str(int sec)
	{
		string rs;
		stringstream ss;
		string h,m,s;
		int  hrs = sec/3600;
		int  smin = sec%3600;
		int   msec = smin/60 ;

		int sec = smin%60;
	//	double total = min + hrs + secs;
		sprintf(rs.c_str(),"%*1d:%d:%d",0,hrs,msec,sec);

		cout<<"The return string is: "<<rs<<"\n";
	
		return rs;
	}

	int str2sec(string s)
	{
		int rv;

		stringstream ss;
		double h,m,s;
		
		ss.str(s);

		ss>>h;
		ss>>m;
		ss>>s;

		

		return rv;


	}

};


int main(void)
{

	vector<int> vh;
	int i, j, k, num, ihldr,cnt;

	string s, shldr, tstr,tend;

	Bigital Bgt;
	cout<<"give me a start time in the form: hh:mm:ss\n";

	getline(cin,s);
	
	tstr = s;
	

	cout<<"give me a end time in the form: hh:mm:ss\n";

	getline(cin,s);
	
	tend = s;

	double dbl;

	dbl = Bgt.energy(tstr,tend);

return 0;
}
