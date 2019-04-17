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


//This class represents a move on the board, can be either:
//A score or a Swap. 
//scoring block. If it is a move 
class move
{
	public:
		int r1;				//the 1st blocks row for this move
		int r2;				//the 1st blocks column for this move

		int c1;				//the 2nd blocks row for this move
		int c2;				//the 2nd blocks col for this move

		int size;			//the size of the set this move removes /number of blocks removed

		int points;			//the possible points for this move

		vector <int> scores;	//holds the possible scores for this move if scores vec is empty then their are no scores
		
		vector<int> clrs;		//will hold the colors for this board

		map <int,vector <int> >nm_clrs;	//will hold the numbers of the sets of indivdual colors keyed on color

};


class Superball {
	public:
		Superball(int argc, char **argv);
		int r;					//number of rows
		int c;					//number of columns
		int mss;				//minimum number of boxes to score
		int empty;				//the number of empty cells
		vector <int> board;		//at board[idx] holds either: color{upper case = goal), "."=empty,"*"=empty goal
	
		vector<int> oboard;
		vector <int> goals;		//same size as board and at board[idx] if goal[idx] = 1 then that part is a goal 
		vector <int> colors;				//holds the at[color char] = points for that color
		
		Disjoint * DJS;						//a pointer to a disjoint set representing the board(size = r*c)
		
		void analyze_superball(int r1, int c1,int r2,int c2);			//will analyze the board for the given move
		
		void look_bk(int &color, int &myid, int &tsid, int i, int j);		//looks above and back for the same color
																			//and if found does union

		void Get_1st();														//will get the first possible move to end the game
		void check_move();													//goals through all moves trying to decide which one
		void get_2_it();													//takes in the given board and starts the process	
		void prnt_mp( map <int,vector<int> > &mp, int is_scrng);			//prints all or part of the set map
		map <int,vector<int> > goals_mp;	
		map <int,vector<int> > set_mp;		//will hold a set id as a key and a vector with info about the set
		map <int,int > pts_mp;				//will hold the points as a key and the coler as a val

		move em;							//a move to try and empty the board
		move pm;							//a move to try and score the most points
		move gm;							//a move to do when no scores can be made but are almost possible
		move gnm;							//a move to do when no scores can be made
	    move scm;                           //a scoring move that will focus on blocks trying to make the most points
		move esm;							//a scoring move that focus on trying to empty the board
		
		void set_disjoint();				//sets up the disjoint set for the current board

		vector <int> clrs;

		int counter;
		int c_size;						
		set <int> nrsc;						//near scoring sets 
		set <int> nsc;						//non scoreing sets
		set <int> scrng;
		
		void prnt_bd();						//will print the current board
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

		printf("SWAP %d %d %d %d\n",r1,c1,r2,c2);
}


//Will print the current state of the board
void Superball::prnt_bd()
{
	int i,j;

	for(i=0; i < r; i++)
	{
		for(j=0; j < c; j++)
		{
			printf("%c",board[i*c +j] );
		
		}

		cout<<"\n";
	}

	cout<<"\n";
}



//will go through current board setting up disjointed sets of the different colors that touch and setting up the sets map
void Superball::set_disjoint()
{
	int i,j,myid,sid,block;


	set_mp.clear();

	delete DJS;

	vector<int> vh(4,0);

	DJS = new Disjoint(r*c);

	for(i = 0; i < r; i++)
	{
		for(j = 0; j < c ; j++)
		{
			block = board[i*c + j];
			myid = i*c + j;
			sid = DJS->Find(myid);
			vh[1] = 1;
			vh[2] = i;
			vh[3] = j;

			if(goals[myid] )
			{
				vh[0] = 1;
			}
			else
			{
				vh[0] = 0;
			}

			

			set_mp[myid] = vh;

			//no look above and behind this spot for matching colors
			if(block != '*' && block != '.')
			{

				look_bk(block,myid,sid,i,j);
		    }
		}
	}

}




