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

		vector <int> scores;	//holds the possible scores for this move if scores vec is empty then their are no scores
		
		vector<int> clrs;		//will hold the colors from least to greatest value startsing at 2--->num of elemnts +1

		map <int,vector <int> >nm_clrs;	//will hold the numbers of the sets of indivdual colors keyed on color

//		int sw_o_sc;         //signifies if this is a swap move (0) or a score move (1)
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

		void Get_1st();
		void check_move();
		void get_2_it();		
		map <int, vector <move *> > moves_mp;									//stores pointers to different moves keyed on thier point value		
		void prnt_mp( map <int,vector<int> > &mp, int is_scrng);
		map <int,vector<int> > goals_mp;	//
		map <int,vector<int> > set_mp;		//will hold a color as a key and a vector of pointers to Disjoint sets
		map <int,int > pts_mp;				//will hold the points as a key and the coler as a val

		move em;							//a move to try and empty the board
		move pm;							//a move to try and score the most points
		move gm;							//a move to do when no scores can be made but are almost possible
		move gnm;							//a move to do when no scores can be made
		
		vector <int> clrs;

		int counter;
		int c_size;
		set <int> nrsc;						//near scoring sets 
		set <int> nsc;						//non scoreing sets

//		int nrNm = 0;
//		int Nnnm = 0;

};

//gets the first move possible to end the game
void Superball::Get_1st()
{


	int i,j,p1, r1,c1,r2,c2;

	
	//if you have no boxes to score just find a move and end the game
			//get the first possible move and do it
			for( i = 0; i < r; i++)
			{

				for(j = 0; j < c; j++)
				{

					if(board[i*c+j] !='.' && board[i*c +j] !='*')
					{

						p1 = board[i*c+j];

						r1=i;
						c1=j;

						i = r;
						j = c;

					}

				}
			}

			//get the second part of first  possible move and do it
			for(i = 0; i < r; i++)
			{

				for(j = 0 ; j < c; j++)
				{

					if(board[i*c+j] !='.' && board[i*c +j] !='*' && i != r1 && j != c1)
					{

						r2 = i;
						c2 = j;
						i = r;
						j = c;
					}
				}
			}



		printf("pSWAP %d %d %d %d\n",r1,c1,r2,c2);
}

