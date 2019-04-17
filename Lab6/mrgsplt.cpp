
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

void recursive_sort(vector<double> &v,vector<double> &tmpv,int start, int size, int print)
{

	int i, j, k, l, min;

	vector<double> v1;
	vector<double> v2;
	vector<double> vt1;
	vector<double> vt2;

	//get mid
	int mid = size/2;
	
	//as per lab instuction print a B when size >1
	if(size > 1)
	{
		printf("B: %5d %5d \n",start,size);
	}
	
	//the base case is when there is only 1 element
	if(size == 1)
	{
		cout<<"we hit a size of 1\n";

		for(i = 0; i < v.size();i++)
		{
			printf(" %5.2lf:",v[i] );
		}


		tmpv.push_back(v[0]);
		printf("E: %5d %5d \n",start,size);
		return;
	}

	//get the first half
	for(i = 0; i < mid; i++)
	{
		v1.push_back(v[i] );
	}

	//get the 2nd half
	for(i = mid; i < v.size(); i++)
	{
		v2.push_back(v[i] );
	}
//	recursive_sort( 

	cout<<"\nthe 1st half is:\n";

	for(i = 0; i < v1.size(); i++)
	{
		printf(" %5.2lf",v1[i] ); 
	}

	cout<<"\nthe 2nd half is:\n";
	for(i = 0; i < v2.size(); i++)
	{
	
		printf(" %5.2lf",v2[i] ); 
	
	}

	cout<<"\n";

	cout<<"\n\nand now the night time....\n";

	j = 0;
	k = 0;
	
	//these will fill vt with a element
	recursive_sort(v1, vt1, start, v1.size(), 1);
	recursive_sort(v2, vt2, mid, v2.size(), 1);
	
	cout<<"\n";

	//next copy tmp back into original
	return;
}


int main(int argc, char ** argv)
{

	int i,j,k;

	double num;

	vector<double> v;
	vector<double> tmpv;

	cout<<"Give me some numbers\n";

	while(cin>>num)
	{
		v.push_back(num);
	}

	tmpv.resize(v.size());
	cout<<"now i start the sort\n";


	//call the recursive sorting function
	recursive_sort(v,tmpv, 0, v.size(), 1);

	return 0;
}

