#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include "city_map.h"

using namespace std;

//take in the city map on std input in the form:
//
City_Map::City_Map()
{
	int i, j, k, street, ave, num_st, num_av;

	double x,y, GTst, GTav, difx, dify;

	Intersection * ip, *ipu, *ipd, *ipl, *ipr;

	Road_Segment * rsp;

	vector < vector<Intersection *> > cty;

	//take in the intersections on std input 
	while(cin>>street>>ave>>x>>y>>GTst>>GTav)
	{
		ip = new Intersection;
		
		if(cty.size() < street + 1) cty.resize(street+1); 
		
		if(cty.at(street).size() < ave + 1) cty.at(street).resize(ave+1); 

		ip->street = street;		
		ip->avenue = ave;
		ip->x = x;
		ip->y = y;
		ip->green[0] = GTst;
		ip->green[1] = GTav;
		
		cty.at(street).at(ave) = ip;

		all.push_back(ip);

	}
	
	//add the first and last pointers
	num_st = cty.size();
	num_av = cty.at(0).size();

	first = cty.at(0).at(0);
	last  = cty.at(num_st-1).at(num_av-1);

	//now set up the adjacency list
	for(i = 0 ; i < cty.size();i++)
	{
		for(j = 0; j < cty.at(i).size();j++)
		{
			ip = cty.at(i).at(j);

			//if the street is a multiple of 5 it goes both ways
			if( ip->street % 5 == 0 )
			{

				//set link left 
				if(j - 1 >= 0)
				{
					ipl = cty.at(i).at(j-1);

					difx = pow( ip->x - ipl->x,2);
					dify = pow( ip->y - ipl->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;				//also i
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipl;
					ip->adj.push_back( rsp );
				}
				//set link right
				if(j + 1 < cty.at(i).size())
				{
					ipr = cty.at(i).at(j+1);

					difx = pow( ip->x - ipr->x,2);
					dify = pow( ip->y - ipr->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;				//also j
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipr;
					ip->adj.push_back( rsp );
				}


				//the avenue is a multiple of 5 or it's the heighest it goes both ways
				if(ip->avenue % 5 == 0 || ip->avenue == num_av-1)
				{
					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				//the avenue is even the street goes both ways and the ave goes down
				else if( ip->avenue % 2 == 0)
				{

					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				//the avenue is odd the street goes both ways and the ave goes up
				else
				{

					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
				}
			}
			//if the street is an even street it goes to the right
			else if(ip->street % 2 == 0)
			{
				//set link right
				if(j + 1 < cty.at(i).size())
				{
					ipr = cty.at(i).at(j+1);

					difx = pow( ip->x - ipr->x,2);
					dify = pow( ip->y - ipr->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->avenue;				//also j
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipr;
					ip->adj.push_back( rsp );
				}

				//check for the last ave or a multiple of 5 if so it goes both ways
				if(ip->avenue == num_av-1 || ip->avenue % 5 == 0)
				{
					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				//check for even avenue
				else if(ip->avenue % 2 == 0)
				{

					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				//otherwise its and odd avenue
				else
				{

					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
				}

			}
			//otherwise it is an odd street so it goes to the left
			else
			{

				//set link left 
				if(j - 1 >= 0)
				{
					ipl = cty.at(i).at(j-1);

					difx = pow( ip->x - ipl->x,2);
					dify = pow( ip->y - ipl->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;				//also i
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipl;
					ip->adj.push_back( rsp );
				}
				
				//check for the last ave or a multiple of 5 if so it goes both ways
				if(ip->avenue == num_av-1 || ip->avenue % 5 == 0)
				{

					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				//check for even avenue
				else if(ip->avenue % 2 == 0)
				{
					//set link down
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				//otherwise its an odd avenue
				else
				{
					//set link up
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				//also j
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
				}
			}
		}
	}
}


double City_Map::Dijkstra(int avg_best_worst)
{

	return 0;
}
