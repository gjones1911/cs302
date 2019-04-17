
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

int main(int argc, char** argv)
{
	vector <int> vec;
	vector <int> vec2;

	string garbage;

	int num,i,j,k,tmp,lst;

	bool notinthere = false;

	cout<<"Give me some numbers to sort please sir...\n";


	while(cin>>num && vec.size() < 5)
	{
		vec.push_back(num);
	}

	cout<<"give me some numbers to look for:\n";

	cin.clear();
//	cin>>garbage;

	while(cin>>num && (vec2.size() <= vec.size() ) )
	{
		vec2.push_back(num);

	}

	cout<<"the original vector is:\n";

	for(i = 0 ; i < vec.size() ; i++)
	{
		printf("element %d is: %d\n",i, vec.at(i) );
	}

	cout<<"the search vector is:\n";
	for(i = 0 ; i < vec2.size() ; i++)
	{
		printf("element %d is: %d\n",i, vec2.at(i) );
	}


	lst = 0;

	for(i = 0 ; i < vec2.size();i++)
	{
		notinthere = true;
		for(j = 0; j < vec.size();j++)
		{
	lst = 0;
			if(vec2[i] == vec[j] ) notinthere = false;

			if(vec2[i] == vec[j] )
			{/*
				printf("vec2 is: %d \n",vec2[i]);
				printf("vec is: %d \n",vec[j]);
				
				printf("1<<vec2[%d] is : %d\n",j,(1<<vec2[j]  ) );
				printf("lst  is : %d\n", lst );
			*/
				lst ^= (1<<vec2[j] ) ;

			//	printf("After lst |= (1<<vec2[j]) lst  is : %d\n", lst );
				
			}

			
		}

		int bob = 0;

		if( !(notinthere) )
		{
			cout<<"the number "<<vec2[i]<<" is there\n";

		}
		else cout<<"the number "<<vec2[i]<<" is not in there!!!\n";
		if( (1<<vec2[i] ) & lst  ) cout<<"the number with binary "<<vec2[i]<<" is there\n";
	}

	cout<<"in binary i hope..\n";
/*

	//	for(i = 0; i < vec.size() ;i++)
	//	{
	for(j = 1; j <= (1<<vec[1] ) ; j++)
	{
		for(k = 0; k <= vec.at(1) ;k++)
		{
			printf("%c", (j & (1<<k)) ? '1' : '0');
		}

		cout<<"\n";	
	}
	cout<<"\n";	
	//	}
*/

	return 0;
}