void Superball::check_move()
{
	int i,j,k,l,p1,p2,p1r,p1c,p2r,p2c,tmp,c1,r1,c2,r2;

	counter = 0;
	c_size = 0;

	vector <int>  b_rm;			//{#_of_boxes_removed, r1,c1, r2, c2 }
    
	b_rm.resize(5);

	vector <int>  h_pts;				//{#_of_points, r1, c1, r2, c2 }



	h_pts.resize(5);

	int pts;

	int mvslft = empty / 5;

	map <int,vector<move *> >::iterator m_it;


	//if have less than 5 boxes after the next move end the game
	if(mvslft < 1 )
	{
		Get_1st();
	}

	else		//go through the board analyzing all possible moves
	{
		
		////////////////////////////////////////////////////////////////////////////////////
		//get the row of the ith element
		for(p1r = 0; p1r < r; p1r++)
		{
			//get the column of the ith element
			for(p1c = 0; p1c < c; p1c++)
			{
				//get the vector index /id of this move
				p1 = p1r*c +p1c;

				

				//if the piece is not empty
				if(board[ p1 ] != '*' && board[ p1 ] != '.')
				{
					//get the row of the jth second move
					for(p2r = 0; p2r < r; p2r++)
					{

						//get the col of the jth second move
						for(p2c = 0; p2c < c; p2c++)
						{
							p2 = p2r*c + p2c;		//get the id of this move

							//if the move is not the first move and not a empty
							//try the swap and analyze
							if(p1c != p2c && p1r != p2r && board[p2] != '*' && board[p2] != '.')
							{
								//do swap and analyze
								tmp = board[p1];

								board[p1] = board[p2];
								board[p2] = tmp;

								//analyze board at this state
								analyze_superball(p1r,p1c, p2r,p2c );

								//undo the swap
								tmp = board[p1];

								board[p1] = board[p2];
								board[p2] = tmp;
							}

						}
					}

				}

			}
		}

		/////////////////////////////////////////////////////////////////////////////////////////////////
//		cout<<"\n\n\n";
//		printf("The max pts %d, max boxes; %d\n\n",pm.points,em.size);
/*
		//now choose the move to send
		////scr whn two moves, the number of blocks removed is grtr than half board
		//or when points are great
		if(mvslft <= 2 || em.size > ( (r*c) / 2 - 2 ))
		{ 
			if(  pm.points >= 90)
			{
				//move through scores vector of the high point  moves
				for(i = 0; i + 1 <= em.scores.size() ; i += 2)
				{
					printf("SCORE %d %d\n",pm.scores[i],pm.scores[i+1] );
				}
			}
			else 
			{
				//move through scores vector of the empty move
				for(i = 0; i + 1 <= em.scores.size() ; i += 2)
				{
					printf("SCORE %d %d\n",em.scores[i],em.scores[i+1] );
				}
			}
		}
		else if(empty > (r*c/2)  ) //if around half the board is empty go for the biggest empty
		{
			//if you can make it three more moves(#boxes removed - 3 = # of boxes after move) go for points 
			if(em.size == 0 && pm.points == 0 )
			{
				Get_1st();
			}
			else if(em.size >= 18)
			{  
				
				printf("3SWAP %d %d %d %d\n", pm.r1, pm.c1, pm.r2, pm.c1);
			}
			else
			{
				//go for largest set	

				printf("4SWAP %d %d %d %d\n", em.r1, em.c1, em.r2, em.c2);
			}


		}
		else
		{
			//go for the most points
			printf("5SWAP %d %d %d %d\n", pm.r1, pm.c1, pm.r2, pm.c1);
		}
*/

	
	}






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
			
			//go through the row of the board (the string s)
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

				//count the empties
				if (board[i*c+j] == '.') empty++;		
				if (board[i*c+j] == '*') empty++;

				//
		//		if(islower(board[myid]) ) vh[0] = 0;

				
				//if this is a goal mark it as one , if not mark that 
				if (isupper(board[i*c+j]) || board[i*c+j] == '*') 
				{
					goals[i*c+j] = 1;

					vh[0] = 1;
					board[i*c+j] = tolower(board[i*c+j]);
				}
				else
				{
					vh[0] = 0;
				}
				
			
				//store this values row(i) and column(j)
				vh[2] = i;
				vh[3] = j;
				
				
				//add this element to the disjoint set map.....adds every elemnt i think
				// can leave out the . and * -------------************
				//
				//
				set_mp[myid] = vh;
	
				//		prnt_mp(set_mp,0);

				if (board[i*c+j] != '.' && board[i*c+j] != '*') 		//if element s[j] is a letter/color
				{
					//now analyze this element of the board and whats around it
                    //now look behind and above this block performing
					//union on it and any blocks that are the same color and disjoint from it
					look_bk(board[myid], myid ,tsid ,i ,j);
				}

			}
		}

		check_move();
		delete DJS;
		
		return;

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

	pm.points = 0;
	pm.size = 0;
	em.points = 0;
	em.size = 0;
	gm.size = 0;
	gm.points = 0;
	gnm.size = 0;
	gnm.points = 0;

	vh.resize(4,0);

	if (argc != 5) usage(NULL);

	//take in command line args while error checking
	if (sscanf(argv[1], "%d", &r) == 0 || r <= 0) usage("Bad rows");
	if (sscanf(argv[2], "%d", &c) == 0 || c <= 0) usage("Bad cols");
	if (sscanf(argv[3], "%d", &mss) == 0 || mss <= 0) usage("Bad min-score-size");

	//set up the colors vector the index with the value of that color stores its point value
	colors.resize(256, 0);

	c_size = strlen(argv[4] );
	//take in the colors and set their point value while error checking
	for (i = 0; i < strlen(argv[4]); i++) 
	{
		if (!isalpha(argv[4][i])) usage("Colors must be distinct letters");
		if (!islower(argv[4][i])) usage("Colors must be lowercase letters");
		if (colors[argv[4][i]] != 0) usage("Duplicate color");
		colors[argv[4][i]] = 2+i;

		clrs.push_back( argv[4][i] );
		
		pts_mp[2+i] = argv[4][i] ;							//store this color and its points

		colors[toupper(argv[4][i])] = 2+i;					//store the upper case version and its points
	}

	//set up board and goals vector
	board.resize(r*c);
	goals.resize(r*c, 0);

	//set up points vector for moves

	//take in the board and analyze for next move
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

						set_mp[siddwn][2] = mit->second[2]; //save this goals row value
						set_mp[siddwn][3] = mit->second[3]; //save this goals column value
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

						set_mp[tsid][2] = mit->second[2]; //save this goals row value
						set_mp[tsid][3] = mit->second[3]; //save this goals column value
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

	vector <int> nmcl( c_size, 0 );

	//	move em;						//a move to make the board more empty, done when moves are low

	//	move pm;						//done when moves are high

	int i,j,elmnt,p1,p2,myid,sid, cnt ,tgoal,tnum, nsnm, pts, nrnm, nrpts, gpts;

	map <int, vector<int>  >::iterator mit; 	
	map <int, vector<move *>  >::iterator mvit;
	vector <int> bg_ng(4,0);
