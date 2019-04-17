#include <cstdio>
#include <set>
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

class move
{
	public:
		int r1;				//the row for this move
		int r2;				//the row for this move

		int c1;				//the column for this move
		int c2;				//the column for this move

		int size;			//the size of the set this move removes /number of blocks removed

		int points;			//the possible points for this move

		vector <int> scores;	//holds the possible scores for this move
};


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
		void analyze_superball(int r1, int c1,int r2,int c2);			//will find all scoring blocks and much more
		void look_bk(int &color, int &myid, int &tsid, int i, int j);		//looks above and back for same colr
																			//and if found does union

		void check_move();
		void get_2_it();		
		map <int, vector <move *> > moves_mp;									//stores pointers to different moves keyed on thier point value		
		void prnt_mp( map <int,vector<int> > &mp, int is_scrng);
		map <int,vector<int> > goals_mp;	//
		map <int,vector<int> > set_mp;		//will hold a color as a key and a vector of pointers to Disjoint sets
		map <int,int > pts_mp;				//will hold the points as a key and the coler as a val

		move em;
		move pm;

		int counter;

};


void Superball::check_move()
{
	int i,j,k,l,p1,p2,p1r,p1c,p2r,p2c,tmp,c1,r1,c2,r2;

	vector <int>  b_rm;			//{#_of_boxes_removed, r1,c1, r2, c2 }
    
	b_rm.resize(5);

	vector <int>  h_pts;				//{#_of_points, r1, c1, r2, c2 }
	
	h_pts.resize(5);

	int pts;

	int mvslft = empty / 5;

	map <int,vector<move *> >::iterator m_it;


	//if have less than 5 boxes left and no boxes to score end it
	if(mvslft < 1 && set_mp.empty() )
	{
		//if you have no boxes to score just find a move and end the game
			//get the first possible move and do it
			for(i = 0; i < r; i++)
			{

				for(j = 0; j < c; j++)
				{

					if(board[i*c+j] !='.' && board[i*c +j] !='*')
					{

						p1 = board[i*c+j];

						r1=i;
						c1=j;

					}

				}
			}

			//get the second part of first  possible move and do it
			for(i = r1; i < r; i++)
			{

				for(j = c1 + 1; j < c; j++)
				{

					if(board[i*c+j] !='.' && board[i*c +j] !='*')
					{

						r2 = i;
						c2 = j;
					}
				}
			}

		printf("ESWAP %d %d %d %d\n",r1,c1,r2,c2);

	}

	else
	{
		//get the row of the ith element
		for(p1r = 0; p1r < r; p1r++)
		{
			//get the column of the ith element
			for(p1c = 0; p1c < c; p1c++)
			{
				//get the vector index 
				p1 = p1r*c +p1c;

				//get the row of the jth second move
				for(p2r = 0; p2r < r; p2r++)
				{

					//get the col of the jth second move
					for(p2c = 0; p2c < c; p2c++)
					{
						p2 = p2r*c + p2c;

						//if the move is not the first move and not a empty
						//try the swap and analyze
						if(p1 != p2 && board[p2] != '*' && board[p2] != '.')
						{

							//do swap and analyze
							tmp = board[p1];

							board[p1] = board[p2];
							board[p2] = tmp;

							analyze_superball(p1r,p1c, p2r,p2c );

							//undo the swap
							tmp = board[p1];

							board[p1] = board[p2];
							board[p2] = tmp;


							//	cout<<"SWAP	p1r p1c p2r p2c\n";

							//take in new board and analyze
							//move on to next move
						}

					}
				}

			}
		}
cout<<"\n\n\n";
		printf("The max pts %d, max boxes; %d\n\n",pm.points,em.size);

		//now choose the move to send
		
		if(mvslft <= 2)   //score if only two moves
		{
			//move through scores vector of the empty move
			for(i = 0; i + 1 <= em.scores.size() ; i += 2)
			{
				printf("2SCORE %d %d\n",em.scores[i],em.scores[i+1] );
			}
		}
//		else if( pm.points > em.size * 2 )
//		{
//
//		}
		else if(empty > (r*c/2)  ) //if around half the board is empty go for the biggest empty
		{
			//go for largest set	

			printf("MSWAP %d %d %d %d\n", em.r1, em.c1, em.r2, em.c2);

		}
		else
		{
			//go for the most points
			printf("PSWAP %d %d %d %d\n", pm.r1, pm.c1, pm.r2, pm.c1);
		}

//		cout<<"	HERE!!!!\n";
	}
//	return;
//	exit(0);
	///now empty out the pointers
}

