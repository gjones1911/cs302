
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include "sorting.h"

using namespace std;

void sort_doubles(vector<double> &v, int print)
{
	recursive_sort(v,int start, int size,int print);
}


void recursive_sort(vector<double> &v, int start, int size, int print)
{

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

}

