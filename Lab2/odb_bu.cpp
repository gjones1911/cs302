
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <set>
#include <cstdlib>
#include <sstream>

using namespace std;



vector<string>  stovec( string strs)
{

	stringstream ss;

	string shldr;

	vector<string> rv;

	ss.clear();
	ss.str(strs);

	while(ss>>shldr)
	{
	rv.push_back(shldr);

	}

	return rv;
}

//creates a instance representing a ball
class Ball
{
	public:
		int val;
		Ball * high;	//will be used to point to the ball in the second pic that is a possibility for the ball to move higher
		Ball * low;		//will be used to point to the ball in the second pic that is a possibility for the ball to move lower
};


class OneDimensionalBalls
{
	public:
		long long countValidGuesses(vector<int> &firstPic, vector<int> &secondPic );	//will set up the neccessarry vectors full of balls 
		//and use the cvg() function to count valid guesses

		long long cvg(int &velocity);														//will count the valid guesses for the given velocity 																 																			
		void set_balls();										//will set up the F/Sp_ptr vectors

		void set_ptrs(int velocity);														//will set up the pointers from FP to Sp and vice/versa

		void print();

		void printptr();

		vector <int> get_vs();								//will calculate the possible vectors for guessing

		vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP

		vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP


		vector <int> fpo;
		vector <int> spo;

};


void OneDimensionalBalls::printptr()
{
	int i,j;
	Ball * FBP;
	Ball * SBP;
	Ball * hbp;
	Ball * lbp;
	
	cout<<"FP:";
	
	for(i = 0; i < FP_ptr.size() ; i++)
	{
		FBP = FP_ptr.at(i);
		hbp = FBP->high;
		lbp = FBP->low;

		if(hbp == NULL && lbp == NULL)
		{
			printf("FP Ball %d is %d and its h/l pointers NULL\n",i+1,FBP->val); 
		}
		else if(hbp == NULL && lbp != NULL)
		{
			printf("FP Ball %d is %d and its h/l pointers NULL and %d\n",i+1,FBP->val,lbp->val); 
		}
		else if(hbp != NULL && lbp == NULL)
		{
			printf("FP Ball %d is %d and its h/l pointers %d and null\n",i+1,FBP->val,hbp->val); 
		}
		else 
		{
			printf("FP Ball %d is %d and its h/l pointers %d and %d\n",i+1,FBP->val,hbp->val,lbp->val); 
		}
	}
	
	
	cout<<"SP:";
	for(i = 0; i < SP_ptr.size() ; i++)
	{
		SBP = SP_ptr.at(i);
		hbp = SBP->high;
		lbp = SBP->low;

		if(hbp == NULL && lbp == NULL)
		{
			printf("SP Ball %d is %d and its h/l pointers NULL\n",i+1,SBP->val); 
		}
		else if(hbp == NULL && lbp != NULL)
		{
			printf("SP Ball %d is %d and its h/l pointers NULL and %d\n",i+1,SBP->val,lbp->val); 
		}
		else if(hbp != NULL && lbp == NULL)
		{
			printf("SP Ball %d is %d and its h/l pointers %d and null\n",i+1,SBP->val,hbp->val); 
		}
		else 
		{
			printf("SP Ball %d is %d and its h/l pointers %d and %d\n",i+1,SBP->val,hbp->val,lbp->val); 
		}
		

	}

}





