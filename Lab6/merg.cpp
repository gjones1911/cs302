
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "sorting.h"


using namespace std;

void recursive_sort( vector <double> &v,vector <double> &t,int start,int size,int print)
{
	int i,mid,szL,szR,L,R;
	
	if(size == 1)
	{
/*
		printf("E: %5d %5d   ",start,size);

		for(i = 0; i < v.size(); i++)
		{
			printf("%5.2lf",v[i]);
		}

		cout<<"\n";
*/
		return;
	}

	if(size > 1)
	{
		printf("B: %5d %5d   ",start ,size);

		for(i = 0; i < v.size(); i++)
		{
			printf("%5.2lf ",v[i]);
		}

		cout<<"\n";

	}

	

	mid = (size/2) + start;

	szL = mid - start;
	szR = size - szL;


	//sort the two halves
	recursive_sort(v,t,start,szL,1);
	recursive_sort(v,t,mid,szR,1);

	L = start;

	R = mid;

	i = start;

	//now merge the two sorted halves
	while(L < mid && R < start + size)
	{
		if(v[L] <= v[R])
		{
			t[i] = v[L];
			L++;
			i++;
		}
		else
		{
			t[i] = v[R];
			R++;
			i++;
		}

	}

	//now get any left over numbers
	if(L < mid)
	{
		while( L < mid)
		{
			t[i] = v[L];
			i++;
			L++;
		}
	}
	else if( R < start + size)
	{
		
		while(R < start + size)
		{
			t[i] = v[R];
			i++;
			R++;
		}
	}

	//now copy into place
	for(i = start; i < start+size ; i++)
	{
		v[i] = t[i];
	}
 
//	if(size > 1)
//	{
		printf("E: %5d %5d   ",start ,size);

		for(i = 0; i < v.size(); i++)
		{
			printf("%5.2lf ",v[i]);
		}

		cout<<"\n";

//	}


}
int main(int argc, char ** argv)
{
	int i , j, k, l;
	double num;

	vector <double> v;
	vector <double> t;

	cout<<"Give me some numbers to sort: \n";
	while(cin>>num)
	{
		v.push_back( num );

	}


	t.resize( v.size() );

	recursive_sort( v,t,0,v.size(),1);

	printf("%17s"," ");

	for(i = 0; i  < v.size();i++)
	{
		printf("%5.2lf ",v[i]);
	}

	cout<<"\n";

	return 0;
}
