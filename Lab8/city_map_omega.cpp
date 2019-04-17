#include <map>
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
		ip->backedge = NULL;
		ip->best_time = -1;
	//	ip->bfsq_ptr = NULL;
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

	//parameter is either 'B' , 'W' and 'A' for best, worst, and average respectively

	Intersection * ip , *ip2, *iptmp;

	Road_Segment * rsp;

	multimap <double, Intersection *>::iterator mmit;

	list <Road_Segment*>::iterator lit;
	
	first->best_time = 0;

	double ndis, ntm;
	
	first->bfsq_ptr = bfsq.insert( make_pair( first->best_time, first) );

	while( !(bfsq.empty()) )
	{
		mmit = bfsq.begin();			//get iterator to the first element
		
		ip = mmit->second;				//get the intersection pointer here
		//pop first node off and add its kids
		bfsq.erase( bfsq.begin() );				//remove the first element
		ip->bfsq_ptr = bfsq.end();

		//go through the adjacencey list adding and adjusting the kids if need be
		for(lit = ip->adj.begin(); lit != ip->adj.end(); lit++)
		{
			ip2 =(*lit)->to;			//get pointer to destination intersection aka a kid node of ip

		//	ndis = ip->best_time + (*lit)->distance;					//get the distance here

			ntm = ( (*lit)->distance /(30) ) * 3600 + ip->best_time;	//get the time it takes to run this segment

			//if the worst case is needed  add the time of the red light to each
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
				if( (*lit)->type == STREET)			//if its a street add the time of the light for the ave
				{
					ntm += ( pow(ip2->green[1], 2) / (2 * (ip2->green[1] + ip2->green[0]) ) );
				}
				else
				{

					ntm += ( pow(ip2->green[0], 2) / (2 * (ip2->green[1] + ip2->green[0]) ) );
				}

			}


			//if this intersection hasnt been visited add it
			if( ip2->best_time == -1) 
			{
				ip2->best_time = ntm;
											//might need to create a new road segment pointer here
/*											//instead of the flip
				rsp = new Road_Segment;

				rsp->to = ip2;
				rsp->from = ip;
				rsp->distance = (*lit)->distance;;
				rsp->type = (*lit)->type;
				rsp->number = (*lit)->number;
*/


/*
				//flip the road segment
				iptmp = (*lit)->to;
				(*lit)->to = (*lit)->from;
				(*lit)->from = iptmp;
*/
				ip2->backedge = (*lit);
//				ip2->backedge = rsp;
				ip2->bfsq_ptr = bfsq.insert( make_pair( ntm, ip2) );
			}
			//otherwise check for it being in the map and if so 
			//if its current distance is greater than the new one replace it
			else if(ip2->bfsq_ptr->first > ntm) 
			{

				ip2->best_time = ntm;

				ip2->backedge = (*lit);
//				ip2->backedge->to = ip2;
//				ip2->backedge->from = ip;
				ip2->backedge->distance = (*lit)->distance;

/*				
				//flip the road segment
				iptmp = (*lit)->to;
				(*lit)->to = (*lit)->from;
				(*lit)->from = iptmp;
*/			


				bfsq.erase(ip2->bfsq_ptr);

				ip2->bfsq_ptr = bfsq.insert( make_pair(ntm,ip2) );

			}

		}


	}

	first->backedge = NULL;
	ip = last;

	//now push the shortest path  from the last node to the first onto path
	while(ip != first )
	{
//		cout<<"from "<<ip->street<<" "<<ip->avenue<<"\n";
		path.push_front(ip->backedge);
		rsp = ip->backedge;

		ip = ip->backedge->from;
		
//		cout<<"to "<<ip->street<<" "<<ip->avenue<<"\n";
	}
	


	return 0;
}
