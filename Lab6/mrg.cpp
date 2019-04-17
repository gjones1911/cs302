
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <cmath>

using namespace std;

void merge(vector<double> &v1, vector<double> &v2,int start, int mid,int size,vector <double>t)
{
	
}

void recursive_sort(vector<double> &v,vector<double> &tmpv,int start, int size, int print)
{

	int i, j, k, l, min;

	vector<double> v1;
	vector<double> v2;
	vector<double> vt1;
	vector<double> vt2;
	vector<double> TMMP;

	//get mid
	int mid = (size)/2 + start;

	int asize = mid - start;
	int bsize = (size - asize);

	int cnt1 =0;
	int cnt2 =0;

	cout<<"asize "<<asize<<"\n";
	cout<<"bsize "<<bsize<<"\n";



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
		
		cout<<"\n\n";

//		tmpv.push_back(v[0]);
		printf("E: %5d %5d \n",start,size);
		return;
	}

	//get the first half
	for(i = 0; i < mid; i++)
	{
		cnt++;
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
cout<<"\ngoing int recursion with 1st and second half\n";
	
	vt1.resize(v1.size());
	vt2.resize(v2.size());

	//these will fill vt with a element
	recursive_sort(v, tmpv, start, asize, 1);
	recursive_sort(v, tmpv, mid, bsize, 1);

	cout<<"\nRight after recursion-------v1 is now:\n";
	
	for(i = 0; i < v1.size();i++)
	{
		printf(" %5.2lf ",v1[i]);
	}
	
	cout<<"\nRight after recursion--------v2 is now:\n";
	
	for(i = 0; i < v2.size();i++)
	{
		printf(" %5.2lf ",v2[i]);
	}
	cout<<"\n";

/*
	//now i must start the merging
	
	cout<<"Vt1 is DaAAMN: \n";

	for(i = 0; i < vt1.size();i++)
	{
		cout<<" "<<vt1[i]<<"\n";

	}

	cout<<"Vt2 is : \n";
	for(i = 0; i < vt2.size();i++)
	{

		cout<<" "<<vt2[i]<<"\n";
	}
	
*/	int smlr;

	i=j=k=0;

	i=0;

	j = start;
	k = mid;
cout<<"i is: "<<i<<"\n";
	//now sort v1 and v2 and store in the temp vector
	while( j <  start +asize && k < bsize + mid  )
	{
		cout<<"comparing "<<v[j]<<" "<<v[k]<<"\n";

		if(v[j] < v[k] )
		{
			tmpv[i] = v[j];
			printf("1 temp just got %5.2lf \n",v[j] );
			cout<<"j: "<<j<<"\n";											
			cout<<"i: "<<k<<"\n";											
			j++;
	i++	;

		}
		else
		{
			tmpv[i] = v[k];
			printf("2 temp just got %5.2lf \n",v[k]);
			cout<<"k: "<<k<<"\n";											
			cout<<"i: "<<i<<"\n";											
			k++;
	i++;	

		}

	}


	//now get the left over from comparision
	if(j < asize +start 	)	//if you did not use all of the v1's
	{
		for(i ; i < size +start ;i++ ) 
		{
			tmpv[i] = (v[j]);
			printf("Atemp just got %5.2lf \n",v[j]);
			cout<<"v1[i] "<<v1[i]<<"\n";
			cout<<"i "<<i<<"\n";
			cout<<"j "<<j<<"\n";
			j++;
		}

	}
	else if(k < bsize +mid	)	//if you didn't use all of the v2's
	{
		for(i ; i < size+start ;i++ ) 
		{
			tmpv[i] =(v[k]);
			printf("Btemp just got %5.2lf \n",v[k]);
			cout<<"v2[i] "<<v2[i]<<"\n";
			cout<<"i "<<i<<"\n";
			cout<<"k "<<k<<"\n";
			k++;
		}

	}


	cout<<"the temp is: ------\n";
	for(i=0; i < tmpv.size();i++)
	{
		printf(" %5.2lf ",i, tmpv[i]);
	}

cout<<"\n\n";

cout<<"above the copy\n";
cout<<"start: "<<start<<"\n";
cout<<"size "<<size<<"\n";
cout<<"tmp.size "<<tmpv.size()<<"\n";
cout<<"v.size "<<v.size()<<"\n";

	i = 0;
	//next copy tmp back into original
	for(i = start ; i <start+ size;i++)
	{
		v[i] = tmpv[i];

		cout<<"v[i] = "<<tmpv[i]<<"\n";
		cout<<"i "<<i<<"\n";
	}
cout<<"now v is:\n";


	for(i = 0 ; i < v.size();i++)
	{
		cout<<" v["<<i<<"] "<<v[i];		
	}

	cout<<"\n";
	return;
}


int main(void)
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

	tmpv = v;
	cout<<"now i start the sort\n";


	//call the recursive sorting function
	recursive_sort(v,tmpv, 0, v.size(), 1);

	cout<<"V is----------------\n\n"; 
	
	
	for(i=0; i < v.size();i++)
	{
		cout<<" "<<v[i]<<"\n";
	}
		

	return 0;
}

