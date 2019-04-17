/*Program: cs302-midi.cpp
 *Created by: Gerald Jones
 *Purpose: This is Lab 3 for cs 302. The below code implements two protectd methods of the CS302_Midi class provided by the CS
		   department. 
		   1) The method nd_to_el(), which converts the ND's(note objects) stored in the NDMap pointed to by the protected 
			  variable nd into Event objects and stores them in the EventList pointed to by the protected variable el.
		   2) The method el_to_nd(), which converts the Event objects stored in the EventList pointed to by the protected 
			  variable el into ND's and stores them in the NDMap pointed to by the protected variable ndi.

		   The class definitions for Event's, ND's, and CS302_Midi, NDMap, and EventList can be found in  cs302-midi.h
		   The implemntations of the methods in the  cs302-midi.h class can be found in  cs302-midi-base.cpp.
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//											My Functions                                                                         //
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//will convert event time to note time(counts to seconds: 480counts = 1 sec:)
//takes the time in an int representing time in counts and returns the time in seconds as a double
double cnvrt_elT_ndT(int &totl)
{	
	double rv;

	if(totl > 0)
	{
		rv = double(totl) / 480;		//convert the time to seconds
	}
	else
	{
		rv = 0;
	}

	return rv;
}


//will convert seconds to counts (1sec = 480 counts)
//takes a double representing time in seconds and returns an int representing time in counts
int  cnvrt_ndT_elT(double &time)
{	
	int rv;
	
	float ans;
	//convert time from seconds to counts
	ans = (time * 480);

	//round the anwer to an integer
	rv = rint(ans);

	return rv;
}

//will get the time relative to the event before
//takes a reference to a event storage map and a time in counts and calculates the difference in time between the time the given event cccurs
//and the previous event
int Get_Rel_T( map <int, multimap <int, Event *> > &nd_map, int &tcount)
{
	 int last, ans;
		
	 map <int, multimap <int, Event *> >::iterator  lb_it;

	 lb_it = nd_map.find(tcount);				// i Know its in here so give me a iterator to it
	 
	 //if it is the first return its own time it is the first
	 if(lb_it == nd_map.begin())
	 {
		 return tcount;
	 }

	 //if its not the first one, look behind it for time of event before;
	 lb_it--;						//grab iterator to whats behind it

	 last = lb_it->first;

	 ans = tcount - last;		//get the time since last event

	 return ans;				//return the time relative to last event
}

////////////////////////////-----------------Lab 3 methods------------------------//////////////////////////////////
//will create a note map from the el event list
void CS302_Midi::el_to_nd()
{
	nd = new NDMap;

	ND * ndp;

	int type,tm, pitch,ud,i,idx;
	
	bool dmp_dwn = false;
	

	int counts = 0;
	double numh;

	vector <ND *> npvec;		

	npvec.resize(128,NULL);			
	
	EventList::iterator e_it;

	//go through the the event list  grabing info and parsing accordingly
	for(e_it = (*el).begin(); e_it != (*el).end(); e_it++)
	{
		//grab the events type and add its time to the running count
		type = (*e_it)->key;
		tm = (*e_it)->time;
		counts += tm;


		//if the current event is a note event
		if(type == 'O' || type == 'F')
		{
			pitch = (*e_it)->v1;		//grab the pitch


			//check for an on or off event
			if(type == 'O')
			{
				npvec.at(pitch) = new ND;	//create a pointer to a new note(ND) object and store in the vector at its pitch

				//set the notes information and on time
				npvec.at(pitch)->key = 'N';		
				npvec.at(pitch)->pitch = pitch;
				npvec.at(pitch)->volume = (*e_it)->v2;
				npvec.at(pitch)->start = cnvrt_elT_ndT(counts); 

				//for error checking will set a bool that if still set at the end of loading
				//will throw an error,
				

			}
			else	//its the next off event off set its off time and store it in the NDMap
			{
				if(npvec.at(pitch) == NULL)
				{
					cerr<<"there is a missing on pitch for: "<<pitch<<"\n";
					exit(1);

				}
				npvec.at(pitch)->stop = cnvrt_elT_ndT(counts);
				numh = npvec.at(pitch)->start;
				ndp = npvec.at(pitch);

				nd->insert( make_pair(numh, ndp) );

				npvec.at(pitch) = NULL;

			}
		}
		else if(type == 'D')		//its a damper event
		{
			//grab the up/ down specifier
			ud = (*e_it)->v1;		//1 for down, 0 for up

			//if you have a damper down event
			if(ud)
			{
				//create a new ND and set its up info
				npvec.push_back(new ND);
				idx = npvec.size() - 1;					//grab its position
				npvec.at( idx )->key = 'D';
				npvec.at( idx )->start = cnvrt_elT_ndT(counts);
				dmp_dn = true;
			}
			else	//it is a up damper event add the down info and store	
			{
				if(dmp_dn != true)
				{
					cerr<<"a damper up event is occuring before a damper down\n";
					exit(1);
				}

				npvec.at( idx )->stop = cnvrt_elT_ndT(counts);

				numh = npvec.at(idx)->start;
				ndp = npvec.at(idx);
				nd->insert( make_pair( numh, ndp) );
			}
		}

	}


}


//will create an event list from a note map
void CS302_Midi::nd_to_el()
{
	el = new EventList;				//get a new eventlist pointer(list <Event *> EventList)

	double tsec, Lsec, rlsec;
	int  thldr;
	double last = 0;
	int  relT = 0;					//will represent the time of the last event and the time relative to the last event
	int type = 0;
	int time = 0;
	int tcount;
	Event * ep;
	NDMap::iterator nd_it;





	map <int, multimap<int, Event *> > nd_el_mp;
	map <int, multimap<int, Event *> >::iterator nemp_it;
	map <double, multimap<int, Event *> >::reverse_iterator rnemp_it;
	multimap <int, Event *>::iterator e_mmp;

	Lsec = 0;


	//go through ND map creating on/off or damper up/down events for each element of the map
	//leaving the time adjust meant to after the map is filled 
	for(nd_it = (*nd).begin(); nd_it != (*nd).end();nd_it++)
	{
		//check to see if it is a note or a damper pedal
		type = nd_it->second->key;

		/*Event codes:
		  0)off
		  1)damper up
		  2)damper down
		  3)on	
		  */
		//if it is a note make an on event and an off event for it
		if(type == 'N')
		{

			////////////////////////////////----------------------create on event-----------/////////////////////////////
			ep = new Event;

			tsec = nd_it->second->start;

			tcount = cnvrt_ndT_elT(tsec);
			//set this events info
			ep->key = 'O';
			ep->v1  = nd_it->second->pitch;
			ep->v2  = nd_it->second->volume;

			//add this to the storage map
			nd_el_mp[tcount].insert( make_pair(3,ep) );		//store it with its time and coded as a 3)event

			/////////////////////////////////////////////----------create off event----------/////////////////////////////////
			ep = new Event;

			tsec = nd_it->second->stop;						
			tcount = cnvrt_ndT_elT(tsec);
			//set this events info
			ep->key = 'F';
			ep->v1  = nd_it->second->pitch;

			//add this to the storage map
			nd_el_mp[tcount].insert( make_pair(0,ep) );		//store it with its time and codes as a 0)event

		}
		else if(type == 'D')			//if its a damper pedal
		{

			/////////////////////////////////////------------create damper down event--------------------------////////////////////////////
			ep = new Event;
			tsec = nd_it->second->start;				
			tcount = cnvrt_ndT_elT(tsec);
			//set this events info
			ep->key = 'D';
			ep->v1  = 1;

			//add this to the storage map
			nd_el_mp[ tcount ].insert( make_pair(2,ep) );	//add it with its time, coded as a 2)event

			////////////////////////////////////------------create damper up event--------------------------/////////////////////////////////
			ep = new Event;
			tsec = nd_it->second->stop;				
			tcount = cnvrt_ndT_elT(tsec);
			//set this events info
			ep->key = 'D';
			ep->v1  = 0;

			//add this to the storage map
			nd_el_mp[ tcount].insert( make_pair(1,ep) ); //add it with its time, coded as a 1)event 
		}
	}



	//now fix the times
	for(nemp_it = nd_el_mp.begin(); nemp_it != nd_el_mp.end();nemp_it++)
	{

		//calculate the time relative to the last event for events with this time
		thldr = nemp_it->first;		//grabs this elements time
		relT = Get_Rel_T( nd_el_mp, thldr );     //get the time relative to last event
		time = relT; //now get the time in counts realtive to last event

		//now go through mutlmap adding times to the events with this time
		for(e_mmp = nemp_it->second.begin(); e_mmp != nemp_it->second.end() ;e_mmp++ )
		{
			if(e_mmp == nemp_it->second.begin() )
			{
				e_mmp->second->time = time;
			}
			else
			{
				e_mmp->second->time = time  - time;
			}
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
