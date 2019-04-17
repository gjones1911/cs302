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
		vector < vector<unsigned int> > e_v;			//holds a vector of disks to take down
		vector < vector<unsigned int> > e_v2;			//holds a variation of the system with each set of m disks down
		vector < vector<unsigned int> > * r_vp;			//holds a vector lists of words to take down
		vector<unsigned int>  grp;						//used to hold a set of m disks to take down
		vector<unsigned int>  grp2;						//used to hold a set of m disks to take down
		void Get_enm(int idx, unsigned int m_dwn);		//creates a list of  m disks to take down, uses the options given in disks vector 
		void Dsk_enm(int idx, unsigned int m_dwn);		//creates a list of  m disks to take down, uses the options given in disks vector 
		void Get_enm2(int idx, unsigned int s_dwn, int i);  
		void Sctr_enm2(int idx, unsigned int s_dwn);  
		vector <unsigned int> Get_PCM_Col(int col);		
		void Make_list_map();
		void Set_wrdV();								//sets the word vector 
		void Set_wrds();								//sets the word vector 
		void Do_disk_enumeration( vector <vector<unsigned int> > PCM, Galois_Field * gf);
		void Set_pcm();
		void prt_e_v();
		void Prnt_fail();
		void prt_e_v2();
		void prt_mp_elmnt(int i );
//		int G_math();
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
		string faiil;
		vector <unsigned int> Dgrp;
		vector <unsigned int> Sgrp;
		string fail;
		Galois_Field * mgf;
		Incremental_Inverse * ic_invp;
		map <int, vector <vector<unsigned int> > * > enm_list;


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
//sets the word vector 
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

/*
//DB print the word vector
	cout<<"The Dgrp is are wwwwwwwwwwwwwwwwwwwwwww\n\n";
	for(i = 0; i < Dgrp.size();i++)
	{
		cout<<" "<<Dgrp[i];
	}

	cout<<"\n\n";

	cout<<"the Dwn disk  is now :----\n\n";
	for(d_it = Dwn_Dsk.begin() ; d_it != Dwn_Dsk.end();d_it++)
	{
		cout<<" "<<(*d_it)<<" ";
	}
	cout<<"--------------------------\n\n";

*/
//TRY------now try to load theese into the matrix and see if it is invertable so far
	
	for(d_it = Dwn_Dsk.begin() ; d_it != Dwn_Dsk.end();d_it++)
	{
		h_v.resize(0);
		
		h_v = Get_PCM_Col( (*d_it) );
/*
//DB		cout<<"the Disk colum being tried is:"<<(*d_it)<<"\n";
		for(i = 0;i < h_v.size(); i++)
		{
			cout<<" "<<h_v[i];
		}
		cout<<"\n";
*/
		//set this part of fail
		fail[ (*d_it) ] = 'D';

		quest = ic_invp->Add_Col( h_v );

		if(quest == 0)
		{

			Prnt_fail();
			exit(0);
		}

/*		
		if(   Dwn_Dsk.begin() == d_it ) 
		{
			cout<<"the hv is:\n\n";
			for(i = 0; i < h_v.size();i++)
			{
				cout<<" "<<h_v[i];
			}
			cout<<"\n\n";
		}
*/		
	}

/*
	
*/
	//now set up the words vector
	for( col = 0; col < nd*rd;col++)
	{
		d_it = Dwn_Dsk.find(col);

		if(d_it == Dwn_Dsk.end())
		{
			words.push_back(col);	
		}
	}
/*
//print words vec
	cout<<"The Words left  are : wwwwwwwwwwwwwwwwwwwwwww\n\n";
	for(i = 0; i < words.size();i++)
	{
		cout<<" "<<words[i];
	}

	cout<<"\n\n";
*/

}


