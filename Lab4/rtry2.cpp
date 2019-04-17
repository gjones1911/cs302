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
		void Dsk_enm(int idx, unsigned int m_dwn);		//creates a list of  m disks to take down, uses the options given in disks vector 
		void Sctr_enm2(int idx, unsigned int s_dwn);  
		vector <unsigned int> Get_PCM_Col(int col);		
		void Set_wrds();								//sets the word vector 
		void Set_pcm();
		void Prnt_fail();
		unsigned int G_math(unsigned int ak, unsigned int ni,Galois_Field * gf);
		unsigned int nd;								//repesents the number of disks
		unsigned int rd;								//repesents the number of rows on each disk
		unsigned int md;								//repesents the number of disks that will be missing on each sweep
		unsigned int sd;								//repesents the number of words that will be missing on each sweep
		unsigned int wd;								//repesents the number of words that will be missing on each sweep
		vector <unsigned int> S_coef;
		vector <unsigned int> disk;						//represents the system as a set of 0->n-1 disk indexes
		vector <unsigned int> words;					//represents the system as a set of 0-n-1 word indexes 
		set <unsigned int> Dwn_Dsk;					//represents the system as a set of 0-n-1 word indexes 
		vector <vector<unsigned int> > pcm;					    //will hold the parity check matrix;	
		vector <unsigned int> Dgrp;
		vector <unsigned int> Sgrp;
		string fail;
		Galois_Field * mgf;
		Incremental_Inverse * ic_invp;


};

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


//sets the word vector, and tries all current disk words to see if it is invertable so far 
void enm::Set_wrds()
{
	int i,col,mul;

	set <unsigned int>::iterator d_it;

	vector <unsigned int> h_v;

	int quest;

	int rows = (int)(md * rd + sd);
	ic_invp = new Incremental_Inverse(rows,mgf);
	Dwn_Dsk.clear();

	words.resize(0);

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

//TRY------now try to load theese into the matrix and see if it is invertable so far
	
	for(d_it = Dwn_Dsk.begin() ; d_it != Dwn_Dsk.end();d_it++)
	{
		h_v.resize(0);
		
		h_v = Get_PCM_Col( (*d_it) );

		//set this part of fail
		fail[ (*d_it) ] = 'D';

		//ask if this current matrix is invertable
		quest = ic_invp->Add_Col( h_v );

		if(quest == 0)
		{

			Prnt_fail();
			exit(0);
		}
	}

	//now set up the words vector
	for( col = 0; col < nd*rd;col++)
	{
		d_it = Dwn_Dsk.find(col);

		if(d_it == Dwn_Dsk.end())
		{
			words.push_back(col);	
		}
	}

}


void enm::Sctr_enm2(int idx, unsigned int s_dwn)
{
	vector<unsigned int> hldv;
	int quest, i;

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


	if( s_dwn > words.size() - idx )
	{
		return;
	}
	
	
	Sgrp.push_back( words[idx] );

	
	Sctr_enm2(idx+1,s_dwn-1);

	Sgrp.pop_back();

	Sctr_enm2(idx+1,s_dwn);

}