//repeatedly grabs the board and checks and analyzes
void Superball::get_2_it()
{
	int i,j, myid, tsid;

	string s;

	vector <int> vh;
	while(1)
	{
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

			}
		}

		check_move();
		delete DJS;

		//clear the disjoint set map that contains the goals
//		set_mp.clear();
		cout<<"Give me another sir...\n";
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

	get_2_it();

}

void Superball::look_bk(int &color, int &myid, int &tsid, int i, int j)
{
	int sidup, upid, dwnid, siddwn, nwsid, inc, k, jnc, clrup, clrdwn, cnt;
	if(board[myid] == '.' || board [myid] == '*') return;

	map<int,vector<int> >::iterator mit;

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
		if(clrup == color && sidup != tsid)
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
					set_mp.erase(tsid);
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
					set_mp.erase(sidup);
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
		if(clrdwn == color && tsid != siddwn)
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
					set_mp.erase(tsid);
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
					set_mp.erase(siddwn);
				}

				
			}
		}

	}

	
}


void Superball::analyze_superball(int r1, int c1, int r2, int c2)
{
//	prnt_mp(set_mp,0);

	move * Mvp;

	vector <int> vh;
	
//	move em;						//a move to make the board more empty, done when moves are low

//	move pm;						//done when moves are high

	int i,j,elmnt,p1,p2,myid,sid, cnt ,tgoal,ngoali,pts;

	map <int, vector<int>  >::iterator mit; 	
	map <int, vector<move *>  >::iterator mvit; 	
	
	cnt = 0;
	cout<<"\n\n";
	pts = 0;
printf("The moves being checked are %d and %d (%d) and %d and %d (%d)",r1,c1, r1*c+c1, r2,c2, r2*c+c2 );

cout<<"Scoring sets:\n";

		//move through the set mp looking for scoreable blocks
		//and store the total points and the total number of possible blocks to remove
		for(mit = set_mp.begin(); mit != set_mp.end() ; mit++)
		{
			if(board[mit->first] != '.' && board[mit->first]  != '*' && mit->second[0] )
			{

				//if the score meets the minimum check and store if better
				if(mit->second[1] >= mss )
				{
					printf("  Size: %2d Char: %c Scoring Cell: %d,%d\n",mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ; 
/*
*/		
		
					vh.push_back(mit->second[2] );
					vh.push_back(mit->second[3] );

					pts += mit->second[1] * colors[ board[mit->first] ];  

//					moves_mp[ colors[board[ mit->first ] ] ].push_back( Mvp ) ;
					cnt += mit->second[1];     //get the number of boxes removed
				}
			}

		}

/*
*/
		printf("The total points: %d and the max boxes removed: %d \n",pts, cnt);

		//if this moves points is greater than the stored moves points store this move
		if(pts > pm.points)
		{

			pm.points = pts;
			pm.size   = cnt; 
			pm.r1     = r1;
			pm.c1     = c1;
			pm.r2     = r2;
			pm.c2     = c2;
			pm.scores = vh;
		}

		//if this moves number of boxes removed is greater than the stored moves points store this move
		if(cnt > em.size)
		{
			
			em.points = pts;
			em.size   = cnt; 
			em.r1     = r1;
			em.c1     = c1;
			em.r2     = r2;
			em.c2     = c2;
			em.scores = vh;
		}

cout<<"haaaa\n\n";
		//remove the old disjoint set
//		delete[] DJS;
//		DJS = new Disjoint_set(r*c);

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
