
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


using namespace std;


int main(int argc, char **argv)
{
		
	string name;

	vector <string> people;

	int i,j,k, l;

	//take in names on std input
	while(cin>>name)
	{
		people.push_back(name);

	}


	printf("your combo's are:\n");

	//go from 1 to 2^people.size() using 1<<pep.size()
	for(i = 1; i < ( 1<< people.size() ) ; i++)
	{
		
		//go from 0 to peoples size -1 printing the result
		//of i and 2^j if is true print 1 , false print 0
		//this prints the bit strings in every combo
		for(j=0; j < people.size() ;j++)
		{
			printf("%c",(i & (1<<j) )? '1' : '0');
		}

		//now print the names corresponding to each combo
		for(j = 0; j < people.size(); j++)
		{
			if(i & (1<<j) ) printf(" %s", people[j].c_str());

		}
		
		printf("\n");
	}


	
	
	return 0;
}