vector <int> OneDimensionalBalls::get_vs()								//will calculate the possible vectors for guessing
{
	int i,j,k,num, sum,d;

	vector < vector <int> >  diff;							//will hold the difference values for FP-SP

	diff.resize(fpo.size() );

	set <int> vels;

	set <int>::iterator vit;

	vector <int> rvec;

	//create the diff vector
	for( i = 0 ; i < fpo.size(); i++)
	{
		for( j = 0; j < spo.size(); j++)
		{
			d = fpo.at(i) - spo.at(j);        //get the difference

			diff.at(i).push_back( d );      //put the difference in the vector
		}
	}


/////DB--17------Difference printer----------////////	
/*

	//db--print out the diff vector
	cout <<"The supposed difference vector is:\n";

	for(i = 0; i < diff.size();i++)
	{
		for( j = 0; j < diff.at(i).size(); j++)
		{
			cout << diff.at(i).at(j) <<" ";
		}

		cout <<"\n";
	}

*/
////DB--17-------------------------------------------	
	
	
	//find valid velocitys
	for(i = 0; i < diff.at(0).size(); i++)
	{

		if(diff.at(0).at(i) != 0)
		{

			for(int r = 1; r < diff.size(); r++)
			{   
				for( int c = 0; c < diff.at(0).size(); c++)
				{

					if(abs( diff.at(0).at(i) ) == abs( diff.at(r).at(c) ) )
					{
						if(r == diff.size() - 1 )
						{
							d = diff.at(r).at(c);

							if( d < 0 ) d = -d;

							vels.insert( d );

							c = diff.at(0).size();
						}
						else
						{
							c = diff.at(0).size();
						}   
					}
				}
			}
		}
	}

	cout<<"the possible velocitys are:\n";

	for(vit = vels.begin(); vit != vels.end(); vit++)
	{
		cout << *vit <<" ";
	}
	cout<<"\n";

	//set up the return vector


	for(vit = vels.begin(); vit != vels.end(); vit++)
	{
		rvec.push_back( (*vit ) );;
	}

	return rvec;
}


long long OneDimensionalBalls::countValidGuesses(vector<int> &firstPicture, vector<int> &secondPicture)
{

	///////////cntvld varss----
	int i , d, j;

	long long rv = 0;           //will be used to return the number of guesses

	string shld;

	stringstream ss;

	vector <int> vels;

	Ball * BP;                  //will be used to grab pointers to ball instances

	fpo = firstPicture;         //will hold the original firstpicture 
	spo = secondPicture;		//will hold the original secondpicture

	//DB    vector <Ball *> FP_ptr;     //will hold pointers to the balls in FP
	//DB    vector <Ball *> SP_ptr;     //will hold pointers to the balls in SP


	vector <vector<int> > diff; //will hold the differences of the FP values and the SP values

	diff.resize( fpo.size() );


	set <int>::iterator vit;    //will be used to iterate through the velocity's set

	//set the values for the ball objects
	set_balls();

	print();

	//calculate the possible velocity values and store them in the vels vector
	vels = get_vs();

	i = 0;
	
	//set the pointers for the ball objects in FP to point to their possible positions in the SP vector
	set_ptrs(vels.at(i));

//DB --print the pointers to check
	printptr();




}




void OneDimensionalBalls::print()
{
	int i ;

	Ball * FBP;
	Ball * SBP;

	cout<<"FP:";
	for(i = 0; i < FP_ptr.size(); i++)
	{
		FBP = FP_ptr.at(i);

		cout<<FBP->val<<" ";
	}

	cout<<"\n";

	cout<<"SFP:";
	for(i = 0; i < SP_ptr.size(); i++)
	{
		SBP = SP_ptr.at(i);

		cout<<SBP->val<<" ";
	}

	cout<<"\n";
}


//will set up the F/Sp_ptr vectors
void OneDimensionalBalls::set_balls()
{

	//FP_ptr.clear();			//clear the vector

	//FP_ptr.clear();			//clear the vector

	Ball * BP;

	int i;

	//set up the firstpicture ball pointer vector
	for( i = 0; i < fpo.size() ; i++ )
	{
		BP = new Ball;		//create a new ball 

		BP->val =  fpo.at(i);	//set the balls value
		BP->low = NULL;					//set the balls low to NULL 
		BP->high = NULL;				//set the balls high to NULL

		FP_ptr.push_back(BP);
	}	



	//set up the secondpicture ball pointer vector
	for( i = 0; i < spo.size() ; i++ )
	{
		BP = new Ball;		//create a new ball 

		BP->val =  spo.at(i);	//set the balls value
		BP->low = NULL;					//set the balls low to NULL 
		BP->high = NULL;				//set the balls high to NULL


		SP_ptr.push_back(BP);			//store the ball pointer in the vector
	}

}


