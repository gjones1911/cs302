
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <string>
using namespace std;


int main(void)
{
	int i , j , k;

	string s,tmps;

	s.resize(10);
	tmps.resize(10);

	for(i = 0 ; i < (1<<9) ;i++)
	{
			for(j = 0 ; j < 9 ; j ++)
			{

				(i & (1<<j) ) ? s[j] = 'A' : s[j] = 'B';

			}

		if(i > 0 && i < (1<<9)-1)
		{
			for(k = 0 ; k < 9; k ++)
			{
				tmps = s;
				tmps.replace(k,0,"C");
				cout<<tmps<<"\n";
			}
			tmps = s + "C";
			cout<<tmps<<"\n";;
		}

	}

	
	return 0;
}

