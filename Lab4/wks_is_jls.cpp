
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: is_jls.cpp                                                                          //
//Created By: Gerald Jones                                                                     //
//Purpose:   This is Lab 4 for cs302. It taks in some command line agruments in the form:      //
//           n,m,s,r,w,a0,...,am+s-1 where:                                                    //
//           n = total number of disks in the memory system                                    //
//           m = the number of disks that will be down during each test                        //
//           s = the number of sectors or words that will be down during each test             //
//           w = is the bit with of the words                                                  //
//           a0,....,am+s-1: are the coefficients that will be used to create the parity check //
//                           matrix.                                                           // 
//           The program will enumerate through every combination of m disks and s words and   //
//           check to see if the matrix created from thess is invertable. This simulates a set //
//           of disks going down, and if the matrix is invertable the information on the disks //
//           can be recovered If all combo's are invertable it prints "OK" to standard output, //
//           and if one of the combo's  does not create an invertable matrix "No:" is printed  //
//           followed by a string representing a possible disk configuration that leads to a   //
//           non invertable matrix or in other words a non recoverable situation.              //
//           U = the disk in that colum of the PCM is up                                       //
//           D = the disk in that colum of the PCM is down                                     //
//           S = the sector or word  in that colum of the PCM is down                          //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <set>
#include <cstdio>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
#include "galois_field.h"
#include "incremental_inverse.h"
#include <map>
using namespace std;

class enm
{
	public:
		enm( unsigned int &wd,unsigned int &m,unsigned int &n, unsigned int &s, unsigned &r, vector <unsigned int> &s_coef);
		
		void Dsk_enm(int idx, unsigned int m_dwn);		//Enumerates through the nd disks in sets of md  disks down 
		
		void Sctr_enm2(int idx, unsigned int s_dwn);    //enumerates through all possible sector down combinations of sd sectors with md disks down
		
		vector <unsigned int> Get_PCM_Col(int col);		//returns the given column of the parity check matrix
		
		void Set_wrds();								//sets the word vector 
		
		void Set_pcm();									//sets up the parity check matrix
		
		void Prnt_fail();
		
		unsigned int G_pwr(unsigned int ak, unsigned int ni,Galois_Field * gf);		//this performs ak^ni using Galois multiplication
		
		unsigned int nd;								//repesents the number of disks
		
		unsigned int rd;								//repesents the number of rows on each disk
		
		unsigned int md;								//repesents the number of disks that will be missing on each sweep
		
		unsigned int sd;								//repesents the number of words that will be missing on each sweep
		
		unsigned int wd;								//repesents the size of the words: either 8, 16,32
		
		vector <unsigned int> S_coef;					//coefficients given on the command line , there must be m+s total
		
		vector <unsigned int> disk;						//represents the system as a set of 0->n-1 disk indexes
		
		vector <unsigned int> words;					//represents the system as a set of 0->n-1 word indexes 
		
		set <unsigned int> Dwn_Dsk;					    //represents the disks that are down for that enumeration  
		
		vector <vector<unsigned int> > pcm;				//will hold the parity check matrix;	
		
		vector <unsigned int> Dgrp;						//will hold a set of m disks to be taken down
		
		vector <unsigned int> Sgrp;						//will hold a set of s words to be taken down
		
		string fail;									//will represent a un recoverable failure state
		
		Galois_Field * mgf;								//used for galois field operations 
		
		Incremental_Inverse * ic_invp;					//used to incrementally check for the invertability of a matrix


};

//This function just returns the given column of the parity check matrix as a vector
vector <unsigned int>  enm::Get_PCM_Col(int col)		
{	
	int i;
	vector <unsigned int> h_v;

	for(i = 0 ; i < pcm.size(); i++)
	{
		h_v.push_back(pcm [ i ] [ (col) ] );
	}

	return h_v;

}


