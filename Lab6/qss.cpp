
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
	
// cout<<"the start is:"<<start<<"\n";
	int  i,j,k,mid, szL, szR, pvt_idx,L,R;
j=1;
	//get pivot
	
	mid = size/2 + start;
	
	double Lf, Rt, Md, pivot,dhldr;


	//will use the below to find the pivot
	Lf =v[start] ;
	Md =v[mid] ;
	Rt =v[start + size -1] ;

//cout <<"the Lf is: "<<Lf<<"\n";;
//cout <<"the Rt is: "<<Rt<<"\n";;
//cout <<"the Md is: "<<Lf<<"\n";;
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

//	if(Lf == Rt && Md == Lf)
//	{
//		pivot = Md;
//		pvt_idx = mid;
//cout <<"the pivot = mid = lf = rt \n";
//cout <<"the mid is "<<mid<<"\n";
//	}
	if(j==1) //else
	{

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

	while(L < R )
	{
/*
		if(L == R && v[L] == pivot && v[R] == pivot)
		{
			L++;;

		}
*/
//		cout<<"L is:"<<L<<"\n";
		while(v[L] < pivot && L < start+size)
		{
			L++;
;		}
		
//		cout<<"L is now:"<<L<<"\n";
//		cout<<"R is:"<<R<<"\n";
		while(v[R] > pivot && R > start)
		{
			R--;
		}

//		cout<<"R is now:"<<R<<"\n";
		if(L<R)
		{
//			cout<<"a switch happened\n";
			dhldr = v[R];
			v[R] = v[L];
			v[L] = dhldr;
			R--;
			L++;
		}
	}


	int nwL;

	if(L == R)
	{
		
		cout<<"\nL is: "<<L<<"\n";
		cout<<"\nR is: "<<R<<"\n";
		//put pivot in l-1
		
		//look at where it stopped and see if it is greater than or less than the pivot
		//if less than pivot is the last lower element so put pivot here and partition
		if(v[L] < pivot )
		{
			
            nwL = L;
			dhldr = v[L];
			v[L] = v[start];
			v[start] = dhldr;

			printf("P: %5d %5d %5d",start,size,L);


			for(i = 0; i < v.size();i++)
			{
				printf("%5.2lf ",v[i] );

			}
cout<<"\n.\n";
			szL = L -start;
			szR = size - L+1;
		cout<<"the size is : "<<size<<"\n";	
			
		recursive_sort(v,start,szL,1);
		recursive_sort(v,size+1,szR,1);
		}
		else    //otherwise it is greater than or equal to the pivot so put pivot behind it
		{

			
			dhldr = v[L-1];
			v[L-1] = v[start];
			v[start] = dhldr;

			printf("Pl: %5d %5d %5d",start,size,L-1);


			for(i = 0; i < v.size();i++)
			{
				printf("%5.2lf ",v[i] );

			}

			cout<<"\n";

			szL = L-1 -start;
			szR = size - L;
			//cout<<"the start is:"<<start<<"\n";
			cout<<"\nthe L is:"<<L<<"\n";
			recursive_sort(v,start,szL,1);
			cout<<"after the first recurs\n";
			cout<<"\nthe L is:"<<L<<"\n";
			cout<<"\nIt is:"<<L-start+L<<"\n";
			cout<<"\nMid is:"<<mid<<"\n";
			recursive_sort(v,size+1,szR,1);

		}
	}
	else
	{



		//	if(L >= start + size) L--;
		//partition around the pivot

		//put pivot in l-1
		dhldr = v[L-1];
		v[L-1] = v[start];
		v[start] = dhldr;

		printf("PL: %5d %5d %5d",start,size,L-1);


		for(i = 0; i < v.size();i++)
		{
			printf("%5.2lf ",v[i] );

		}

		cout<<"\n";
		szL = L-1 -start;
		szR = size - L+1 ;
		//cout<<"the start is:"<<start<<"\n";
		//cout<<"the L is:"<<L<<"\n";
		recursive_sort(v,start,szL,1);
	cout<<"The size is b: "<<size<<"\n";	
			cout<<"after the first recursL\n";
		recursive_sort(v,size-L,szR,1);
	}
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