//will go through all possible moves and try to choose the "best" one
void Superball::check_move()
{
	int i,j,k,l,p1,p2,p1r,p1c,p2r,p2c,tmp,c1,r1,c2,r2;

	counter = 0;
	
	int mvslft = empty / 5;				//estimate the number of moves left


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
							if(board[p1] != board[p2] && p2 != p1 && board[p2] != '*' && board[p2] != '.' )
							{
								if(counter < 2)
								{
//									printf("about to swap %d %d and %d %d\n",p1r,p1c,p2r,p2c);
							//		prnt_bd();
								}
								//do swap and analyze
								tmp = board[p1];

								board[p1] = board[p2];
								board[p2] = tmp;

//								if(counter < 2)
//								{
//									printf("did swap %d %d and %d %d\n",p1r,p1c,p2r,p2c);
//								//	prnt_bd();
//								}
							
								
								set_disjoint();
								
cout<<"";								//analyze board at this state
								analyze_superball(p1r,p1c, p2r,p2c );

//cout<<"made it 3\n";								//analyze board at this state
								//now i need to adjust the disjoint set


								//undo the swap
								tmp = board[p1];

								board[p1] = board[p2];
								board[p2] = tmp;

//								if(counter < 2)
//								{
				//					printf("did re-swap %d %d and %d %d\n",p1r,p1c,p2r,p2c);
								//	prnt_bd();
//								}

								
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
*/

	
	}
//cout<<"in check after analyze\n";
	
	int rw = 0; 
	int cl = 0;
	int bgst = 0;

	int ro = 0;
	int co = 0;

//cout<<"here\n\n";	
//cout<<"the number of moves left is: "<<mvslft<<"\n";
//cout<<"the empty score moves size is  is: "<<esm.scores.size()<<"\n";
//printf("the  esm size: %d, its points : %d , it scores size: %d \n",esm.size, esm.points, esm.scores.size() );

/*
for(i = 0; i < scm.scores.size();i++)
{
	cout<<scm.scores[i]<<"\n\n";
}
*/
	
//cout<<" pm's scores map is
	//if need to score
	//if moves left is 2 and i can score do so
   if(esm.scores.size() > 0)
{
	if( mvslft < 4 || esm.size  > c/2)
	{
		//if there is a largest score 
		if(mvslft ==  3)
		{

			//go for points
			printf("SCORE %d %d\n", scm.scores[0], scm.scores[1] );
		}

		else		
		{
			//go for empty
			printf("SCORE  %d %d\n",esm.scores[0], esm.scores[1] );
		}
	}

}
//if no scores possible 
else if(scm.scores.size() == 0 ) 
{
//	if( empty > r*c/4) //if there is more than a fourth of the board empty 
//	{
		//go for possible points
		printf("SWAP %d %d %d %d\n",gm.r1, gm.c1, gm.r2, gm.c2 );
//	}
//	else		//otherwise there is less than a fourth of the board so try to empty 
//	{

//			printf("gnmSWAP %d %d %d %d\n",gnm.r1, gnm.c1, gnm.r2, gnm.c2 );
//		}
//	}
	
		}
	//if i can bullshit 




}

//repeatedly grabs the board and checks and analyzes
void Superball::get_2_it()
{
	int i,j, myid, tsid;
//cout<<"in get 2 it\n";
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
			//		look_bk(board[myid], myid ,tsid ,i ,j);
				}

			}
		}
//cout<<"made it\n";
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
//cout<<"in the constructor\n";
	int mxpts = 0;

	pm.points = 0;
	pm.size = 0;
	em.points = 0;
	em.size = 0;
	gm.size = 0;
	gm.points = 0;
	gnm.size = 0;
	gnm.points = 0;
	scm.size = 0;
	scm.points = 0;
	esm.size = 0;
	psm.size = 0;
	esm.points = 0;
	psm.points = 0;

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

		clrs.push_back( argv[4][i] );
		
		pts_mp[2+i] = argv[4][i] ;							//store this color and its points

		colors[toupper(argv[4][i])] = 2+i;					//store the upper case version and its points
	}

	c_size = clrs.size() ;
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
//cout<<"in the look back\n";
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
			//		set_mp[tsid][1] = set_mp[sidup][1];

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
			//		set_mp[sidup][1]  = set_mp[tsid][1];

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
				//look_bk( goals[upid],upid,nwsid,i-1,j);
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
			//		set_mp[tsid][1] = set_mp[siddwn][1];;

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
			//		set_mp[siddwn][1] = set_mp[tsid][1];
					
					//if it is a goal caryy its stuff over
					if(mit->second[0] )
					{
					//	set_mp[tsid][0] = 1;	//set it as a goal set

						set_mp[tsid][2] = mit->second[2]; //save this goals row value
						set_mp[tsid][3] = mit->second[3]; //save this goals column value
					}

					//now remove it
					set_mp.erase(siddwn);
				}


			}
//				look_bk( goals[dwnid],dwnid,nwsid,i,j-1) ;
		}

	}


}


