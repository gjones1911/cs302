
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program:    merge_1_sort.cpp																   //
//Created By: Gerald Jones                                                                     //
//Purpose:	  This is part of Lab 6. It performs a merge sort.                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "sorting.h"


using namespace std;


//This performs a recursive merge sort
void recursive_sort( vector <double> &v,vector <double> &t,int start,int size,int print)
{
	int i,mid,szL,szR,L,R;

	if(size == 1)
	{
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

	printf("E: %5d %5d   ",start ,size);

	for(i = 0; i < v.size(); i++)
	{
		printf("%5.2lf ",v[i]);
	}

	cout<<"\n";

}


void sort_doubles( vector<double> &v, int print)
{
	int i;

	vector <double> t = v;

	recursive_sort(v,t,0,v.size(),print);

	printf("%17s"," ");

	for(i = 0; i  < v.size();i++)
	{
		printf("%5.2lf ",v[i]);
	}

	cout<<"\n";
}
