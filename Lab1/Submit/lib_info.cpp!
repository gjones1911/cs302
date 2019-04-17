/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Program: lab_info.cpp																																		   //
//Created by: Gerald Jones																																	   //	
//Purpose: This program is Lab 1 of CS302. It will take in a text file consisting of imformation about a MP3 file and reorganize the information in the given  //
//		   format. The program will output the name of each artist, the number of songs by this artist, the total time the artist songs contain on one line.   //
//		   On the next line output the differnt albums of the artist, the song count for the album, and the total time. Following the album on seqenchial lines//
//		   will be the different tracks of that album one after the other and so on																			   //	
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <string>
#include <iostream>
#include <cstdio>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <sstream>
#include <cstdlib>

using namespace std;

//This block creates a song class that represents a song.
struct Song
{
	public:
		string name;
		int track_number;
		int time;


};

//This block creates a album class that will hold the name of the album and info about it.
class Album
{
	public:
		string name;
		int scount;			//holds the album's number of songs
		int total_tm;		//holds the abmount of time the songs from the able take
		map <int ,Song* >  songs;	//points to songs from the able 

};


//This block of code will create a artist class that represents a musical artist
class Artist
{
	public:
		string name;
		int s_total;	//holds the artists 	
		int time_tl;	//holds the given artists total time
		map <string, Album*> albums; //the artists albums

};


//stovec(string): this function takes in a string and uses it to creata a vector of strings that is returned
vector <string>  stovec(string &line)
{
	stringstream ss;		//will be used to turn a given string into a stream of input

	ss.clear();

	ss.str(line);

	string hldr;

	vector <string> rtnv;

	//feed the string called line into holder as seqenchial inputs turning it into a vector of strings
	while(ss>>hldr)
	{
		rtnv.push_back(hldr);
	}

	return rtnv;
}



//cnvrt_ms(int):This function takes in an integer amount of seconds and converts it to a vector of the form [min:sec]
vector <int> cnvrt_ms(int &sec)
{
	int min = sec / 60;	//convert the number of secondes into min's as a integer
	int s = sec % 60;	//collect the remaining seconds

	vector <int> rtn;
	rtn.push_back(min);		//add the minutes to the return vector

	rtn.push_back(s);		//add the seconds to the return vector

	return rtn;
}


//Get_time:This will take in a string of the form min:sec and return the time in seconds as a int
int Get_time(string &ms)
{
	int inx;				
	//find the colon
	inx = ms.find(':');							

	//get the minutes and seconds by creating a substring of the ms string excluding the colon
	//then convert those to integers
	string minstr = ms.substr(0,inx);
	string secstr = ms.substr(inx+1);

	int sec = atoi (secstr.c_str());
	int min = atoi (minstr.c_str());

	//convert the time to seconds and return
	//min_given*60
	return (min * 60) + sec;
}




