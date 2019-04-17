#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <vector>
#include "DJ.h"
#include <set>
#include <map>

using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
  public:
    Superball(int argc, char **argv);
    int r;
    int c;
    int mss;
    int empty;
	Disjoint * DJS;

    vector <int> board;
    vector <int> goals;
    vector <int> colors;
	void analyze_superball();
	void look_bk(int myid, int mi, int mj);

	map <int,vector<int> > gol_mp;
	map <int, vector<int>  >set_mp;
};

void usage(const char *s) 
{
  fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
  if (s != NULL) fprintf(stderr, "%s\n", s);
  exit(1);
}

Superball::Superball(int argc, char **argv)
{
  int i, j;
  string s;

  if (argc != 5) usage(NULL);

  if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
  if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
  if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

  colors.resize(256, 0);

  for (i = 0; i < strlen(argv[4]); i++) {
    if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
    if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
    if (colors[argv[4][i]] != 0) usage("Duplicate color");
    colors[argv[4][i]] = 2+i;
    colors[toupper(argv[4][i])] = 2+i;
  }

  board.resize(r*c);
  goals.resize(r*c, 0);
  //DJS(r*c);


  empty = 0;

  for (i = 0; i < r; i++) {
    if (!(cin >> s)) {
      fprintf(stderr, "Bad board: not enough rows on standard input\n");
      exit(1);
    }
    if (s.size() != c) {
      fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
      exit(1);
    }
    for (j = 0; j < c; j++) {
      if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0) {
        fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
        exit(1);
      }
      board[i*c+j] = s[j];
      if (board[i*c+j] == '.') empty++;
      if (board[i*c+j] == '*') empty++;
      if (isupper(board[i*c+j]) || board[i*c+j] == '*') {
        goals[i*c+j] = 1;
        board[i*c+j] = tolower(board[i*c+j]);
      }
	}
  }
}

void Superball::analyze_superball()
{
	int i, j, k;

	map <int,vector<int> >::iterator mit;
	map <int,vector<int> >::iterator gmit;
cout<<"in top of analyze\n";

DJS = new Disjoint(r*c);

	//go through board making disjoint sets
	for(i =0; i < r; i++)
	{
		for(j =0; j < c; j++)
		{
			
			set_mp[i*c +j].resize(3);

			set_mp[i*c +j][0] = 1;
			set_mp[i*c +j][1] = i;
			set_mp[i*c +j][2] = j;
			if( board[i*c+j] != '.' && board[i*c +j] != '*')
			{
				look_bk(i*c +j, i,j);	
			}
		
		}
	}
	
	for(mit = set_mp.begin(); mit != set_mp.end(); mit++)
	{

		printf("sid: %d , size %d\n",mit->first, mit->second[0] );
	}
	

	for(gmit = gol_mp.begin(); gmit != gol_mp.end(); gmit++)
	{
		mit = set_mp.find(gmit->first);

		printf("sid: %d, size %d, spot: %d %d\n", gmit->first,mit->second[0], gmit->second[0], gmit->second[1]);
	}


}

void Superball::look_bk(int myid, int mi, int mj)
{
	int i,j,k,msid,upid,upsid,did,dsid,nwsid;

	map<int,vector<int> >::iterator mit;

	vector<int> vh(2,0);
	
	if(board[myid] == '.' || board[myid] == '*') return;


	//make sure what you are looking for is in the map
	mit = set_map.find(myid);

	//if it is not in there move on


	//look above
	if(mi - 1 >= 0)
	{
		upid = (mi-1)*c +mj;
		msid = DJS->Find(myid );
		upsid = DJS->Find(upid);

		//if they have the same color perform union
		if(board[myid] == board[upid] && msid != upsid)
		{	
			

			nwsid = DJS->Union(msid,upsid);

			//if the new set id is my set id get the info 
			if(nwsid == msid)
			{


			mit = set_mp.find(upid);


			//if it is in the map
			if(mit  != set_mp.end() )
			{

				//if it is in the map to the count
				set_mp[msid ][0]  += set_mp[ upid ][0] ;

				//look for a goal if so store it in the goal map
				if( goals[upid] ) 
				{
					vh[0] = mi;
					vh[1] = mj;

					set_mp[msid][1] = set_mp[upid][1];
					set_mp[msid][2] = set_mp[upid][2];


					mit = gol_mp.find(msid);

					//if it is not in the map add it
					if(mit == gol_mp.end())
					{

						gol_mp.insert(make_pair(msid, set_map[msid]) ) ;

					}
					else	//otherwise go to it's part of the map and add to the size
					{
						gol_mp[msid] += set_mp[ upid ][0] ;
					}
				}
				else if( !(goals[myid]) && (goals[upid]) )
				{
					vh[0] = mi-1;
					vh[1] = mj;
					gol_mp.insert(make_pair(msid, vh) ) ;
				}
				else if( (goals[myid]) && (goals[upid]) )
				{
					vh[0] = mi-1;
					vh[1] = mj;
					gol_mp.insert(make_pair(msid, vh) ) ;
				}



			}
			}
		}

		cout<<"above the look behind\n";	
		//look behind
		if(mj - 1 >= 0)
		{
			did = mi*c +(mj-1);
			msid = DJS->Find(myid );
			dsid = DJS->Find(did);


			//if they have the same color perform union
			if(board[myid] == board[did] && msid != dsid)
			{	

				cout<<"hereb\n";
				nwsid = DJS->Union(msid,dsid);

				mit = set_mp.find(dsid);
				//mit->second[0] += mit			
				//if it is in the map to the count
				set_mp[msid ][0]  += set_mp[dsid][0]; 

				//look for a goal if so store it in the goal map
				if(goals[myid] && !(goals[did]) )
				{
					vh[0] = mi;
					vh[1] = mj;
					gol_mp.insert(make_pair(msid, vh) ) ;
				}
				else if( !(goals[myid]) && (goals[did]) )
				{
					vh[0] = mi;
					vh[1] = mj-1;
					gol_mp.insert(make_pair(msid, vh) ) ;
				}
				else if( (goals[myid]) && (goals[did]) )
				{
					vh[0] = mi;
					vh[1] = mj;
					gol_mp.insert(make_pair(msid, vh) ) ;
				}

			}

		}
		cout<<"end\n";
	}

	main(int argc, char **argv)
	{
		Superball *s;
		int i, j;
		int ngoal, tgoal;

		s = new Superball(argc, argv);
		s->analyze_superball();
		tgoal = 0;
		ngoal = 0;
		for (i = 0; i < s->r*s->c; i++) {
			if (s->goals[i] && s->board[i] != '*') {
				tgoal += s->colors[s->board[i]];
				ngoal++;
			}
		}

		printf("Empty cells:                    %2d\n", s->empty);
		printf("Non-Empty cells:                %2d\n", s->r*s->c - s->empty);
		printf("Number of pieces in goal cells: %2d\n", ngoal);
		printf("Sum of their values:            %2d\n", tgoal);
		exit(0);
	}
