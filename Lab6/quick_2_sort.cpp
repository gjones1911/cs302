
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program:	  quick_2_sort.cpp				      											   //
//Created By: Gerald Jones                                                                     //
//Purpose:	  This program is part of Lab 6 and performs a quick sort algorithm.               //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "sorting.h"

using namespace std;


//will recursively perform a quick sort 
void recursive_sort(vector<double> &v, int start, int size, int print)
{
	
	if(size <= 1) return;
	
	int  i,j,k,mid, szL, szR, pvt_idx,L,R;
	
	mid = size/2 + start;
	
	double Lf, Rt, Md, pivot,dhldr;


	//will use the below to find the pivot
	Lf =v[start] ;
	Md =v[mid] ;
	Rt =v[start + size -1] ;

	if(size > 1)
	{
		printf("S: %5d %5d      ",start,size);

		for(i = 0; i < v.size();i++)
		{
			printf("%5.2lf ",v[i] );

		}

		cout<<"\n";

	}
	if(size == 2)
	{
		//sort by hand
		if(v[start] > v[start+1])
		{
			dhldr = v[start];
			v[start] = v[start+1];
			v[start+1] = dhldr;
		}

		return;
	}

		//get the pivot by finding the median
		//making sure if there are repeats to take the one farthest to the right
		if( (Md >= Lf && Md < Rt) || (Md <= Lf && Md > Rt)  )
		{
			pivot = Md;
			pvt_idx = mid;

			dhldr = v[start]; 
			v[start] = v[mid];
			v[mid] = dhldr;

		}

		else if( (Lf> Md && Lf < Rt) || (Lf < Md && Lf > Rt) )
		{
			pivot = Lf;
			pvt_idx = start;

		}
		else if(Rt >=  Md && Rt <=  Lf || (Rt <=  Md && Rt >= Lf) )
		{
			pivot = Rt;
			pvt_idx = start+size-1;

			dhldr = v[start+size-1]; 
			v[start+size-1] = v[start];
			v[start] = dhldr;

		}



	printf("M: %5d %5d %5.2lf",start,size,pivot);

	for(i = 0; i < v.size();i++)
	{
		printf("%5.2lf ",v[i] );

	}

	cout<<"\n";


	//get the Left and right starting points
	L= start+1;
	R=start + size -1;

	//move through the vector putting things greater than the pivot on the right
	//and those less than it on the left 
	while(L < R )
	{
		while(v[L] < pivot && L < start+size)
		{
			L++;
;		}
		
		while(v[R] > pivot && R > start)
		{
			R--;
		}

		if(L<R)
		{
			dhldr = v[R];
			v[R] = v[L];
			v[L] = dhldr;
			R--;
			L++;
		}
	}


	//if this is so i need  to put the pivot at L because it is 
	//the last lower than element
	if(L == R && v[L] <= pivot)
	{
		dhldr = v[L];
		v[L] = v[start];
		v[start] = dhldr;

		printf("P: %5d %5d %5d",start,size,L);

		for(i = 0; i < v.size();i++)
		{
			printf("%5.2lf ",v[i] );

		}

		cout<<"\n";

		szL = L - start;
		szR = size - szL-1;

		//and call the function on whats on either side of the pivot
		recursive_sort(v,start,szL,1);
		recursive_sort(v,L+1,szR,1);

	}
	else		//otherwise put the pivot at L-1
	{

		dhldr = v[L-1];
		v[L-1] = v[start];
		v[start] = dhldr;

		printf("P: %5d %5d %5d",start,size,L-1);

		for(i = 0; i < v.size();i++)
		{
			printf("%5.2lf ",v[i] );

		}

		cout<<"\n";

		szL = L-1 -start;
		szR = size - szL -1;

		//and call the function on whats on either side of the pivot
		recursive_sort(v,start,szL,1);
		recursive_sort(v,L,szR,1);
	}



}

void sort_doubles(vector<double> &v, int print)
{
	int i,start,size;

	recursive_sort(v,0, v.size() ,print);

	printf("%20s"," ");

	for(i = 0; i < v.size();i++)
	{
		printf("%5.2lf ",v[i] );

	}

	cout<<"\n";


}