//This uses a set of m disks to set up the word vector that represents the set of words left when m disks are taken down
//and tries all current downed disk  to see if it is invertable so far. 
void enm::Set_wrds()
{
	int i,col,mul;

	set <unsigned int>::iterator d_it;

	vector <unsigned int> h_v;

	int quest;										//this represents the question of if the matrix created so far is invertable

	int rows = (int)(md * rd + sd);
	
	ic_invp = new Incremental_Inverse(rows,mgf);
	
	Dwn_Dsk.clear();

	words.resize(0);

	
	//The below goes through and looks for every word belonging to a certain set of disk and stores  
	//each word of the set of disks that will be considered taken down
	for( i = 0; i < Dgrp.size();i++)
	{
		for(  col = Dgrp[i] + 1; col < nd*rd; col++)
		{
			for( mul = 0 ; mul < rd; mul++)
			{
				if( col == Dgrp[i] + mul * nd )
				{
					Dwn_Dsk.insert( col ); 
				}
			}

		}

		Dwn_Dsk.insert(Dgrp[i]);
	}

	
	//Below each disk that is considered taken down is placed in the matrix column by column and 
	//using the Add_Col() method the resulting matrix is checked for invertability. If it is found
	//to be invertable the enumeration stops and outputs a variation of a system that leads to an 
	//invertable matrix, otherwise the program moves on to check all combintaions with this set of disks down
	//and all possible sector's down. As it goes it creates the fail string to be printed in case of 
	//an non invertable matrix

	for(d_it = Dwn_Dsk.begin() ; d_it != Dwn_Dsk.end();d_it++)
	{
		h_v.resize(0);
		
		h_v = Get_PCM_Col( (*d_it) );

		fail[ (*d_it) ] = 'D';

		quest = ic_invp->Add_Col( h_v );

		if(quest == 0)
		{

			Prnt_fail();
			exit(0);
		}
	}

	//now set up the words vector that will represent the possible "s" sectors down combinations
	for( col = 0; col < nd*rd;col++)
	{
		d_it = Dwn_Dsk.find(col);

		if(d_it == Dwn_Dsk.end())
		{
			words.push_back(col);	
		}
	}

}



//This method eneumerates through all combinations of 's' sectors and 'm' disks down checking the 
//invertability of the matrix created from the words there in
void enm::Sctr_enm2(int idx, unsigned int s_dwn)
{
	vector<unsigned int> hldv;
	int quest, i;

	//when you have gotten the correct number of elements (s) try this
	//group
	if(s_dwn == 0)
	{
		for( i = 0 ; i < Sgrp.size();i++)
		{
			hldv.resize(0);
			
			fail[ Sgrp[i] ] = 'S'; 

			hldv = Get_PCM_Col( Sgrp[i] ) ;
		
			
			quest = ic_invp->Add_Col(hldv);
			

			if(quest == 0)
			{
				Prnt_fail();
				exit(0);
			}
		}
        
		//if it got here I need to reset the fail string to its original state
		for(i = 0; i <  Sgrp.size() ;i++)
		{
			ic_invp->Delete_Col();

			fail[ Sgrp[i] ] = 'U';
		}

		return;
	}


	//if you have more elements to get than you have elements return
	if( s_dwn > words.size() - idx )
	{
		return;
	}
	
	
	//store the ith element in words
	Sgrp.push_back( words[idx] );


	//do an enumeration with the ith element and the rest
	Sctr_enm2(idx+1,s_dwn-1);

	//remove the ith element
	Sgrp.pop_back();

	//do an enumeration without the ith element
	Sctr_enm2(idx+1,s_dwn);

}



//The below method will enumerate through all combo's of 'm' disks, simulating these disks being down. 
//It creates a grouping of these disks to be used by the Sctr_enm method
void enm::Dsk_enm(int idx, unsigned int m_dwn)
{

	set <unsigned int>::iterator d_it;

	if(m_dwn == 0)
	{
		Set_wrds();						//set the words that will be left without this grouping of m disks

		Sctr_enm2( 0,sd );				//enumerate through all combos of s sectors with this set of m disks

		//if it got here it tried all combos with these disks down so remove them
		for(d_it = Dwn_Dsk.begin(); d_it != Dwn_Dsk.end();d_it++)
		{
			fail[ (*d_it) ] = 'U';

			ic_invp->Delete_Col();
		}

		return;
	}


	if( m_dwn > disk.size() - idx ) return;

	Dgrp.push_back( disk[idx] );

	Dsk_enm(idx+1,m_dwn-1);

	Dgrp.pop_back();

	Dsk_enm(idx+1,m_dwn);

}


//This method sets up the Parity check matrix using the given command line arguments
void enm::Set_pcm()
{
		
	int i, j, k, x,a_k,c,num;
	
	pcm.resize( md*rd+sd);

	for(i = 0; i < pcm.size();i++)
	{
		pcm.at(i).resize(nd*rd, 0);
	}


	//set up the top m*r rows 
	for(x = 0; x < md*rd; x++)
	{
		i = x % rd;
		k = x / rd;
		a_k = S_coef.at(k);
		
		for( c = i * nd ; c < i*nd + nd; c++)
		{
			num = G_pwr(a_k,c,mgf);
			pcm.at(x).at(c) = num;
		}

	}


	//set up the bottom  s rows of the pcm
	for(i = 0; i < sd; i++)
	{
		a_k = S_coef.at(md+i);
		for(j = 0;j < nd*rd; j++)
		{ 

			pcm[ md*rd + i][j] = G_pwr(a_k,j,mgf);
		}
	}

}


