#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctype.h>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "DJ.h"
using namespace std;

#define talloc(type, num) (type *) malloc(sizeof(type)*(num))

class Superball {
	public:
		Superball(int argc, char **argv);
		int r;					//number of rows
		int c;					//number of columns
		int mss;				//minimum number of boxes to score
		int empty;				//the number of empty cells
		vector <int> board;		//at board[idx] holds either: color{upper case = goal), "."=empty,"*"=empty goal
		vector <int> goals;		//same size as board and at board[idx] if goal[idx] = 1 then that part is a goal 
		vector <int> colors;				//holds the at[color char] = points for that color
		Disjoint * DJS;						//a pointer to a disjoint set representing the board(size = r*c)
		void analyze_superball();			//will find all scoring blocks and much more
		void look_bk(int &color, int &myid, int &tsid, int i, int j);		//looks above and back for same colr
																			//and if found does union
		
		
		void prnt_mp( map <int,vector<int> > &mp, int is_scrng);
		map <int,vector<int> > goals_mp;	//
		map <int,vector<int> > set_mp;		//will hold a color as a key and a vector of pointers to Disjoint sets
		map <int,int > pts_mp;				//will hold the points as a key and the coler as a val

};


//will print the map 
void Superball::prnt_mp( map <int,vector<int> > &mp, int is_scrng)
{

	int sid, color, goal, id,i,cnt;

	map <int, vector<int> >::iterator mit;
	
	cnt = 0;

	if( !(is_scrng) )
	{
		cout<<"the map is:\n";

		for(mit = mp.begin(); mit != mp.end() ; mit++)
		{
			if(mit->first != '.' && mit->first != '*' )
			{
				printf("element %d of map is %d with color %c, size %d and goal status %d \n", 
						cnt, mit->first, board[mit->first], mit->second[1], mit->second[0]);

				cnt++;
			}
		}

	}
	else
	{
		cout<<"Scoring sets:\n";

		for(mit = mp.begin(); mit != mp.end() ; mit++)
		{
			if(mit->first != '.' && board[mit->first]  != '*' && mit->second[0] )
			{
				if(mit->second[1] >= mss)
				{
					printf("Size: %2d Char: %c Scoring Cell: %d,%d\n",mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ; 
				}
			}
		}


	}
}

void usage(const char *s) 
{
	fprintf(stderr, "usage: sb-read rows cols min-score-size colors\n");
	if (s != NULL) fprintf(stderr, "%s\n", s);
	exit(1);
}


//creates the instance of Superball
Superball::Superball(int argc, char **argv)
{
	int i, j,myid, tsid ;
	string s;
	
	vector <int> vh;

	vh.resize(4,0);

	if (argc != 5) usage(NULL);

	//take in command line args while error checking
	if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
	if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
	if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

	//set up the colors vector the index with the value of that color stores its point value
	colors.resize(256, 0);

	//take in the colors and set their point value while error checking
	for (i = 0; i < strlen(argv[4]); i++) 
	{
		if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
		if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
		if (colors[argv[4][i]] != 0) usage("Duplicate color");
		colors[argv[4][i]] = 2+i;

		pts_mp[2+i] = argv[4][i] ;							//store this color and its points

		colors[toupper(argv[4][i])] = 2+i;					//store the upper case version and its points
	}

	//set up board and goals vector
	board.resize(r*c);
	goals.resize(r*c, 0);

	//set up the disjoint set
	DJS = new Disjoint(r*c);
	empty = 0;							//initialize the empty count

	for (i = 0; i < r; i++) 
	{
		if (!(cin >> s))	//get the string/row of the board and error check
		{
			fprintf(stderr, "Bad board: not enough rows on standard input\n");
			exit(1);
		}
		if (s.size() != c) 
		{
			fprintf(stderr, "Bad board on row %d - wrong number of characters.\n", i);
			exit(1);
		}
		//go through element of the row of board
		for (j = 0; j < c; j++) 
		{
			if (s[j] != '*' && s[j] != '.' && colors[s[j]] == 0)
			{
				fprintf(stderr, "Bad board row %d - bad character %c.\n", i, s[j]);
				exit(1);
			}

			vh.resize(4,0);
			board[i*c+j] = s[j]; 	    //set this part of the board vector

			myid = i*c+j;				//get the id of this part of the disjoint set
			vh[1] = 1;					//set this ones size to 1
			
			tsid = DJS->Find(myid);		//get this element of the DJS's true set id 
			//reads the element in the string into the board
			if (board[i*c+j] == '.') empty++;			//will be a . a * or a letter/color
			if (board[i*c+j] == '*') empty++;
			if(islower(board[myid]) ) vh[0] = 0;
/*
			if(isupper(board[myid]) )
			{
				cout<<"we have a goal at "<<i<<" "<<j<<"\n";
				vh[0] = 1;
				vh[2] = i;
				vh[3] = j;
			}
*/
			if (isupper(board[i*c+j]) || board[i*c+j] == '*') 
			{
				goals[i*c+j] = 1;

				vh[0] = 1;
				vh[2] = i;
				vh[3] = j;
				board[i*c+j] = tolower(board[i*c+j]);
			}
			else
			{
				vh[0] = 0;
				vh[2] = i;
				vh[3] = j;
			}
		
			set_mp[myid] = vh;
	//		prnt_mp(set_mp,0);

			if (board[i*c+j] != '.' && board[i*c+j] != '*') 		//will be letter/color
			{
				//now analyze this element of the board and whats around it

				look_bk(board[myid], myid ,tsid ,i ,j);
			}
				
	//		cout<<"after looking around\n\n";

	//		prnt_mp(set_mp,0);
		}
	}


	analyze_superball();
}

void Superball::look_bk(int &color, int &myid, int &tsid, int i, int j)
{
	int sidup, upid, dwnid, siddwn, nwsid, inc, k, jnc, clrup, clrdwn, cnt;

	map<int,vector<int> >::iterator mit;

	//base if im at (0,0) just return
//	if( i == 0 && j == 0) return;

	//look up 
	if( i - 1 >= 0) 
	{
		tsid = DJS->Find(myid);
		upid = (i-1)*c + j;
	
		clrup = board[ upid ];

		sidup = DJS->Find(upid);
		
		//if they have the same set leave them alone
//		if(sidup == tsid) return;
		
		//if they share a color connect them
		if(clrup == color && upid != tsid)
		{
			nwsid = DJS->Union(sidup,tsid);
			
			//see if the new set id is the same as the sidup or tsid
			//if = sidup then tsid need to be removed 
			//otherwise = tsid so look for and remove sidup
			if(nwsid == sidup)
			{
				//look for tsid in map
				mit = set_mp.find(tsid);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];
					
					set_mp[sidup][1] += cnt;

					//if it is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[sidup][0] = 1;	//set it as a goal set

						set_mp[sidup][2] = mit->second[2]; //save this goal value
						set_mp[sidup][3] = mit->second[3]; //save this goal value
					}
				
					//now remove it
//					set_mp.erase(tsid);
				}
			}

			else
			{
				//look for sidup in map
				mit = set_mp.find(sidup);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];
					
					set_mp[tsid][1] += cnt;

					//if it is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[tsid][0] = 1;	//set it as a goal set

						set_mp[tsid][2] = mit->second[2]; //save this goal value
						set_mp[tsid][3] = mit->second[3]; //save this goal value
					}
				
					//now remove it
