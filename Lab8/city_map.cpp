/*Program:       city_map.cpp
 * created by:   Gerald Jones
 * Purpose:      This is Lab 8 for cd 302. It implements the constructor for a graph representation of a city street
 *				 grid. It sets up the intersections and road segments given on std input in the format specified in 
 *				 the lab write up. It also implents the Dijkstra algorithm to find the fastest path from the start or 
 *				 (0,0) / lowest farthest to the right intersection to the heighest farthest to the left intersection 
 *				 in the cases when all lights are green(Best case), all lights are Red(worst), and a case where you get
 *				 an average of both red and green lights. These methods work in conjuntion with city_map.h provided by
 *				 Dr. plank
*/




#include <map>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstdio>
#include <cmath>
#include "city_map.h"

using namespace std;


//The below is the constructor for the City Map class it creates instances for all the needed
//intersections and road segments
City_Map::City_Map()
{
	int i, j, k, street, ave, num_st, num_av;

	double x,y, GTst, GTav, difx, dify;

	Intersection * ip, *ipu, *ipd, *ipl, *ipr;

	Road_Segment * rsp;

	vector < vector<Intersection *> > cty;

	//The below loop sets up the intersections that represent the city streets in the city map
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
		ip->backedge = NULL;
		ip->best_time = -1;
		cty.at(street).at(ave) = ip;

		all.push_back(ip);

	}
	
	num_st = cty.size();
	num_av = cty.at(0).size();

	first = cty.at(0).at(0);
	last  = cty.at(num_st-1).at(num_av-1);

	//now set up the adjacency list or road segments for each intersection
	//even streets go left to right
	//odd steets go right to left
	//even avenues go from the top down
	//odd avenues go from the bottom up
	//any street or avenue that is a multiple of 5 as well as the highest ave goes both ways
	for(i = 0 ; i < cty.size();i++)
	{
		for(j = 0; j < cty.at(i).size();j++)
		{
			ip = cty.at(i).at(j);

			if( ip->street % 5 == 0 )
			{

				if(j - 1 >= 0)
				{
					ipl = cty.at(i).at(j-1);

					difx = pow( ip->x - ipl->x,2);
					dify = pow( ip->y - ipl->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;			
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipl;
					ip->adj.push_back( rsp );
				}
				if(j + 1 < cty.at(i).size())
				{
					ipr = cty.at(i).at(j+1);

					difx = pow( ip->x - ipr->x,2);
					dify = pow( ip->y - ipr->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;		
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipr;
					ip->adj.push_back( rsp );
				}


				if(ip->avenue % 5 == 0 || ip->avenue == num_av-1)
				{
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				else if( ip->avenue % 2 == 0)
				{

					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				else
				{
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
				}
			}
			else if(ip->street % 2 == 0)
			{
				if(j + 1 < cty.at(i).size())
				{
					ipr = cty.at(i).at(j+1);

					difx = pow( ip->x - ipr->x,2);
					dify = pow( ip->y - ipr->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->avenue;			
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipr;
					ip->adj.push_back( rsp );
				}

				if(ip->avenue == num_av-1 || ip->avenue % 5 == 0)
				{
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}

				}
				else if(ip->avenue % 2 == 0)
				{

					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				else
				{

					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
				}

			}
			else
			{

				if(j - 1 >= 0)
				{
					ipl = cty.at(i).at(j-1);

					difx = pow( ip->x - ipl->x,2);
					dify = pow( ip->y - ipl->y,2);

					rsp = new Road_Segment;
					rsp->type = STREET;
					rsp->number = ip->street;			
					rsp->distance = sqrt( difx + dify );
					rsp->from = ip;
					rsp->to   = ipl;
					ip->adj.push_back( rsp );
				}
				
				if(ip->avenue == num_av-1 || ip->avenue % 5 == 0)
				{

					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;				
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipu;
						ip->adj.push_back( rsp );
					}
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				else if(ip->avenue % 2 == 0)
				{
					if(i + 1 < cty.size())
					{
						ipd = cty.at(i+1).at(j);

						difx = pow( ip->x - ipd->x,2);
						dify = pow( ip->y - ipd->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;	
						rsp->distance = sqrt( difx + dify );
						rsp->from = ip;
						rsp->to   = ipd;
						ip->adj.push_back( rsp );
					}
				}
				else
				{
					if(i - 1 >= 0)
					{
						ipu = cty.at(i-1).at(j);

						difx = pow( ip->x - ipu->x,2);
						dify = pow( ip->y - ipu->y,2);

						rsp = new Road_Segment;
						rsp->type = AVENUE;
						rsp->number = ip->avenue;			
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


//This method uses the dijkstra algorithm to find the fastest time in one of the three cases:
//if avg_best_worst is: 'B' it will calculate the best time with all green lights in your favor
//					    'W' it will calculate the worst time with all red lights
//					    'A' it will calculate the time when you get a average of both red and green lights
double City_Map::Dijkstra(int avg_best_worst)
{

	Intersection * ip , *ip2, *iptmp;

	Road_Segment * rsp;

	multimap <double, Intersection *>::iterator mmit;

	list <Road_Segment*>::iterator lit;
	
	first->best_time = 0;

	double ndis, ntm;
	
	first->bfsq_ptr = bfsq.insert( make_pair( first->best_time, first) );

	while( !(bfsq.empty()) )
	{
		mmit = bfsq.begin();			
		
		ip = mmit->second;				
		
		//pop first node off and add its kids
		bfsq.erase( bfsq.begin() );			
		ip->bfsq_ptr = bfsq.end();

		//go through adding and adjusting the best time of adjacent nodes
		for(lit = ip->adj.begin(); lit != ip->adj.end(); lit++)
		{
			ip2 =(*lit)->to;		


			ntm = ( (*lit)->distance /(30) ) * 3600 + ip->best_time;	//get the time it takes to run this segment

			if(avg_best_worst == 'W')
			{
				//need to decide if i am taking a street or avenue
				if( (*lit)->type == STREET)			//if its a street add the time of the light for the ave
				{

					ntm += ip2->green[1];
				}
				else								//if its an ave add the time for the street
				{
					ntm += ip2->green[0];
				}
			}
			if(avg_best_worst == 'A')				
			{
				if( (*lit)->type == STREET)			
				{
					ntm += ( pow(ip2->green[1], 2) / (2 * (ip2->green[1] + ip2->green[0]) ) );
				}
				else
				{

					ntm += ( pow(ip2->green[0], 2) / (2 * (ip2->green[1] + ip2->green[0]) ) );
				}

			}


			//if this intersection hasnt been visited add it to the map
			if( ip2->best_time == -1) 
			{
				ip2->best_time = ntm;

				ip2->backedge = (*lit);
				
				ip2->bfsq_ptr = bfsq.insert( make_pair( ntm, ip2) );
			}
			//if its current distance is greater than the new one replace it
			else if(ip2->bfsq_ptr->first > ntm) 
			{
				ip2->best_time = ntm;

				ip2->backedge = (*lit);

				//if its in the map already remove it
				bfsq.erase(ip2->bfsq_ptr);

				ip2->bfsq_ptr = bfsq.insert( make_pair(ntm,ip2) );
			}

		}


	}

	ip = last;

	//now push the shortest path  from the last node to the first onto path
	while(ip != first )
	{
		path.push_front(ip->backedge);

		rsp = ip->backedge;

		ip = ip->backedge->from;
	}
	


	return 0;
}
