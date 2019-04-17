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
////A score or a Swap.
////scoring block. If it is a move
class move
{
	public:
		int r1;				//the 1st blocks row for this move
		int r2;				//the 2nd blocks row for this move

		int c1;				//the  1st blocks column for this move
		int c2;				//the 2nd blocks column for this move

		int size;			//the size of the set this move removes /number of blocks removed

		int points;			//the possible points for this move

		vector <int> scores;	//holds the possible scoreing block for this move
		

		map <int,vector <int> >nm_clrs;	//will hold the numbers of the sets of indivdual colors keyed on point value

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
		
		void analyze_superball(int r1, int c1,int r2,int c2);			//will analyze the given move
		
		void analyze_superballB();			//will analyze the current board and decide if scoring is warrented
		
		void look_bk(int &color, int &myid, int &tsid, int i, int j);		//looks above and back for same color
																			//and if found does union

		void Get_1st();														//gets the first move it can and plays if
		
		void check_move();													//checks all possible moves
		
		void get_2_it();													//reads the board and starts the process
		
		void prnt_mp( map <int,vector<int> > &mp, int is_scrng);            //prints all or part of the set map
		map <int,vector<int> > goals_mp;	//will hold the coordinates to all goals for distance checking/
		map <int,vector<int> > set_mp;		//will hold a set id as a key and information about that set in a vector
		map <int,int > pts_mp;				//will hold the points as a key and the coler as a val

		move em;							//a move to try and empty the board
		move pm;							//a move to try and score the most points
		move gm;							//a move to do when no scores can be made but are almost possible
		move gnm;							//a move to do when no scores can be made
	    move scm;							//a score with the objective of getting the most points
		move esm;							//a score with the objective of emptying the board
		
		void set_disjoint();				//sets up disjoint sets based on the board and adjacent colors

		vector<int> clrs;		//will hold the char values of the "color" blocks

		int counter;
		int c_size;             //how many colors there are

		void prnt_bd();			//will print the current board

		int mxpt;

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

		printf("SWAP %d %d %d %d\n",r1,c1,r2,c2);
}


//will print the current board---used for testing
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


//will set up the disjoint sets based on adjacent same colored blocks
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

			if(block != '*' && block != '.')
			{

				//look above and behind this block for blocks of the same color
				look_bk(block,myid,sid,i,j);
		    }
		}
	}
}


