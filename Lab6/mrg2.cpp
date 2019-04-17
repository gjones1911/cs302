/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>

using namespace std;

void recursive_sort(vector <double> &v, vector<double> &tmp, int start, int size, int print)
{

	int i, j,k,l,mid, min;
	vector<double> v1;
	vector<double> v2;
	vector<double> vt1;
	vector<double> vt2;

	//get the mid
	mid = size/2; 

	cout<<"the mid is: "<<mid<<"\n";	
	
	//check for the base case
		if(size == 1)
		{
			//tmp = v1;

			cout<<"we hit a size of 1\n";	

			for(i = 0; i < v.size();i++)
			{
				printf("element %d:",i,v[i] );
			}

			return;
		}

		if(size > 1)
		{
			printf("B: %d %d ",start, size);
		}

		//get 1st and 2nd half of V
		for(i = 0; i < mid ;i++)
		{
			v1.push_back(v[i] );
		}

		cout<<"\n";

		for(i = mid; i < v.size() ;i++)
		{

			v2.push_back(v[i] );
		}

		///////////////////////////////////////	
		cout<<"The first half is\n";

		for(i = 0; i < v1.size() ;i++)
		{

			printf(" %5.2lf ",v1[i]);
		}


		cout<<"\n";

		cout<<"The Second half is\n";
		for(i = 0; i < v2.size() ;i++)
		{

			printf(" %5.2lf ",v2[i]);
		}


		//now continue spliting until there is only 1 element


		vt1.resize( v1.size() );
		vt2.resize( v2.size() );


		recursive_sort(v1,vt1,start,v1.size,1);
		recursive_sort(v2,vt2,mid,v2.size,1);

}

int main(int argc, char** argv)
{
	int i, j,k,l,mid, min;

	vector<double> v1;
	vector<double> tmp;

	cout<<"the size of argv is "<<argc;
	cout<<"\n";

	sscanf(argv[1] , "%d",&min);

	while(cin>>mid)
	{
		v1.push_back(mid);
	}


	recursive_sort(v1,tmp,0,v1.size(),1);

	cout<<"the min is :"<<min<<"\n";
	
	
	
	return 0;

	
}