//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//The below code  will organize and print the mp3 information in the given text file from the command line, with the format specified in the Lab     /////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
	///////////////////////////////////////////////----------------variable declarations-----------/////////////////////////////////
	int i = 0;						//used to iterate


	ifstream i_f;					//an input stream to a file given on the command line
	vector <string> hldr;			//will hold a vector of strings
	vector <string> sv;				//will hold a vector of strings
	vector <string> lines;			//will hold the lines of the input file
	vector <vector<string> > linv;  //will hold the lines of the file as a vector of vectors
	vector <int> minsec(2,0);


	string line;					//will hold the line from the file as a single string
	string shldr;					//will hold a single string
	string gen;						//will hold the genre of the mp3
	string art_nm;					//will hold the artists name as a string
	string alb_nm;					//will hold the album's name
	string title;					//will hold the name of the song
	string t_nm;					//will hold the track's name
	string ms;						//will hold the minutes and seconds of a MP3
	string total;					//will store the total time as a string

	int trknm;						//will hold the track number

	int min;						
	int sec;

	int inx;						//will hold an index

	int sz;

	Song * song ;
	Album * album = new Album;
	map <int, Song *>::iterator  crnt_sng;	//will hold the current song
	Artist * artist = new Artist;			

	map <string, Artist *> table;			//will represent a table of the given artist
	map <string, Artist *> ::iterator t_it;	//will be used to iterate through artist table


	map <string, Album*>::iterator alb_it;	//Will be used to iterate through the album map

	////////////////////////////////////////////////////////////////////////////////////////////////
	/////The below lines of code will read in the given text file and remove the underscores  //////
	////////////////////////////////////////////////////////////////////////////////////////////////
	string filename = argv[1];		//get the file to open from the command line argument

	i_f.open(filename.c_str());		//open the neccessary file
	if(i_f.fail())					//check for failure to open file and send error message
	{
		cerr<<"Failed to Open file...\n"; return -1;
	}
	//get the different lines in the file and store them in the vector of strings called lines
	while(getline(i_f,line) )
	{


		lines.push_back(line);

	}


	//adjust the size of linv using the size of the vector lines
	linv.resize(lines.size());


	//go through the lines vector turning each line into a vector of strings using the stovec function
	for(int i=0; i < lines.size();i++)
	{
		sv = stovec (lines.at(i));

		linv.at(i) =  sv  ;   //add the new vector of strings to the lines vector of vectors
	}

	//remove the underscores and replace with spaces
	//go through each string one by one looking for _'s
	for(int r=0;r < linv.size(); r++ )
	{
		for(int c=0; c < linv.at(r).size(); c++ )
		{

			shldr = linv.at(r).at(c);		//get the current string

			//go through it looking for underscores if found change to a space
			for(int idx = 0; idx < shldr.size() ; idx++)
			{

				if(shldr.at(idx) == '_' )
				{

					shldr.replace(idx,1," ");
				}

			}

			//replace withjadjusted string
			linv.at(r).at(c) = shldr;
		}

	}


	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////The code segment below will create Artist, Album and Song instances as neccessary by going through the lines vector            ////////////
	////to get the information. Then it will add the artist to the table map used for storing the informatio                           ////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	///go through the lines vector row by row, each row represents a song. 
	for(int r=0;r < linv.size(); r++ )
	{

		/////get the track info
		art_nm = linv.at(r).at(2);			//get the current tracks artist
		alb_nm = linv.at(r).at(3);			//album name  
		title = linv.at(r).at(0);			//songs name
		ms = linv.at(r).at(1);				//min:seconds
		gen = linv.at(r).at(4);				//song genre
		t_nm = linv.at(r).at(5);			//track number

		//get the track number as a int
		trknm = atoi(t_nm.c_str());	

		//convert the tracks time into seconds as a integer
		sec = Get_time(ms);

		///create a new song instance for this song
		song = new Song;

		//set up the song info
		song->name = title;
		song->track_number = trknm;
		song->time = sec;


		//look for this artist in the table, t_it = current artist finder
		t_it = table.find(art_nm);

		//if the artist is not found add them to the table and set up a album table for this artist.
		//Then add the song info for the artist and the album
		if(t_it == table.end() )
		{



			//create new artist and album instance
			artist = new Artist;
			album = new Album;

			//set up the album
			album->name = alb_nm;
			album->total_tm = sec;
			album->scount = 1;
			album->songs.insert( make_pair(trknm, song) );

			//set up this artist
			artist->name = art_nm;
			artist->s_total = 1;
			artist->time_tl = sec;
			artist->albums.insert( make_pair(alb_nm , album) );

			//add this artist to the table
			table.insert( make_pair(art_nm, artist) );
		}

		//if the artist is found add the song and if need be the album
		else
		{


			//grab this artist pointer
			artist = t_it->second;
			artist->time_tl += sec;			//add the time to the total
			artist->s_total += 1;			//add to the artist's song count

			//look for this album in the artist albums map
			alb_it =  artist->albums.find(alb_nm);

			//if the album is not found add it then the song to the album's songs map
			if(alb_it == artist->albums.end())
			{

				//create new album
				album = new Album;

				//set up the album
				album->name = alb_nm;
				album->total_tm = sec;
				album->scount = 1;
				album->songs.insert( make_pair(trknm, song) );

				//add album to the artist's map
				artist->albums.insert(make_pair(alb_nm,album)  );

			}
			//otherwise the ablum was found so  add this song to the album
			else
			{
				album = alb_it->second;	//get a pointer to this album

				//add the song to this album
				album->songs.insert(make_pair(trknm, song ) );
				album->total_tm += sec;
				album->scount +=1;
			}
		}
	}		

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//This portion of the code will print the contents of the file in the following format:                               //
	//Artist: Number of songs, total min's: total sec's                                                                   //
	//        Album Name: # songs from album, Min's:Sec's                                                                 //
	//                1. Song title: Min's:Sec's                                                                          //
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//got thtough the Storage table(map) pulling out the neccessary information
	for( t_it = table.begin(); t_it != table.end(); t_it++)
	{
		//get the current artist name
		shldr = t_it->first;

		//get the artist pointer
		artist = t_it->second;

		//convert the time to min sec
		minsec = cnvrt_ms(artist->time_tl);	

		//The below if/else statement will add a 0 to the secondes to make it match the required format
		//i.e. turne :2 -> :02
		if(minsec.at(1) < 10)
		{

			printf("%s: %d, %d:0%d\n",t_it->first.c_str(),artist->s_total,minsec.at(0),minsec.at(1));
		}
		else
		{
			printf("%s: %d, %d:%d\n",t_it->first.c_str(),artist->s_total,minsec.at(0),minsec.at(1));
		}
		//album name: song count, Min:sec
		for(alb_it = artist->albums.begin(); alb_it != artist->albums.end();alb_it++ )
		{

			album = alb_it->second;	//Get the pointer to a album
			shldr = alb_it->first;	//Get the name of the album
			i = album->scount;		//Get the total song count for this album

			//use the pointer to get the album time convert it into min and sec
			minsec = cnvrt_ms(album->total_tm);

			//The below code segment will add a extra zero to match the format requiremnt.
			if(minsec.at(1) < 10)
			{

				printf("%*s%s: %d, %d:0%d\n",8,"",shldr.c_str(), i,minsec.at(0), minsec.at(1) );
			}
			else
			{

				printf("%*s%s: %d, %d:%d\n",8,"",shldr.c_str(), i,minsec.at(0), minsec.at(1) );

			}

			//go through this album's songs printing them like: track number. title, Min:Sec
			for(crnt_sng = album->songs.begin(); crnt_sng != album->songs.end(); crnt_sng++)
			{
				shldr = crnt_sng->first;		//get the name of this song
				song = crnt_sng->second;		//get the pointer to this song

				minsec = cnvrt_ms(song->time);
				if(minsec.at(1) < 10)
				{

					printf("%*s%d. %s: %d:0%d\n",16,"",song->track_number, song->name.c_str(), minsec.at(0), minsec.at(1));
				}
				else
				{
					printf("%*s%d. %s: %d:%d\n",16,"",song->track_number, song->name.c_str(), minsec.at(0), minsec.at(1));
				}
			}
		}
	}

	return 0;
}