//Will check all possible moves and act accordingly based on the available options 
void Superball::check_move()
{
	int i,j,k,l,p1,p2,p1r,p1c,p2r,p2c,tmp,c1,r1,c2,r2;
	
	counter = 0;

	int pts;

	int mvslft = empty / 5;								//estimate the number of turns you have left

	map <int,vector<move *> >::iterator m_it;		    		


	//if have less than 5 boxes after the next move end the game
	if(mvslft < 1 )
	{
		Get_1st();
	}

	else		//go through the board analyzing all possible moves
	{
		//first look at the current board and see if i can score
		set_disjoint();

		analyze_superballB();
//cout<<"There must be no scores\n";


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
							
								//set the disjoint set with this configuration
								set_disjoint();
								
//cout<<"";								//analyze board at this state
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
//printf("the max size set is: %d and the points are: %d\n",esm.size,esm.points);	
	int rw = 0; 
	int cl = 0;

	int ro = 0;
	int co = 0;
/*
//cout<<"here\n\n";	
cout<<"the number of moves left is: "<<mvslft<<"\n";
cout<<"the empty score moves size is  is: "<<esm.scores.size()<<"\n";
printf("the  scm size: %d, its points : %d\n",esm.size, esm.points);

cout<<"the score  moves size is  is: "<<scm.scores.size()<<"\n";
printf("the  scm size: %d, its points : %d\n",scm.size, scm.points);
*/

/*
for(i = 0; i < scm.scores.size();i++)
{
	cout<<scm.scores[i]<<"\n\n";
}
*/
	
//cout<<" pm's scores map is
	//if need to score
		
		//if no scores possible 
		if(mvslft >= 5 ) 
		{
	
			//if you have some scoring possible for for that
				if(pm.size > 0 && mvslft > 3)
				{
				//go for a move that will hopefully lead to more pointsnts

				printf("SWAP %d %d %d %d\n",pm.r1, pm.c1, pm.r2, pm.c2 );

			}
			else		//just go big
			{
				
				printf("SWAP %d %d %d %d\n",gm.r1, gm.c1, gm.r2, gm.c2 );

			}

		}
		else		//otherwise focus on emptying the board
		{	

			printf("SWAP %d %d %d %d\n",gnm.r1, gnm.c1,gnm.r2, gnm.c2 );
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
	//	while(1)
//	{
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
//	}
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
	esm.points = 0;

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
		tsid = DJS->Find(myid);				//get the true set id of the calling function
		upid = (i-1)*c + j;					//get the id of whats above you

		clrup = board[ upid ];				//get the color (char value)  of whats above you 

		sidup = DJS->Find(upid);			//get the set id of whats above me

		//if they have the same set leave them alone
		//		if(sidup == tsid) return;

		//if they share a color connect them
		if(clrup == color && sidup != tsid)
		{
			//perform the union and get the new set id
			nwsid = DJS->Union(sidup,tsid);

			//Check to see if the new set id is the old set id of whats above
			//if so grab all the stuff from myid and  tsid before removing
			if(nwsid == sidup)
			{
				//look for tsid in map
				mit = set_mp.find(tsid);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];			//get the size of myid's old set id

					set_mp[sidup][1] += cnt;		//add it to the size of the new sid which is the old up sid
			//		set_mp[tsid][1] = set_mp[sidup][1];

					//if  tsid is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[sidup][0] = 1;	//set it as a goal set

						set_mp[sidup][2] = mit->second[2]; //save this goal value
						set_mp[sidup][3] = mit->second[3]; //save this goal value
					}
					//check for myid to be a goal

					//now remove it
				set_mp.erase(tsid);
				}

			}

			//otherwise then tsid is the new set id so get all the stuff from 
			//sidup idup
			else
			{
				//look for sidup in map
				mit = set_mp.find(sidup);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];			//get this size

					set_mp[tsid][1] += cnt;			//add it to the total
			//		set_mp[sidup][1]  = set_mp[tsid][1];

					//if sidup is a goal caryy its stuff over
					if( mit->second[0] )
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
//					tsid = nwsid;
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
			if(nwsid == siddwn)   //get tsid and myid's stuff
			{
				//look for tsid in map
				mit = set_mp.find(tsid);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];				//get the size of tsid

					set_mp[siddwn][1] += cnt;			// add it to the size of the new set id which is the old down's set id
			//		set_mp[tsid][1] = set_mp[siddwn][1];;

					//if tsid(the passing elements old set id ) is a goal cary its stuff over
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

			//otherwise the new set id is myid's tsid so get all the stuff off did siddwn before removing
			else
			{
				//look for siddwn in map
				mit = set_mp.find(siddwn);

				//if its in the map get its info and remove it
				if(mit != set_mp.end() ) 
				{
					cnt = mit->second[1];			//get the size of downs set id

					set_mp[tsid][1] += cnt;			//add it to the new sid's size
			//		set_mp[siddwn][1] = set_mp[tsid][1];
					
					//if it's set id  is a goal caryy its stuff over
					if(mit->second[0] )
					{
						set_mp[tsid][0] = 1;	//set it as a goal set

						set_mp[tsid][2] = mit->second[2]; //save this goals row value
						set_mp[tsid][3] = mit->second[3]; //save this goals column value
					}

					//otherwise try to see if the old down was a goal

//					//now remove it
					set_mp.erase(siddwn);
				}


			}
//				look_bk( goals[dwnid],dwnid,nwsid,i,j-1) ;

			//		tsid = nwsid;
		}

	}


}

void Superball::analyze_superballB()
{
	int mxst, i,k;

	mxst = 0;

	map <int, vector<int> >::iterator mit;

//	cout<<"Scoring sets:\n";

	for(mit = set_mp.begin(); mit != set_mp.end() ; mit++)
	{
		if(board[mit->first] != '.' && board[mit->first]  != '*' && mit->second[0] )
		{
			if(mit->second[1] >= mss )
			{
	//			printf("Size: %2d Char: %c Scoring Cell: %d,%d\n",mit->second[1], board[mit->first], mit->second[2], mit->second[3] ) ;

				//save the heighest scoring move this one just wants the most total points
				if(scm.points < mit->second[1] * colors[ board[mit->first] ] )
				{
					scm.points = mit->second[1] * colors[ board[ mit->first] ] ;
					scm.size  = mit->second[1] ;
					scm.scores.clear();
					scm.scores.push_back( mit->second[2]   ) ;
					scm.scores.push_back( mit->second[3]  ) ;

				}
				

				//grab the sizes and look for the max, wants to make the most blocks 
				//dissapear
				if(mit->second[1] >= esm.size )
				{
					esm.size = mit->second[1];
					esm.points = colors[ board[mit->first] ];
					esm.scores.clear();
					esm.scores.push_back(mit->second[2]);
					esm.scores.push_back(mit->second[3]);

				}
			}
		}
	}

	//if there are possible scores check em out
	if(esm.size > 0)
	{
		//if i have less than four moves just score
		if( empty/5 < 3)
		{
			printf("SCORE %d %d\n",esm.scores[0], esm.scores[1]);
			exit(0);
		}
		//otherwise score if i can get at least five empty slots
		else if(esm.size >= 5)
		{

			printf("SCORE %d %d\n",scm.scores[0], scm.scores[1]);
			exit(0);
		}
	}



}