//will set up the pointers from FP to Sp and vice/versa
void OneDimensionalBalls::set_ptrs( int velocity)
{

	int i, j, val, num, cnt; 
	Ball * FBP;
	Ball * SBP;



	for(i = 0; i < FP_ptr.size(); i++)
	{
		for( j = -1 ; j <= 1 ; j += 2 )
		{
			FBP = FP_ptr.at(i);
			val = FBP->val + velocity * j;


			for(cnt = 0; cnt < SP_ptr.size(); cnt++)
			{
				SBP = SP_ptr.at(cnt);      
				num = SBP->val;


				if(val == num)
				{
					if(j < 0)
					{
						FBP->low = SBP;
						SBP->high = FBP;
					}
					else
					{
						FBP->high = SBP;
						SBP->low = FBP;
					}
				}
			}
		}

	}


}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cvg( ) ---  will count the valid guesses for the velocity given
///////////////////////////////////////////////////////////////////////////////////////////
long long OneDimensionalBalls::cvg( int &velocity)
{
	//varscvg
	int i, j ,cnt,val,num;


	vector< Ball *> FPr = FP_ptr;	//make a copy for manipulation
	vector< Ball *> SPr = SP_ptr;	//make a copy for manipulation

	vector< Ball *>::iterator P_it;	//will be used to iterate through pictures

	vector< Ball *>::iterator ED_it;	//a iterator that represents the current end of the vector

	Ball * FBP;

	Ball * SBP;

	Ball * HBP;		//pointer to a ball's high

	Ball * LBP;		//pointer to a ball's low

	Ball * GBP;		//pointer to a general ball

	Ball Bhld;

	
	cout<<"the velocity is:"<<velocity<<"\n";




	//DB check that the pointers are set up correctly	
	//go through FPr and look at the balls pointers
	for(i = 0; i < FPr.size(); i++)
	{
		FBP = FPr.at(i);

		//show what the val of FPR[i]
		printf("the val of ball %d is %d\n",i,FBP->val);

		//show the high of current ball
		HBP = FBP->high;	//get the high of this ball

		LBP = FBP->low;		//get the low of this ball

		//print the high and low---
		//
		//check the high ball and it's low pointer
		if(HBP != NULL)
		{
			printf("the high of ball %d is %d\n",i,HBP->val);

			//get the low of this SP ball
			GBP = HBP->low;

			if(GBP != NULL)
			{
				//print the high balls low  to check
				printf("the high  balls low is %d \n",FBP->val);

			}
			else
			{
				cout<<"this high sp balls low is null\n";
			}
		}
		else
		{
			cout<<"the fp balls high is null\n";
		}


		//check the low ball and it's high pointer
		if(LBP != NULL)
		{
			printf("the low of ball %d is %d\n",i,LBP->val);

			//get the fp ball's low's high
			GBP = LBP->high;


			if(GBP != NULL)
			{
				//print this balls low
				printf("the low  balls high is %d \n",GBP->val);

			}
			else
			{
				cout<<"this low SP ball's high is null\n";
			}
		}
		else
		{
			cout<<"the fp balls low is null\n";
		}


	}


	/////////////////////////////////////////////////////////////////////////////////////////////////
	// look through both FPr and SPr vector looking for balls with no pointers and removing them/
	/////////////////////////////////////////////////////////////////////////////////////////////////
	cout<<"make it it it \n";
	//print out the pointers for SP for checking
	for(i = 0; i < SPr.size();i++)
	{


		SBP = SPr.at(i);


		//look for a non connected bottom ball and remove it/print it
		if( SBP->high == NULL && SBP->low == NULL)
		{
			printf("Ball %d is %d and it's h/l are Null\n", i,SBP->val);

		}
		else if(SBP->high == NULL)
		{
			printf("Ball %d is %d and it's h/l are Null and %d\n", i,SBP->val,SBP->low->val);
		}
		else if(SBP->low == NULL)
		{
			printf("Ball %d is %d and it's h/l are %d and NULL\n", i,SBP->val,SBP->high->val);
		}
		else
		{
			printf("Ball %d is %d and it's h/l are %d and %d\n", i,SBP->val,SBP->high->val,SBP->low->val);
		}

	}


	//DB--62 lines above	


	//DB387---------------------------------------------------		
	//DB390-------------------------------------------
	/*
	   for(i = 0; i < FPr.size(); i++)
	   {
	   FBP = FPr.at(i);

	//show what the val of FPR[i]
	//DB	printf("the val of ball %d is %d\n",i,FBP->val);

	//show the high of current ball
	HBP = FBP->high;	//get the high of this ball

	LBP = FBP->low;		//get the low of this ball

	//check for Null on high side and low side
	//if found remove ball from FPr
	if(HBP == NULL)
	{

	//if so check for null on low side
	//if found given be is not a solution so return 0 guesses
	if(LBP == NULL)
	{
	return 0;

	}
	//remove the ball with only one pointer
	else
	{
	//remove the pointer to the ball in FPr

	for(P_it = FPr.begin();P_it != FPr.end();P_it++)
	{	//if you find the first ball pointer
	//remove it
	if(*P_it == FBP)
	{

	FPr.erase(P_it);
	P_it = FPr.end();
	}
	}


	//remove the balls from SPr
	for(P_it == SPr.begin() ; P_it != SPr.end();P_it++)
	{
	if(*P_it == LBP)
	{
	SPr.erase(P_it);
	P_it = SPr.end();
	}
	}

	for(P_it == SPr.begin() ; P_it != SPr.end();P_it++)
	{
	if(*P_it == HBP)
	{
	SPr.erase(P_it);
	P_it = SPr.end();
	}
	}
	}

	}

	//if it gets here the HBP was not null
	//check for a low == null
	//if found remove ball due to one null pointer
	else if(LBP == NULL)
	{

	//remove the pointer to the ball in FPr

	for(P_it = FPr.begin();P_it != FPr.end();P_it++)
	{	//if you find the first ball pointer
		//remove it
		if(*P_it == FBP)
		{

			FPr.erase(P_it);
			P_it = FPr.end();
		}
	}


	//remove the balls from SPr
	for(P_it == SPr.begin() ; P_it != SPr.end();P_it++)
	{
		if(*P_it == LBP)
		{
			SPr.erase(P_it);
			P_it = SPr.end();
		}
	}

	for(P_it == SPr.begin() ; P_it != SPr.end();P_it++)
	{
		if(*P_it == HBP)
		{
			SPr.erase(P_it);
			P_it = SPr.end();
		}
	}

}

//if it gets the ball is double pointed

}


//remove gaps in SPr
int size = SPr.size();	
for(i = 0; i < SPr.size(); i++)
{
	//check the balls High and low
	SBP = SPr.at(i);		//grab pointer i
	HBP = SBP->high;
	LBP = SBP->low;

	if(HBP == NULL && LBP == NULL)
	{
		for(j = 0; j < SPr.size() ; j++)
		{


			cout<<"it gets here\n";

			GBP = SPr.at(j);

			GBP->val = -1;

			//	if((*P_it) == SBP)
			//	{

			cout<<"The ball number is: \n"<<num<<"\n";

			//		SPr.erase(P_it);
			//		P_it = SPr.end() - 1;
			size--;
			//	}
			//			else
			//			{
			cout<<"the SPBwas not found\n";
			//			}
		}
	}

}
//DB
//print out the adjusted FPr and SPr balls
//

cout<<"The FPr contains :\n";

for(i = 0 ; i < FPr.size(); i++)
{
	FBP = FPr.at(i);

	cout<<FBP->val<<"\n";
}

cout<<"The SPr contains :\n";

for(i = 0 ; i < SPr.size(); i++)
{
	SBP = SPr.at(i);

	cout<<SBP->val<<"\n";
}

*/
return 0;
}

//////themain/////////////////////
int main(int argc, char **argv)
{


	//create instance of OneDimensionalBalls class
	OneDimensionalBalls  ODB;



	vector <int> fp ;

	vector <int> sp ;

	for(int i = 2 ; i < 7; i +=2)
	{
		fp.push_back(i);
	}

	for(int i = 1 ; i < 9; i++)
	{

		if(i != 6)
		{
			sp.push_back(i);
		}
	}


	long long num = ODB.countValidGuesses(fp, sp); 


	return 0;
}
