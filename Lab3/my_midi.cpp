#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"
using namespace std;

//will convert event time to note time(counts to seconds: 480counts = 1 sec:w)
double cnvrt_elT_ndT(int totl)
{	
	double rv;

	if(totl > 0)
	{
		rv = double(totl/480);		//convert the time to seconds
	}
	else
	{
		rv = 0;
	}

	return rv;
}


//will convert seconds to counts (1sec = 480 counts)
int  cnvrt_ndT_elT(double time)
{	
	int rv, rnum;

	//round the number to an int
	rnum = rint(time);

	//convert time from seconds to counts
	//
	rv = rnum * 480;

	return rv;
}

//will create a note map from the el event list
void CS302_Midi::el_to_nd()
{
	nd = new NDMap;

	ND * ndp;

	int type,tm, pitch,ud,i;

	int counts = 0;

	double numh;

	vector <ND *> npvec;

	npvec.resize(128,NULL);			//createa a vector of size 128 full of NULL
	//create an iterator for the event list
	EventList::iterator e_it;

	//go through the the event list  grabing info and parsing accordingly
	for(e_it = (*el).begin(); e_it != (*el).end(); e_it++)
	{

		//grab the events type by looking into the pointer at this iterator position
		type = (*e_it)->key;

		//if the current event is a note event
		if(type == 'O' || type == 'F')
		{
			pitch = (*e_it)->v1;		//grab the pitch

			counts += (*e_it)->time;	//grab the time and add it to the running time

			//check for an on or off event
			if(type == 'O') //if its an on
			{
				npvec.at(pitch) = new ND;	//create a pointer to a new not object and store at the appropriate place in vector
				npvec.at(pitch)->key = 'N';			//set the notes key to N for note
				npvec.at(pitch)->pitch = pitch;
				npvec.at(pitch)->volume = (*e_it)->v2;	//grab the volume for this note
				npvec.at(pitch)->start = cnvrt_elT_ndT(counts); 

			}
			else	//its an off
			{
				npvec.at(pitch)->stop = cnvrt_elT_ndT(counts);
			}
		}
		else if(type == 'D')		//its a damper event
		{
			//grab the up/ down specifier
			ud = (*e_it)->v1;		//1 for down, 0 for up

			//if you have a damper down event
			if(ud)
			{
				npvec.push_back(new ND);

				npvec.at(npvec.size() - 1)->key = 'D';
				npvec.at(npvec.size() - 1)->start = cnvrt_elT_ndT(counts);

			}
			else	//it is a up damper event	
			{
				npvec.at(npvec.size() - 1)->stop = cnvrt_elT_ndT(counts);
			}
		}

	}

	//now go through the npvec putting the pointers and the notes start times in the mulitmap 
	for( i = 1; i < npvec.size();i++)
	{


		if(npvec.at(i) != NULL)
		{
			numh = npvec.at(i)->start;
			ndp = npvec.at(i);

			nd->insert( make_pair(numh , ndp ) );
		}
	}
}


//will create an event list from a note map
void CS302_Midi::nd_to_el()
{
	el = new EventList;				//get a new eventlist pointer(list <Event *> EventList)

	double tsec, Lsec, rlsec;

	int last = 0;
	int relT = 0;					//will represent the time of the last event and the time relative to the last event
	int type = 0;
	int time = 0;

	Event * ep;
	NDMap::iterator nd_it;

	

	

	map <double, multimap<int, Event *> > nd_el_mp;
	map <double, multimap<int, Event *> >::iterator nemp_it;
	multimap <int, Event *>::iterator e_mmp;

	Lsec = 0;


	//go through ND map creating on/off or damper up/down events for each element of the map
	for(nd_it = (*nd).begin(); nd_it != (*nd).end();nd_it++)
	{
		//chek to see if it is a note or a damper pedal
		type = nd_it->second->key;

		//if it is a note make an on event and an off event for it
		if(type == 'N')
		{
			//----------create off event----------/////////
			ep = new Event;

			//get the time in counts
			time = cnvrt_ndT_elT( nd_it->second->stop);

			//calculate the time relative to last event
			relT = abs(last - time);

			//set this events info
			ep->key = 'F';
			ep->time = relT;
			ep->v1  = nd_it->second->pitch;
			ep->v2  = nd_it->second->volume;

			//add this to the storage map
			nd_el_mp[nd_it->second->stop].insert( make_pair(0,ep) );

			//////////----------------------create on event-----------//////////
			ep = new Event;
			
			//get the time in counts
			time = cnvrt_ndT_elT( nd_it->second->start);

			//calculate the time relative to last event
			relT = abs(last - time);

			//set this events info
			ep->key = 'O';
			ep->time = relT;
			ep->v1  = nd_it->second->pitch;
			ep->v2  = nd_it->second->volume;

			//add this to the storage map
			nd_el_mp[nd_it->second->start].insert( make_pair(3,ep) );

			last = time;
		}
		else if(type == 'D')			//if its a damper pedal
		{
			//------------create damper up event--------------------------/////////
			ep = new Event;
			//get the time in counts
			time = cnvrt_ndT_elT( nd_it->second->stop);

			//calculate the time relative to last event
			relT = abs(last - time);

			//set this events info
			ep->key = 'D';
			ep->time = relT;
			ep->v1  = 0;

			//add this to the storage map
			nd_el_mp[nd_it->second->stop].insert( make_pair(1,ep) );

			//------------create damper down event--------------------------/////////
			ep = new Event;
			//get the time in counts
			time = cnvrt_ndT_elT( nd_it->second->start);

			//calculate the time relative to last event
			relT = abs(last - time);

			//set this events info
			ep->key = 'D';
			ep->time = relT;
			ep->v1  = 1;

			//add this to the storage map
			nd_el_mp[nd_it->second->start].insert( make_pair(2,ep) );
			
			last = time;
		}
	}

	//now add the events to the event list from 
	for(nemp_it = nd_el_mp.begin(); nemp_it != nd_el_mp.end(); nemp_it++)
	{
		for(e_mmp = nemp_it->second.begin(); e_mmp != nemp_it->second.end(); e_mmp++)
		{
			ep = e_mmp->second;
			el->push_back(ep);		
		}
	}

}