//will print the fail message and a possible non invertable disk set up
void enm::Prnt_fail()
{
	int i ;

	cout<<"N0: ";	
	for(i = 0; i < fail.size();i++)
	{
		cout<<fail[i];
	}
	cout<<"\n";

	exit(0);
}


//The constructor for the enumerator(enm) class, the below code uses the given arguments to set up a parity check matrix
//and enumerate through all possible variations of m disks down and s sectors down making sure each combo can be recoverd
//if not a form of the system that is not recoverable will be outputted as a string
enm::enm(unsigned int &wd,unsigned int &m,unsigned int &n, unsigned int &s, unsigned &r, vector <unsigned int> &s_coef)
{
	unsigned int i;

	unsigned int ttl_wrds = n*r;



	nd = n;
	md = m;
	rd = r;
	sd = s;
	wd = wd;
	S_coef = s_coef;
	mgf = new Galois_Field(wd);	


	//make the disk list vector
	//will be used to create the enumeration vector
	for(i = 0; i < n;i++)
	{
		disk.push_back(i);
	}

	//set up fail string
	fail = string(nd*rd,'U');

	//create the parity check matrix
	Set_pcm();

	//start the enumerations for the m disks
	Dsk_enm(0,md);


}

//calculated ak ^ ni using galois multiplication, provided by Dr. Plank
unsigned int enm::G_pwr(unsigned int ak, unsigned int ni,Galois_Field * gf)
{
	unsigned int rv = 1;


	if(ni == 0) return 1;
	if(ni == 1) return ak;

	for(int m = 1; m <= ni; m++)
	{
		rv = gf->Multiply(rv,ak);
	}

	return rv;
}

//used for error checking , in the case of bad commands the user will get this message
void Usage()
{
			    
		cerr<<"Usage:-- ./ n m s r w a0 ... aM+s\n"
			<<"n = number of disks\n"
			<<"m = number of disks down\n"
			<<"s = number of sector failures\n"
			<<"r = number of rows/words on each disk\n"
			<<"w = word size: 8 16 or 32\n"
			<<"a0...aM+s = coefficients must be m+s of them\n";
			    
		exit(0);

}

//The main takes in the command line arguments in the form :n m s r w a0 -> aM+s
int main(int argc, char** argv)
{
	unsigned int n, m, s, w, i, j, c, mul, r, x, k, a_k, input_constraint;

	unsigned int row, col,num,pwr;
	Galois_Field * gf;

	enm * ENM;

	long long dcomb;
	long long wcomb;
	long long wrds;
	
	int g = 0;

	vector <unsigned int> co_v;
	vector < vector<unsigned int> > PCM;
	
	if(argc < 7 )
	{
		Usage();		    
	}

	//grab command line args----------------ADD ERROR CHECKING
	if( atoi(argv[1])  < 0 ) Usage();
	n = atoi( argv[ 1 ] ); 

	if( atoi(argv[2])  < 0 ) Usage();
	m = atoi( argv[ 2 ] ); if( m < 0 ) Usage();  
	
	if( atoi(argv[3])  < 0 ) Usage();
	s = atoi( argv[ 3 ] ); if( s < 0 ) Usage();
	
	if( atoi(argv[4])  < 0 ) Usage();
	r = atoi( argv[ 4 ] ); if( r < 0 ) Usage();
	
	w = atoi( argv[ 5 ] ); 

	input_constraint = 6 + m + s;


	if( (w != 8 && w != 16 && w != 32) )
	{
		Usage();
	}
	
	//check for the appropriate number of command line arguments
	if(argc < input_constraint || argc > input_constraint )
	{
			   Usage();
	}

	//create a new instance of a Galois Field and use w (given from command line)  
	gf = new Galois_Field(w);	


	//grab the coefficients
	for(i = 0; i < m + s ; i++)
	{
		co_v.push_back( atoi ( argv[6 + i] ) );
	}


	ENM = new enm(w,m,n,s,r,co_v);

	cout<<"OK\n";

	return 0;
}