void Superball::analyze_superball(int r1, int c1, int r2, int c2)
{
	//	prnt_mp(set_mp,0);
//cout<<"1 in the analyze\n";


//printf("\n\nthe color vector's size is %d\n\n", c_size);
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

	int cpts ;

//	int sid;

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
				if(counter < 6 )
				{
			//	printf("  Size: %2d Char: %c Scoring Cell: %d,%d\n",mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ; 
				}

				//get this scoring set set id
				sid = DJS->Find(mit->first);

				scrng.insert(sid);

				vh.push_back(mit->second[2] );        //get the row of this move
				vh.push_back(mit->second[3] );		  //get the column of this move

				pts += mit->second[1] * colors[ board[mit->first] ];  //get the total points for this move

				//save the heighest scoring move
				if(scm.points < mit->second[1] * colors[ board[mit->first] ] )
				{
					scm.points = mit->second[1] * colors[ board[ mit->first] ] ;
					scm.size  = mit->second[1] ;
					scm.r1 = r1;
					scm.c1 = c1;
					scm.r2 = r2;
					scm.c2 = c2;
					scm.scores.clear();
					scm.scores.push_back( mit->second[2]   ) ;
					scm.scores.push_back( mit->second[3]  ) ;

				}
//cout<<"2 in analyze\n";

				//save the move that makes the most blocks disappear
				//if its size is greater than the one stored replace it
				if(esm.points < mit->second[1] )
				{
					
					esm.points = mit->second[1] * colors[ board[ mit->first] ] ;
					esm.size  = mit->second[1] ;
					esm.r1 = r1;
					esm.c1 = c1;
					esm.r2 = r2;
					esm.c2 = c2;
					esm.scores.clear();
					esm.scores.push_back( mit->second[2]   ) ;
					esm.scores.push_back( mit->second[3]  ) ;
				}
				cnt += mit->second[1];     //get the total number of boxes removed
			}
			else		//if it is a goal but doesnt meet the minimun count it for the number of near goals and 
			{

				//insert the set id in the set
				nrsc.insert( DJS->Find( mit->first ) );
			}
//cout<<"ma ma ma\n\n";
		}

		//NON GOAL SAVE
		//if not a goal and not an empty
		else if(board[mit->first] != '.' && board[mit->first]  != '*' && !(mit->second[0]) )
		{
			//get the the id of this : mit->first
			//its color : board[ mit->first ]
			//look for it in the set and get its size and store it in that part of the gm_map
			nsc.insert( DJS->Find( mit->first ) );
		
		}
	}

//	counter++;
//cout<<"\n";
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

//cout<<"\n";
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
		pm.scores = vh;						//can get the number of scores 
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

/*
	//now go through the near scoring and non scoring sets
	for(sit = nrsc.begin(); sit != nrsc.end()  ; sit++)
	{
		mit = set_mp.find( *sit );				//get iterator to this set id

		//get the size of this set
		nrnm += mit->second[1];						//add up the total number of boxes in almost goals

		nrpts += mit->second[1] * colors[ board[ *sit ] ];		//calculate the possible points for this swap

	}
*/
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
//cout<<"5 in analyze\n";	

/*
//go through the no score set counting the number of the different colored blocks
	for(sit = nsc.begin() ; sit != nsc.end() ; sit++)
	{
		mit = set_mp.find( *sit );		//get an iterator to this id in the map

//		cout<<"int nearscor\n";
		cout<<"the size of nmcl is: "<<nmcl.size()<<"\n";
		printf("the index is %d.\n", colors[ board[ mit->first  ]  ]  ) ;
		nmcl[ colors[ board[ mit->first  ]  ] - 2 ] += mit->second[1];
//		cout<<"2int nearscor\n";
	
	}
*/

	int c_brk = 0; 
//cout<<"6 in analyze\n";
	//rank the board 
	for(i = 0 ; i < nmcl.size(); i ++)
	{
//		cout<<"in rank\n";
		c_brk += nmcl[ i ] * (i + 2) ;  //rank = num colors * pt value

		nsnm += nmcl[i]; 
//		cout<<"2in rank\n";
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
int gsid;

map<int,vector<int> > holderm;
map<int, vector<int> >::iterator  hit;

	//now print the scoring set
//	if(counter < 5 )
//	{
/*
      for(sit = scrng.begin(); sit != scrng.end();sit++)
		{
			mit = set_mp.find(*sit);		//find this set id

			gsid = DJS->Find(mit->first);
			if(goals[*sit] )
			{
//				printf("setid: %d Size: %2d Char: %c Scoring Cell: %d,%d\n",gsid,mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ; 
			}
//

			hit = holderm.find(*sit);
			if(hit != holderm.end() )
			{
				//if it is in the map compare the size
				if(mit->second[1] > hit->second[1] )
				{
					holderm[*sit][0] = mit->second[1];
					holderm[*sit][1]  = mit->second[2];
					holderm[*sit][2]  = mit->second[3];
				}
				
			}
			else
			{
				holderm[*sit].resize(3,0);

				holderm[*sit][0] = hit->second[1];
				cout<<"   "<<hit->second[1];
				holderm[*sit][1] = hit->second[2];
				holderm[*sit][2] = hit->second[3];
			}


//			cout<<"the sit is \n"<<*sit<<"\n";;

			

		}

		*/////
//	}
/*

	for(hit = holderm.begin() ; hit != holderm.end() ;hit++)
	{
		

		printf("sid: %d Sze: %2d Chr: %c Scng Cell: %d, %d\n",hit->first,hit->second[0], board[hit->first], hit->second[1], hit->second[2] ) ; 
	}
*/

	//		cout<<"3in rank\n";
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