void enm::Sctr_enm2(int idx, unsigned int s_dwn)
{
	vector<unsigned int> hldv;
	int quest, i;

	if(s_dwn == 0)
	{
//		cout<<"The Sgrp is: ----------------\n";
		for( i = 0 ; i < Sgrp.size();i++)
		{
			hldv.resize(0);
//			cout<<" "<<Sgrp[i];

//			hldv = Get_PCM_Col( Sgrp[i] ) ;
  
//			fail[ Sgrp[i] ] = 'S';
//			quest = ic_invp->Add_Col(hldv);
			
			
//			cout<<" the Sector colum being tried is:"<<Sgrp[i];

			fail[ Sgrp[i] ] = 'S'; 

			hldv = Get_PCM_Col( Sgrp[i] ) ;
		
			
			quest = ic_invp->Add_Col(hldv);
			
//			for(i = 0;i < hldv.size(); i++)
//			{
//				cout<<" "<<hldv[i];
//			}
//			cout<<"\n";

			if(quest == 0)
			{
				Prnt_fail();
				exit(0);
			}
			
			
		}
        
//		cout<<"\nthe size of SGrp:"<<Sgrp.size();"\n\n";
		
		//if it got here I need to reset the fail string to its original state
		for(i = 0; i <  Sgrp.size() ;i++)
		{
			ic_invp->Delete_Col();

			fail[ Sgrp[i] ] = 'U';
		}

//		Sgrp.resize(0);	

		return;
	}


	if( s_dwn > words.size() - idx )
	{
		return;
	}
	
	
	Sgrp.push_back( words[idx] );

	
//	cout<<"it got here and idx:"<<idx<<"\n\n";
	
	
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
//sets up the map that will be used in conjunction  with e_v to iterate through the different combinations of n total disks and of m disks and s words down
//the map entries are keyed to the e_v entry that corresponds to that set of disks down. i.e. if the number of disks down "m"= 2, and number of disks "n"= 6.
//
//e_v[0][0] = 1 0 == this means disk 0 and disk 1 are down: code = 0;
//--this will correspond with the 0 coded map entry that has every group of s words left once the 0th set of disks removed
void enm::Make_list_map()
{
	int i, j, k, l;
 
	vector <vector<unsigned int> > * vvp;
	for(i = 0; i < e_v2.size();i++)
	{
		
		r_vp = new vector < vector<unsigned int> >;
		
		Get_enm2( 0,sd,i);



		enm_list.insert( make_pair(i,r_vp) );

		grp2.resize(0);

	}


	cout<<"_________________________________________________\n\n";
	cout<<"the size of list map is :"<<enm_list.size()<<"\n";
	cout<<"_________________________________________________\n\n";

}


//sets up the e_v2 that represents the disk as a set 0 indexed words that are without the words used when two disks 
//are down
void enm::Set_wrdV()
{
	unsigned int evr,evc,wi,i,Dc, mul;
	vector<unsigned int> hv;
	bool is_there = false; 
	
/*
	for(i = 0; i < words.size();i++)
	{
		cout<<words[i]<<" ";
		
	}
	
	cout<<"\n";
*/



	//every row of ev
	for(evr = 0 ; evr < e_v.size(); evr++)
	{

		//end every entry in words 
		for(wi = 0; wi < words.size();wi++)
		{
			is_there = false;
			//look at each entry in e_v and if it is wi word set the bool 
			for(evc = 0; evc < e_v.at(evr).size() ;evc++)
			{	
				Dc = e_v.at(evr).at(evc);
				for(mul = 1; mul < rd ; mul++)
				{
					if( words.at( wi ) == Dc || (wi == ( (nd * mul) + Dc)  ) )
					{
						is_there = true;

					}
				}
//				//if it not there push it onto the holder vector
//				if( is_there == false ) hv.push_back( words.at(wi) );


			}
				//if it not there push it onto the holder vector
				if( is_there == false ) hv.push_back( words.at(wi) );
		}

		//once here push the holder vector onto ev2
		e_v2.push_back(hv);
		hv.resize(0);
	}
}


void enm::Do_disk_enumeration( vector <vector<unsigned int> > PCM, Galois_Field * gf)
{
	unsigned int evr, evc, Dc,wi,prw,mul;

	bool need_special = false;

	int i, j ,r,add_rslt;
	vector<unsigned int> try_v;
	vector<unsigned int> rm_set;
	map <int, vector <vector<unsigned int> > * >::iterator emp_it;

	vector <vector<unsigned int> > * vvp;

	set <unsigned int> try_st;
	set <unsigned int>::iterator s_it;

	int rows = (int)(md * rd + sd);

	Incremental_Inverse  ic_inv(rows,gf);

	//for every row in the e_v 
	for(evr = 0; evr < e_v.size();evr++)
	{
		//fail = string(nd*rd,'U');
		//go through this e_v row's columns
		for(evc = 0; evc < e_v.at(evr).size(); evc ++)
		{
			Dc = e_v[evr][evc];

			//go through the columns of the PCM looking at the column number
			//and comparing it to the m values stored in the e_v and thier possible multiples
			for(wi = 0; wi < PCM.at(0).size(); wi++)
			{
				for(mul = 1; mul < rd;mul++)
				{
					if(wi == Dc || wi == (mul*nd) + Dc)
					{
			    //		for(prw = 0; prw < PCM.size();prw++)
				//		{
				//		}
				
						try_st.insert(wi);
					//	fail.at(wi) = 'D';
						//cout<<" "<<wi<<"was inserted\n";

					}
				}
			}

		}

		
cout<<"The evr code is: "<<evr<<"\n";
		//now load sets of s sector failures from the map coded at evr
		//emp_it = enm_list.find(evr);
		 if(enm_list.find(evr) != enm_list.end() )vvp = enm_list.find(evr)->second;
		
		 for(r = 0; r < vvp->size(); r++)
		 {
cout<<"the size of the "<< r <<"th row of the vp here is:"<< vvp->at(r).size()<<"\n";
//The bug is around here			 
			
			 for( j = 0 ; j < vvp->at(0).size();j++)
			 {   cout<<"the vp element is : "<<vvp->at(r).at(j)<<"\n";
				 rm_set.push_back(vvp->at(r).at(j));
				 fail[vvp->at(r).at(j)] = 'S';
				 try_st.insert(vvp->at(r).at(j) );
			 }

			 //after a word set is loaded try it on the matrix

			 		

			 //DB print the try set
			 //		cout<<"The set is: \n";
			 for(s_it = try_st.begin(); s_it != try_st.end(); s_it++)
			 {
				 //			cout<<" "<<*s_it;
				 unsigned int col_I = *s_it;

				 cout<<" the try is: "<<col_I<<"\n";
				 for(i = 0; i < PCM.size();i++)
				 {
					 try_v.push_back( PCM[i][col_I] );
				 }

				 //cout<<" try's size: "<<try_v.size()<<"\n";

				 add_rslt = ic_inv.Add_Col(try_v);
//fail state-------
				 if(add_rslt == 0)
				 {
					 cout<<"No: ";
                     for(int fi = 0; fi < fail.size();fi++)
					 {
						cout<<fail[fi];
					 }
					 cout<<"\n";
					 exit(0);
				 }

				 try_v.resize(0);
				 //switch the s's back to u's
				 for(int fi = 0; fi < rm_set.size();fi++)
				 {
					fail[rm_set[fi]] = 'U';
				 }
			 }
			 // if it gets here clear the supposed invertable matrix
			 for(i = 0 ; i < md*rd + sd;i++)
			 {
				ic_inv.Delete_Col();
			 }

			 //now i need to remove the s things I added from the vvp
			 for(i = 0; i < rm_set.size(); i++)
			 {
				try_st.erase(rm_set.at(i) );
			 }
		 }
	}

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
