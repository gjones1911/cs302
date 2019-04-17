
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "sorting.h"

using namespace std;



void recursive_sort(vector<double> &v, int start, int size, int print)
{
	
	if(size <= 1) return;
	
	int  i,j,k,mid, szL, szR, pvt_idx,L,R;

	//get pivot
	
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
	if( (Md >= Lf && Md <= Rt) || (Md <=Lf && Md >=Rt) )
	{
		pivot = Md;
		pvt_idx = mid;
		
		dhldr = v[start]; 
		v[start] = v[mid];
		v[mid] = dhldr;
		

	}

	else if( (Lf>= Md && Lf <= Rt) || (Lf <= Md && Lf >=Rt) )
	{
		pivot = Lf;
		pvt_idx = start;

	}
	else if(Rt >= Md && Rt <= Lf || (Rt <= Md && Rt >= Lf) )
	{
		pivot = Rt;
		pvt_idx = start+size-1;
		
		dhldr = v[start+size-1]; 
		v[start+size-1] = v[start];
		v[start] = dhldr;

	}
/*
	cout<<"The pivot is:"<<pivot<<"\n";
	cout<<"The pivot idx was:"<<pvt_idx<<"\n";
*/
	printf("M: %5d %5d %5.2lf",start,size,pivot);


	for(i = 0; i < v.size();i++)
	{
		printf("%5.2lf ",v[i] );

	}

	cout<<"\n";


	L= start+1;
	R=start + size -1;

	while(L < R &&  (L < start + size) && (R > start)  )
	{

		while(v[L] < pivot)
		{
			L++;
		}
		
		while(v[R] > pivot)
		{
			R--;
		}

		if(L<R)
		{
			dhldr = v[R];
			v[R] = v[L];
			v[L] = dhldr;
		}
	}
	
	//partition around the pivot

	//put pivot in l-1
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
	szR = size - L ;

	recursive_sort(v,start,szL,1);
	recursive_sort(v,L,szR,1);

	return;
	

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

int main (int argc, char** argv)
{
	int i,j,k,l;

	double num;

	vector<double> v;

	//take in cmd line args 
	for(i = 1; i < argc; i++)
	{
		sscanf(argv[i],"%lf",&num);

		v.push_back(num);
	}

	cout<<"You gave me:\n";

	for(i = 0; i < v.size();i++)
	{
		printf("%.2lf ",v.at(i));
	}

	cout<<"\n";

	sort_doubles(v,1);

//	recursive_sort(v,0,v.size(),1);

	return 0;


}