//	set <int> nrsc;
//	set <int> nsc;
	set <int>::iterator sit;
	cnt = 0;
	//	cout<<"\n\n";
	pts = 0;
	tnum = 0;

	
	//move through the set mp looking for scoreable blocks
	//and store the total points and the total number of possible blocks to remove
	//it the id was part of a scoring set it was removed so cant look for it
	//if can look for it do so and look around it for scoring sets or blocks
	for(mit = set_mp.begin(); mit != set_mp.end() ; mit++)
	{
		if(board[mit->first] != '.' && board[mit->first]  != '*' && mit->second[0] )
		{

			//if the score meets the minimum check and store if better
			if(mit->second[1] >= mss )
			{
				//printf("  Size: %2d Char: %c Scoring Cell: %d,%d\n",mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ; 

				vh.push_back(mit->second[2] );        //get the row of this move
				vh.push_back(mit->second[3] );		  //get the column of this move

				pts += mit->second[1] * colors[ board[mit->first] ];  

				//					moves_mp[ colors[board[ mit->first ] ] ].push_back( Mvp ) ;
				cnt += mit->second[1];     //get the number of boxes removed
			}
			else		//if it is a goal but doesnt meet the minimun count it for the number of near goals and 
			{

				//insert the set id in the set
				nrsc.insert( DJS->Find( mit->first ) );
			}

		}

		//NON GOAL SAVE
		//if not a goal store board info and not an empty
		else if(board[mit->first] != '.' && board[mit->first]  != '*' && !(mit->second[0]) )
		{
			//get the the id of this : mit->first
			//its color : board[ mit->first ]
			//look for it in the set and get its size and store it in that part of the gm_map
			nsc.insert( DJS->Find( mit->first ) );
		
		}
	}

	counter++;
cout<<"\n";
/*
	//now get the total size of sets for each color
	for(mit = gm.nm_clrs.begin(); mit != gm.nm_clrs.end(); mit++)
	{
			printf("the color is:  %c and it sets are, \n",mit->first); 

		for(i = 0; i < mit->second.size();i++)
		{
			cout<<i+1<<": "<<mit->second[ i ]<<"\n";
		}

	}
*/

cout<<"\n";
//	cout<<"\n\n"<<counter<<": ";
//	printf("The total damn points: %d and the max boxes removed: %d \n\n",pts, cnt);

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

	//cout<<"haaaa\n\n";
	//remove the old disjoint set
	//		delete[] DJS;
	//		DJS = new Disjoint_set(r*c);


	//now go through the near scoring and non scoring sets
	for(sit = nrsc.begin(); sit != nrsc.end()  ; sit++)
	{
		mit = set_mp.find( *sit );				//get iterator to this set id

		//get the size of this set
		nrnm += mit->second[1];						//add up the total number of boxes in almost goals

		nrpts += mit->second[1] * colors[ board[ *sit ] ];		//calculate the possible points for this swap

	}

	//now if this number is greater than the number store store this as the general move
	if(nrpts > gm.points)
	{

		gm.points = pts;
		gm.size   = nrnm;; 
		gm.r1     = r1;
		gm.c1     = c1;
		gm.r2     = r2;
		gm.c2     = c2;

	}
	
	//go through the no score set counting the number of the different colored blocks
	for(sit = nsc.begin() ; sit != nsc.end() ; sit++)
	{
		mit = set_mp.find( *sit );		//get an iterator to this id in the map

		

		nmcl[ colors[ board[ *sit  ]  ] - 2 ] += mit->second[1];
	
	}


	int c_brk = 0; 

	//rank the board 
	for(i = 0 ; i < nmcl.size(); i ++)
	{
		c_brk += nmcl[ i ] * (i + 2) ;  //rank = num colors * pt value

		nsnm += nmcl[i]; 
	}

	//if this moves board's rank out ranks the current greatest save this move
	if( c_brk > gnm.size)
	{
		gnm.size = c_brk;
		gnm.points = nsnm;
		gnm.r1 = r1;
		gnm.c1 = c1;
		gnm.r2 = r2;
		gnm.c2 = c2;
	}
}

main(int argc, char **argv)
{
	Superball *s;
	int i, j;
	int ngoal, tgoal;

	s = new Superball(argc, argv);

	//	s->analyze_superball();

/*

	tgoal = 0;
	ngoal = 0;
	for (i = 0; i < s->r*s->c; i++) {
		if (s->goals[i] && s->board[i] != '*') {
			tgoal += s->colors[s->board[i]];
			ngoal++;
		}
	}
*/
	/*

cout<<"\n\n";
	printf("Empty cells:                    %2d\n", s->empty);
	printf("Non-Empty cells:                %2d\n", s->r*s->c - s->empty);
	printf("Number of pieces in goal cells: %2d\n", ngoal);
	printf("Sum of their values:            %2d\n", tgoal);

*/
	exit(0);
}