//creates a list of  m disks to take down, uses the options given in disks vector 
void enm::Dsk_enm(int idx, unsigned int m_dwn)
{

	set <unsigned int>::iterator d_it;

	if(m_dwn == 0)
	{
		Set_wrds();

		Sctr_enm2( 0,sd );

		//if it got here it triad all combos with these disks down so remove them
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



void enm::Set_pcm()
{
		
	int i, j, k, x,a_k,c,num;
	
	pcm.resize( md*rd+sd);

	for(i = 0; i < pcm.size();i++)
	{
		pcm.at(i).resize(nd*rd, 0);
	}


	
	for(x = 0; x < md*rd; x++)
	{
		i = x % rd;
		k = x / rd;
		a_k = S_coef.at(k);
		
		for( c = i * nd ; c < i*nd + nd; c++)
		{
			num = G_math(a_k,c,mgf);
			pcm.at(x).at(c) = num;
		}

	}


//set up the bottom rows of the pcm
	for(i = 0; i < sd; i++)
	{
		a_k = S_coef.at(md+i);
		for(j = 0;j < nd*rd; j++)
		{ 

			pcm[ md*rd + i][j] = G_math(a_k,j,mgf);
		}
	}
/*
//DB--print the pcm
	cout<<"The PCM is:------------------\n";
    cout<<"   0   1   2  3  4  5  6  7  8  9  10   11   12   13   14   15   16   17   18   19   20   21   22   23\n" ;
	for(i = 0 ; i < pcm.size();i++)
	{
		cout<<"the "<<i<<"th row is size:"<<pcm.size()<<"  \n";
		for(j = 0; j < pcm.at(i).size(); j++)
		{
			cout<<" "<<pcm[i][j]<<" ";
		}

		cout<<"\n";
	}
*/


}

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


void enm::prt_mp_elmnt(int i )
{
	int j,k;
	vector <vector<unsigned int> > * vp;

	map <int, vector <vector<unsigned int> > * >::iterator  enm_it;

	vector <vector<unsigned int> >::iterator v_it;
	cout<<"The code  of the map is:"<<i<<"\n";
	for(enm_it = enm_list.begin() ; enm_it !=  enm_list.end() ; enm_it++)
	{

		//get the it disk set up
		if(enm_it->first == i)
		{
			vp = enm_it->second;

			for(j = 0; j < vp->size(); j++)
			{
				printf("\n");
				cout<<" "<<j<<":";
				cout<<"the size of this list is: "<<vp->size()<<"\n\n";
				for(k = 0; k < vp->at(j).size() ; k++)
				{
					cout<<" "<<vp->at(j).at(k);
				}

				cout<<"\n";

			}

		}
	}
}

void enm::prt_e_v2()
{
	int i,j;
	cout<<"size of ev2:"<<e_v2.size()<<"\n";
	for(i = 0; i < e_v2.size();i++)
	{
		cout<<" "<<i<<":";
		for(j = 0; j < e_v2[i].size(); j++)
		{
			cout<<" "<<e_v2[i][j];
		}

		cout<<"\n";
	}

}

void enm::prt_e_v()
{
	int i,j;
	cout<<"size of ev:"<<e_v.size()<<"\n";
	for(i = 0; i < e_v.size();i++)
	{
		for(j = 0; j < e_v[i].size(); j++)
		{
			cout<<" "<<e_v[i][j];
		}

		cout<<"\n";
	}

}

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
/*
	//will be used to make the words list vector
	//will be used to create the enumeration 2 vector
	for(i = 0; i < ttl_wrds; i++)
	{
		words.push_back(i);
	}
*/

	//set up fail string
	fail = string(nd*rd,'U');

	
	Set_pcm();

	Dsk_enm(0,md);

	//set up the e_v vector of enumeration lists
//	Get_enm(0,m);
	//get the word vector that we will enumerate
//	Set_wrdV();

	//uses Get_enm2 to create the map that will be used in conjunction with e_v to enumerate through
	//the combinations of disks and sector failures
//	Make_list_map();





//db--p	prt_mp_elmnt( 0  );

}

void enm::Get_enm(int idx, unsigned int m_dwn)
{
	if(m_dwn == 0)
	{
		//			cout<<"adding the following group\n";
		//			for(int i =0;i < grp.size();i++)
		//			{
		//				cout<<" "<<grp[i];
		//			}
		//			cout<<"\n";
		e_v.push_back(grp);
		//			cout<<"added a group\n";
		return;

	}

	//		cout<<"here it gets idx: "<<idx<<"\n";
	//		cout<<"here it gets d size: "<<disk.size()<<"\n";
	//		cout<<"here it gets d m_dwn: "<<m_dwn<<"\n";


	if(m_dwn > disk.size() - idx) return;
	//		if(idx == disk.size()) return;

	grp.push_back( disk[idx] );



	Get_enm(idx+1,m_dwn-1);

	grp.pop_back();

	Get_enm(idx+1,m_dwn);
}


//creates the map used in conjunction with e_v to iterate through combos of disks and words down
void enm::Get_enm2(int idx, unsigned int s_dwn, int code)
{


	//	vector < vector<unsigned int> > r_v;			//holds a vector lists of words to take down
	//    vector<unsigned int> grp2;						//holds a vector of words to take down

	//go through e_v2 creating a vector of vectors representing this combination of 2 words to take down

	if(s_dwn == 0)
	{
		//			cout<<"adding the following group\n";
		//			for(int i =0;i < grp.size();i++)
		//			{
		//				cout<<" "<<grp[i];
		//			}
		//			cout<<"\n";
		r_vp->push_back(grp2);
		//			cout<<"added a group\n";
		return;

	}

	//		cout<<"here it gets idx: "<<idx<<"\n";
	//		cout<<"here it gets d size: "<<disk.size()<<"\n";
	//		cout<<"here it gets d m_dwn: "<<m_dwn<<"\n";


	if(s_dwn > e_v2.at(code).size() - idx) return;
	//		if(idx == disk.size()) return;

	grp2.push_back( e_v2.at(code).at(idx) );



	Get_enm2(idx+1,s_dwn-1,code);

	grp2.pop_back();

	Get_enm2(idx+1,s_dwn,code);


}


//will get the number of combinations needed
long long  comb(unsigned int n, unsigned int  k)
{
	long long rv=1;

	//find minimum
	long long mmin = min(n-k,k);

	for(long long  i = 1; i <= mmin; i++)
	{
		rv *= (n - mmin+i );

		rv /= i;

	}

	return rv;
}


//calculated ak ^ ni using galois multiplication, provided by Dr. Plank
unsigned int enm::G_math(unsigned int ak, unsigned int ni,Galois_Field * gf)
{
	unsigned int rv = 1;

	//	cout<<"ni:"<<ni<<"\n";

	if(ni == 0) return 1;
	if(ni == 1) return ak;

	for(int m = 1; m <= ni; m++)
	{
		rv = gf->Multiply(rv,ak);
	}

//	cout<<"rv:"<<rv<<"\n";

	return rv;
}


int main(int argc, char** argv)
{
	unsigned int n, m, s, w,i,j,c,mul,r,x,k,a_k;

	unsigned int row, col,num,pwr;
	Galois_Field * gf;

	enm * ENM;

	long long dcomb;
	long long wcomb;
	long long wrds;
	
	int g = 0;

	vector <unsigned int> co_v;
	vector < vector<unsigned int> > PCM;

	//grab command line args----------------ADD ERROR CHECKING
	n = atoi( argv[ 1 ] ); 
	m = atoi( argv[ 2 ] );
	s = atoi( argv[ 3 ] );
	r = atoi( argv[ 4 ] );
	w = atoi( argv[ 5 ] );

	//create a new instance of a Galois Field and use w (given from command line)  
	gf = new Galois_Field(w);	

//DB---	//get neccessary number of combinations of disks
	dcomb = comb(n,m);
	
//--	//get neccessary number of word left when m disks are subtractedwords
	wrds = r * (n-m); 

	wcomb = comb(wrds,s);



//	cout<<"there are "<<dcomb<<" disk combos and "<<wcomb<<" word combos\n"; 


//	ENM->Get_enm(0,m);
//	ENM->prt_e_v();
//	ENM->prt_e_v2();

	//grab the coefficients
	for(i = 0; i < m + s ; i++)
	{
		co_v.push_back( atoi ( argv[6 + i] ) );
	}

//DB the below code just prints all the command line args
/*
	cout<<" n: "<<n<<"\n"
		<<" m: "<<m<<"\n"
		<<" s: "<<s<<"\n"
		<<" r: "<<r<<"\n"
		<<" w: "<<w<<"\n";
*/
/*
	cout<<"the coefficient vector is:\n";

	for(i = 0; i < co_v.size(); i++)
	{
		cout<<" "<<co_v.at(i)<<"\n";
	}
*/

	//make parity check matrix  of size mr+s, nr
//	PCM.resize( m*r+s);

//	for(i = 0; i < PCM.size();i++)
//	{
//		PCM.at(i).resize(n*r, 0);
//	}

//	cout<<"the size of PCM is: "<<PCM.size()<<" x "<<PCM.at(0).size()<<"\n";

	/*
	//DB
	for(i = 0; i < PCM.size();i++)
	{
	for(j = 0; j < PCM.at(0).size();j++)
	{
	cout<<" "<<PCM[i][j];
	}

	cout<<"\n";

	}
	*/
/*
	for(x = 0; x < m*r; x++)
	{
		i = x % r;
		k = x / r;
		a_k = co_v.at(k);
		pwr = i * n;
//		cout<<"a_k:"<<a_k<<"\n";
		for( c = i * n ; c < i*n + n; c++)
		{
			//num = G_math(a_k,c,gf);
			PCM.at(x).at(c) = num;
		}

	}

*/
/*

//set up the bottom rows of the pcm
	for(i = 0; i < s; i++)
	{
		row = m*r +i;
		a_k = co_v.at(m+i);
		for(j = 0;j < n*r; j++)
		{ 

	//		PCM[ m*r + i][j] = G_math(a_k,j,gf);
		}
	}
*/



//DB print the pcm
//
/*
	for(i = 0; i < PCM.size();i++)
	{
		for(j = 0; j < PCM.at(0).size();j++)
		{
			cout<<" "<<PCM[i][j];
		}

		cout<<"\n";

	}
*/

	ENM = new enm(w,m,n,s,r,co_v);
//	ENM->Do_disk_enumeration(PCM, gf);

	cout<<"OK\n";

	return 0;
}
