
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: BuildingAdvertise.cpp															   //
//Created By: Gerald Jones                                                                     //
//Purpose: This is part 2 of Lab 2 for CS 302. It is a problem from Top Coder. Idefines a class//
//		   that will use a given vector representing heights of buildings in a skyline to      //
//		   determine the maxiumum area to be used to place an advertisment. Used in            //
//		   conjunction with a driver program called SRM-337-Driver.cpp                         //
//																							   //	
//		   Note: I am getting a bug for test case 53. The correct solution is: 104858900000    //
//		                                              My solution is:          104857851411    //
//		                                              and I have no idear why.				   //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <set>
#include <map>
#include <vector>
#include <cstdlib>
#include <algorithm>


using namespace std;

//The class that holds the method used to find the maximum area to advertise
class BuildingAdvertise
{
	public:
	long long getMaxArea(vector <int> &h, int &size);

};

//will calculate the maximum area in which to advertise on a given skyline represented by the vector h
long long BuildingAdvertise::getMaxArea(vector <int> &h, int &size)
{
	long long i,s,I;

	long long H;
	long long rsize = size + 2;
	vector <long long> R;

	//Resize the R vector accordingly and add sentinel 0's to the begining and end
	R.resize(rsize,0);

	long long rv = 0;
	long long  j = 0;
	long long  m = h.size();

	long long r,l, area;
	
	multimap <long long, long long> XM;
	multimap <long long ,long long >::iterator mit;

	set <long long> X;
	set<long long>::iterator x_sit;

	//set up R array
	for(i = 0; i < size ; i++  )
	{
		R[i] = h[j];
		s = (j+1) % m;
		h[j] = ( (h[j] ^ h[s]) + 13 ) % 835454957;

		j = s;
	}

	//make the multi map that has heights as keys and indices as values
	for(i = 0; i < rsize; i++)
	{
		XM.insert( make_pair( R[i],i ) );
	}

	///////////////////////////////////////////////////////////////////////////////////////////
	//The below code segment will iterate through the multimap and calculate the maximum area//
	///////////////////////////////////////////////////////////////////////////////////////////
	for(mit = XM.begin(); mit != XM.end(); mit++)
	{
		H = mit->first;		//get the height
		I = mit->second;	//get the index
		//if the H is 0 or a sentinel insert I into the set so upperbound will not return end()
		if(H == 0)
		{
			X.insert(I);
		}
		else
		{
			x_sit = X.upper_bound(I);		//find the smallest value greater than I in the set

			r = (*x_sit);
			x_sit--;						//go to the element smaller than r
			l = (*x_sit);

			area = H * (r - l - 1);			//get current area

			if(area > rv) rv = area;

			X.insert(I);
		}
	}

	return rv;

}


