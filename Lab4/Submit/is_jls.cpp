
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
//           check to see if the matrix created from these is invertable. This simulates a set //
//           of m disks and s sectors(words) going down, and if the matrix is invertable the   //
//           information on the disks can be recovered. If all combo's are invertable it prints//
//           "OK" to standard output, and if one of the combo's  does not create an invertable //
//           matrix "No:" is printed followed by a string representing a possible disk         //
//           configuration that leads to a  non invertable matrix or in other words a non      //
//           recoverable situation.   The fail string elements are as such:                    //
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

//Can be used to enumerate through a set of m disks and s words/sectors checking for a non recoverable system
class enm
{
	public:
		enm( unsigned int &wd,unsigned int &m,unsigned int &n, unsigned int &s, unsigned &r, vector <unsigned int> &s_coef);
		
		void Dsk_enm(int idx, unsigned int m_dwn);		//Enumerates through the nd disks in sets of md  disks down 
		
		void Sctr_enm2(int idx, unsigned int s_dwn);    //enumerates through all possible sector down combinations of sd sectors with md disks down
		
		vector <unsigned int> Get_PCM_Col(int col);		//returns the given column of the parity check matrix
		
		void Set_wrds();								//sets the word vector based on the contents of the Dwn_dsk set 
		
		void Set_pcm();									//sets up the parity check matrix
		
		void Prnt_fail();								//prints the fail message
		
		unsigned int G_pwr(unsigned int ak, unsigned int ni,Galois_Field * gf);		//this performs ak^ni using Galois multiplication
		
		unsigned int nd;								//repesents the number of disks
		
		unsigned int rd;								//repesents the number of rows on each disk
		
		unsigned int md;								//repesents the number of disks that will be down on each sweep
		
		unsigned int sd;								//repesents the number of words that will be down on each sweep
		
		unsigned int wd;								//repesents the size of the words: either 8, 16,32 bits
		
		vector <unsigned int> S_coef;					//coefficients given on the command line , there must be md+sd total
		
		vector <unsigned int> words;					//represents the system as a set of 0-n->1 word indexes representing the sectors that need to be down
		
		set <unsigned int> Dwn_Dsk;					    //represents the system as a set of 0-n->1 word indexes  
		
		vector <vector<unsigned int> > pcm;				//will hold the parity check matrix;	
		
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
void enm::Set_wrds()
{
	int col;

	set <unsigned int>::iterator d_it;

	words.resize(0);
	
	for( col = 0; col < nd*rd;col++)
	{
		d_it = Dwn_Dsk.find(col);

		if(d_it == Dwn_Dsk.end())
		{
			words.push_back(col);	
		}
	}

}



//This method eneumerates through all combinations of 's' sectors and 'm' disks down incrementally checking the 
//invertability of the matrix created from the words there in
void enm::Sctr_enm2(int idx, unsigned int s_dwn)
{
	vector<unsigned int> hldv;
	int quest, i;

	if(s_dwn == 0)
	{
		//incrementally test this set of words for invertability
		for( i = 0 ; i < Sgrp.size();i++)
		{
			hldv.resize(0);
			
			fail[ Sgrp[i] ] = 'S'; 

			hldv = Get_PCM_Col( Sgrp[i] ) ;
		
			quest = ic_invp->Add_Col(hldv);

			if(quest == 0)
			{
				Prnt_fail();
			}
		}
        
		//if it got here this set of words lead to an invertalbe matrix so  I need to reset the fail string to its original state
		//and remove the tested words from the matrix
		for(i = 0; i <  Sgrp.size() ;i++)
		{
			ic_invp->Delete_Col();

			fail[ Sgrp[i] ] = 'U';
		}

		return;
	}


	//if you have more elements to get than you have elements left return
	if( s_dwn > words.size() - idx )
	{
		return;
	}
	
	
	Sgrp.push_back( words[idx] );

	//do an enumeration with the idx element of words and the rest
	Sctr_enm2(idx+1,s_dwn-1);

	//remove the idx element of words from the group
	Sgrp.pop_back();

	//do an enumeration without the idx element of words and continue enumerating
	Sctr_enm2(idx+1,s_dwn);

}



//The below method will enumerate through all combo's of 'm' disks, simulating these disks being down. 
//It creates a grouping of these disks to incrementally try each in the matrix to test for invertability
//as well as to be used by the Set_wrds method
void enm::Dsk_enm(int idx, unsigned int m_dwn)
{

	int i,j,k;

	set <unsigned int>::iterator d_it;
	vector <unsigned int> h_v;
	int quest;
	

	if(m_dwn == 0)
	{
		//try this set of disks to see if it creates an invertable matrix
		for(d_it = Dwn_Dsk.begin() ; d_it != Dwn_Dsk.end();d_it++)
		{
			h_v.resize(0);

			h_v = Get_PCM_Col( (*d_it) );

			fail[ (*d_it) ] = 'D';

			quest = ic_invp->Add_Col( h_v );

			if(quest == 0)
			{

				Prnt_fail();
			}
		}

		//if if makes it here the matrix is invertable so far so start the word enumeration

		Set_wrds();					

		Sctr_enm2( 0,sd );		

		//if it got here it tried all combos with these disks down so remove them
		for(d_it = Dwn_Dsk.begin(); d_it != Dwn_Dsk.end();d_it++)
		{
			fail[ (*d_it) ] = 'U';             

			ic_invp->Delete_Col();
		}

		return;
	}


	if( m_dwn > nd - idx ) return;



	//insert the current disk index and all columns that belong to this disk
	for( j = 0; j < rd ; j++)
	{
		Dwn_Dsk.insert( idx + nd*j );
	}

	Dsk_enm(idx+1,m_dwn-1);


	//remove the current disk index and all columns that belong to it
	for( j = 0; j < rd ; j++)
	{
		Dwn_Dsk.erase( idx + nd*j );
	}


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


//The below code uses the given arguments to set up a parity check matrix
//and enumerate through all possible variations of m disks down and s sectors down making sure each combo can be recoverd
//if not, a form of the system that is not recoverable will be output as a string on std input
enm::enm(unsigned int &wd,unsigned int &m,unsigned int &n, unsigned int &s, unsigned &r, vector <unsigned int> &s_coef)
{
	nd = n;
	md = m;
	rd = r;
	sd = s;
	wd = wd;
	S_coef = s_coef;
	mgf = new Galois_Field(wd);	

	int rows = (int)(md * rd + sd);
	ic_invp = new Incremental_Inverse(rows,mgf);
	
	fail = string(nd*rd,'U');

	Set_pcm();

	//start the enumerations for the m disks
	Dsk_enm(0,md);
}

//The below method calculates ak ^ ni using galois multiplication, provided by Dr. Plank
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

//The main takes in the command line arguments in the form :n m s r w a0 -> aM+s, does some error checking and does the enumeration for 
//the information and disk set up given
int main(int argc, char** argv)
{
	unsigned int n, m, s, w, i,  r, input_constraint;

	enm * ENM;


	int g = 0;

	vector <unsigned int> co_v;

	if(argc < 7 )
	{
		Usage();		    
	}

	//grab command line args-----do some ERROR CHECKING
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
	if(argc != input_constraint )
	{
		Usage();
	}


	//grab the coefficients
	for(i = 0; i < m + s ; i++)
	{
		co_v.push_back( atoi ( argv[6 + i] ) );
	}


	ENM = new enm(w,m,n,s,r,co_v);

	//if it gets here all combinations have proven to be invertable 
	cout<<"OK\n";

	return 0;
}