//this one will analyze the board for the next move
void Superball::analyze_superball(int r1, int c1, int r2, int c2)
{
	//	prnt_mp(set_mp,0);
//	cout<<"1 in the analyze\n";


	//printf("\n\nthe color vector's size is %d\n\n", c_size);
	move * Mvp;

	vector<int> c_cnt(256,0);		//will be used to count sets of each color
	vector <int> vh;

	vector <int> nmcl( c_size, 0 );

	//	move em;						//a move to make the board more empty, done when moves are low

	//	move pm;						//done when moves are high

	int i,j,elmnt,p1,p2,myid,sid, cnt ,tgoal,tnum, nsnm, pts, nrnm, nrpts, gpts;
	nrpts = 0;
	map <int, vector<int>  >::iterator mit; 	
	map <int, vector<move *>  >::iterator mvit;
	vector <int> bg_ng(4,0);
	set <int>::iterator sit;
	cnt = 0;
	//	cout<<"\n\n";
	pts = 0;
	tnum = 0;

	int val= 0;

	int mxst = 0;

	int cpts ;
	nrpts = 0;
	//	int sid;

	//move through the set mp looking for scoreable blocks
	//and store the total points and the total number of possible blocks to remove
	//it the id was part of a scoring set it was removed so cant look for it
	//if can look for it do so and look around it for scoring sets or blocks
	for(mit = set_mp.begin(); mit != set_mp.end() ; mit++)
	{
		//look for possible scoring sets made
		if(board[mit->first] != '.' && board[mit->first]  != '*' && goals[mit->first] )
		{
			//cout<<"the size is\n"<<mit->second[1]; 
			//if the score meets the minimum check and store if better
			if(mit->second[1] >= mss )
			{
			}
			else		//if it is a goal but doesnt meet the minimun count it for the number of near goals and 
			{
//			cout<<"bluuuuueeeee\n";	
				//look for the most scoreing potential  looks for goal blocks under minimum
				if(pm.size <= mit->second[1] && (colors[ board[mit->first] ] * mit->second[1]) > pm.points  )
				{
					pm.size = mit->second[1];
					pm.points = colors[board[mit->first] ] * mit->second[1] ;
					pm.r1=r1;
					pm.c1=c1;
					pm.r2=r2;
					pm.c2=c2;
				}

			
			
			}
		}

		//NON GOAL SAVE
		//if not a goal and not an empty
		//look for the largest set in this move use this to score the board
		else if(board[mit->first] != '.' && board[mit->first]  != '*' && !(goals[mit->first] ) )
		{

//			cout<<"make\n";
			if(mxst < mit->second[1]) mxst = mit->second[1];		//if its larger save it
			
			//if this set value is bigger than this part of c_cnt save iti
			//wants to find the largest set of each color
			if(c_cnt[ board[mit->first ] ] < mit->second[1] )
			{
				c_cnt[ board[mit->first  ] ] = mit->second[1];
//				printf("it is %c and has %d in it",board[mit->first ] ,c_cnt[ board[ mit->first] ] );
			}

		}
	}


	int score = 0;

	//now score the board based on the number 
	for(i = 0; i < c_cnt.size();i++)
	{	
			if(c_cnt[i] != 0)
			{
				score += c_cnt[i] * colors[i]; 	

			}
	}
	
	//cout<<"the max is: "<<mxst<<"\n";
	//	counter++;
	//cout<<"\n";
	/*
	*/
	//now if this number is greater than the number store store this as the general move
	if(score > gm.points)
	{

		gm.points = score;
		gm.size   = 0; 
		gm.r1     = r1;
		gm.c1     = c1;
		gm.r2     = r2;
		gm.c2     = c2;

	}
	//cout<<"the gnm is: "<<gnm.size<<"\n";
	//if this moves board's moves makes the maximum sized set save it
	//think about value
	if( mxst > gnm.size)
	{
		gnm.size = mxst;
		gnm.points = 0;
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

