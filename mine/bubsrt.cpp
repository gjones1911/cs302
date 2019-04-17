
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
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
	vector <int> vec;
	vector <int> vec2;
	
	char buff[100];

	int l,m,n,o,p;

	stringstream ss;

	string number,shldr;

	if(argc > 1)
	{
		
		if(argc == 3)
		{
//			cout<<"arg checker\n";
			shldr = argv[1];

			if( shldr == "v")
			{
//				cout<<"it got here\n";
				sscanf(argv[2]," %d ",&o);

				cout<<"o is "<<o<<"\n";
			}

			for(m = o; m >= 0; m--)
			{
				vec.push_back(m );

			}
		}
		else
		{
			cerr<<"Usage: ./bsrt v #of loops\n";
			exit(0);
		}

	}

	string garbage;

	int num,i,j,k,tmp,lst,bgr;

	bool notinthere = false;
if(argc == 1)
{
	cout<<"Give me some numbers to sort please sir...\n";


	while(cin>>num && vec.size() < 5)
	{
		vec.push_back(num);
	}
}
	cin.clear();
if(argc == 0)
{
	cout<<"give me some numbers to look for:\n";

//	cin>>garbage;

	while(cin>>num && (vec2.size() <= vec.size() ) )
	{
		vec2.push_back(num);

	}
}

if(argc == 4)
{
	shldr = argv[3];

}
if(vec.size() <= 25 || (argc == 4 && shldr == "p" ) )
{
	cout<<"the original vector is:\n";

	for(i = 0 ; i < vec.size() ; i++)
	{
		printf("%4d",vec.at(i) );
	}

	cout<<"\n";
}	
	bool iswapped = true; 

	//start bubble sorting
	for(i = vec.size() - 1 ; iswapped && i >= 0 ; i--)
	{

		iswapped = false;
		for(j = 0; j < i ; j++)
		{
			
			if( vec[j] > vec[j+1] )
			{
				iswapped = true;;
				bgr = vec[j];
				vec[j] = vec[j+1];
				vec[j+1] = bgr;
			}
		}

	}


if(vec.size() <= 25 || (argc == 4 && shldr == "p" ) )
{

	cout<<"the bubble sorted array is:\n";

	for(i=0; i < vec.size(); i++)
	{
	//	sprintf(buff,"vec at %*d is %4d "," ",3,i,vec[i] );

		printf("%4d ",vec[i] );

//		cout<<shldr;
	
	}


	cout<<"\n";
}
	return 0;
}