//					set_mp.erase(sidup);
				}
			}
		}

	}
	
	//look back
	if( j - 1 >= 0) 
	{
		tsid = DJS->Find(myid);
		dwnid = i*c + (j-1) ;
		
		clrdwn = board[ dwnid ];

		siddwn = DJS->Find(dwnid);
		
		//if they have the same set leave them alone
//		if(siddwn == tsid) return;
//		if(clrdwn != color) return;	

        //if they share a color connect them
		if(clrdwn == color && tsid != dwnid)
		{  

			nwsid = DJS->Union(siddwn,tsid);
			
			//see if the new set id is the same as the sidup or tsid
			//if = sidup then tsid need to be removed 
			//otherwise = tsid so look for and remove sidup
			if(nwsid == siddwn)
			{
				//look for tsid in map
				mit = set_mp.find(tsid);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];
					
					set_mp[siddwn][1] += cnt;

					//if it is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[siddwn][0] = 1;	//set it as a goal set

						set_mp[siddwn][2] = mit->second[2]; //save this goal value
						set_mp[siddwn][3] = mit->second[3]; //save this goal value
					}
				
					//now remove it
//					set_mp.erase(tsid);
				}
			}

			else
			{
				//look for siddwn in map
				mit = set_mp.find(siddwn);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];
					
					set_mp[tsid][1] += cnt;

					//if it is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[tsid][0] = 1;	//set it as a goal set

						set_mp[tsid][2] = mit->second[2]; //save this goal value
						set_mp[tsid][3] = mit->second[3]; //save this goal value
					}
				
					//now remove it
//					set_mp.erase(siddwn);
				}

				
			}
		}

	}

	
}


void Superball::analyze_superball()
{
//	prnt_mp(set_mp,0);
	prnt_mp(set_mp,1);
}

main(int argc, char **argv)
{
	Superball *s;
	int i, j;
	int ngoal, tgoal;

	s = new Superball(argc, argv);

//	s->analyze_superball();

	tgoal = 0;
	ngoal = 0;
	for (i = 0; i < s->r*s->c; i++) {
		if (s->goals[i] && s->board[i] != '*') {
			tgoal += s->colors[s->board[i]];
			ngoal++;
		}
	}
/*

cout<<"\n\n";
	printf("Empty cells:                    %2d\n", s->empty);
	printf("Non-Empty cells:                %2d\n", s->r*s->c - s->empty);
	printf("Number of pieces in goal cells: %2d\n", ngoal);
	printf("Sum of their values:            %2d\n", tgoal);

*/
	exit(0);
}
