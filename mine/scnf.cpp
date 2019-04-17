
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <sstream>

using namespace std;

int main(int argc, char ** argv)
{

	char buf[5];

	int i, j,k, h, g, f,scn;

	string s;
	stringstream  ss;

	cout<<"Give me a damn number!!!!\n";

	string numstr = "250";
	string numstr2 = "250";
	string numstr3 = "250";

//	if(cin>>i)
//	{
		//scan the given string into the 3 given ints hopefully
		scanf(" %d ",&scn);
		sscanf(numstr.c_str()," %d ",&i);
		sscanf(numstr.c_str()," %d ",&j);
		sscanf(numstr.c_str()," %d ",&k);
//	}

	cout<<"the scn is:       "<<scn<<"\n";
	cout<<"the num is:       "<<i<<"\n";
	cout<<"the num  + 5  is: "<<j + 5<<"\n";
	cout<<"the num  + 15 is: "<<k + 15<<"\n";


	cout<<"now for some sprintf stuff......\n";

	//use the buffer called buff to print some stuff to it.
	//
	sprintf(buf,"scn is:%d i is: %d j+5 is: %d ",scn,i,j+5);

	//unload the buffer into a string

	numstr3 = buf;

	cout<<"now numstr3 is the buffer or: "<<numstr3<<"\n";



	


	return 0;
}
