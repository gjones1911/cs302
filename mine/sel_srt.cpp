
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: sel_srt.cpp//
//Created By: Gerald Jones                                                                     //
//Purpose: My own version of a selction sort. also test my is_n_ivec which searchs a integer   //
//         for a given number
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <sstream>

using namespace std;


//this looks for the given number in the given vector and counts how many times it is there
void is_n_ivec( int &num,vector <int> &srchv)
{
	int i, j, btlst,cnt;
	
	cnt = 0;

	//search the vector for the number
	for(i = 0; i < srchv.size(); i++)
	{
		if( (1<<num) & (1<<srchv[i]) )
		{

			cnt++;

		}
	}

	if(cnt > 0) printf("the number %d is there %d times\n",num,cnt);

	else printf("the number %d was not found!!!!\n",num);

}

//Usuage----------
//will  take in a set of numbers and sort them -- ./slsrt 
//will  take in a set of numbers and search for another set of numbers in the first set-- ./slsrt l
//will create a reverse ordered vector and sort it with selection sort-- ./slsrt v #of_numbers
//will take in  a line and read the numbers into some ints-- ./slsrt gl 
int main(int argc, char** argv)
{
	vector <int> vec;
	vector <int> hv;
	vector <int> vec2;
	vector <string> sv;

	char * buff;

	int l,m,n,o,p;

	int num,i,j,k,tmp,lst,bgr,minval, minidx,h,r,mn,sc;
	stringstream ss;

	string number,shldr,s,hr,tm;

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
		else if(argc == 2)
		{
			if( (*argv[1])  == 'l') 
			{
				cout<<"so were gonna search.....\n";

			}
		
			else if( (*argv[1]) == 'g')
			{
				if(sv.size() == 1 && sv[0] == "bob") 
				{
					cout<<"he did it!!\n";
					exit(1);
				}
				else
				{
					cout<<"Give a time in the form: hh:mm:ss\n";



					while(getline(cin,tm) )
					{
						s = tm;


						cout<<"you gave me: "<<tm<<"\n";

						for(i=0; i < tm.size(); i++)
						{
							cout<<"ughghgh\n";
							if(tm.at(i) == ':' ) hv.push_back(i);

						}

						for(i = 0 ; i < hv.size(); i++)
						{
							tm.replace(hv[i],1," ");
						}

						ss.str(s);

						sv.clear();

						while(ss>>s)
						{ 
							sv.push_back(s);
						}


						cout<<"the vector is:\n";

						for(i = 0; i < sv.size();i++)
						{
							printf("%*%ss",3," ",sv[i].c_str());
						}

						if(sv.size() == 1 && sv[0] == "bob") 
						{
							cout<<"he did it!!\n";
							exit(1);
						}
						//	char time[] = s.c_str;

						cout<<"gets here a\n";
						cout<<"you gave me alpha:"<<tm<<"\n";
						sscanf(tm.c_str()," %d  %d  %d ",&h,&mn,&sc) ;

					}

					cout<<"gets here b\n";
					buff = (char *) tm.c_str();
					sprintf(buff,"your time is %d:%d:%d\n",h,mn,sc);

					//s = buff;

					cout<<s<<"\n";
					exit(0);
				}				

			}
		}
		else
		{
			cerr<<"Usage: ./bsrt v #of loops\n";
			exit(0);
		}

	}

	string garbage;
	if(argc == 2 ) 
	{
		garbage = argv[1];
	}

	bool notinthere = false;

	if(argc == 1 || (argc == 2 && garbage == "l" ) )
	{
		cout<<"Give me some numbers to sort please sir...\n";


		while(cin>>num && vec.size() < 26)
		{
			vec.push_back(num);
		}
	}
	cin.clear();
	if(argc == 2 && garbage == "l")
	{
		cout<<"give me some numbers to look for too:\n";

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

	//start selection sorting
	//go through finding the smallest for that sweep
	//and putting it in the front
	for(i = 0 ; i < vec.size()-1  ; i++)
	{

		minidx = i;
		minval = vec[i];
		for(j = minidx+1; j < vec.size() ; j++)
		{

			//compare vec[j] with current minimum
			if( vec[j] <  vec[minidx] )
			{
				//save the lesser of the comparison
				minval = vec[j];
				minidx = j;
			}
		}

		//now do swap
		tmp = vec[i];
		vec[i] = minval;
		vec[minidx] = tmp;


	}


	if(argc == 2 && garbage == "l")
	{
		for(i = 0 ; i < vec2.size();i++)
		{
			is_n_ivec(vec2[i], vec);
		}
	}


	if(vec.size() <= 25 || (argc == 4 && shldr == "p" ) )
	{

		cout<<"the selection  sorted array is:\n";

		for(i=0; i < vec.size(); i++)
		{
			//	sprintf(buff,"vec at %*%sd is %4d "," ",3,i,vec[i] );

			printf("%4d ",vec[i] );

			//		cout<<shldr;

		}


		cout<<"\n";
	}
	return 0;
}
