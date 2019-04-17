
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
/////setvel
	i = 3;
///SETPTR--	
	//set the pointers for the ball objects in FP to point to their possible positions in the SP vector
	set_ptrs(vels.at(i));

//DB --print the pointers to check
	printptr();


	return cvg( vels.at(i) );

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

	bool done = false;

	int count = 1;


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

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// look through both FPr and SPr vector looking for balls with no pointers and removing them/
	/////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	//look through SP for balls with no pointers and remove them( make their val = -1)
	for(i = 0; i < SPr.size();i++)
	{


		SBP = SPr.at(i);


		//look for a non connected bottom ball and remove it/print it
		if( SBP->high == NULL && SBP->low == NULL)
		{
			SBP->val = -1;

		}
	}


	//go through FP looking for balls with : a)no pointers and if so no guesses can be made
	//										 b)only one pointer and if so set its val to -1	
	
	while( !(done) )
	{
		for(i = 0; i < FP_ptr.size(); i++)
		{
			FBP = FP_ptr.at(i);

			// if a ball with no pointers is found end the run and return 0, no guesses can be made
			if(FBP->low == NULL && FBP->high == NULL)
			{
				cout<<"no guesses can be made\n";
				
				return 0;
			}
			else if( (FBP->low != NULL && FBP->high == NULL) || (FBP->low == NULL && FBP->high != NULL) )
			{
				FBP->val = -1;

				if(FBP->low == NULL)
				{
					
					FBP->high->val = -1;
//					FBP->high->low = NULL;
//					FBP->high->high->low = NULL;
//					FBP->high->high = NULL;
//					FBP->high = NULL;
				}
				else if(FBP->high == NULL)
				{
					FBP->low->val = -1;
//					FBP->low = NULL;
				}
			}

		}
        
		int test = 0;

		for(i = 0;i < FP_ptr.size(); i++)
		{
			if(FP_ptr.at(i)->val != -1) test = 1;
		}
		
		if(test != 1)
		{
			cout<<"the answer is one\n";
			
			return 1;
		}
		done = true;

		//check for only double pointers
		for(i = 0; i < FP_ptr.size(); i++)
		{
			if(FP_ptr.at(i)->val != -1)
			{
				if(FP_ptr.at(i)->low->val == -1 || FP_ptr.at(i)->high->val == -1)
				{

					done = false;

				}
			}
		}
	}



	//check the ball values
	print();

//DB	printptr();

	//now loop through chains counting
	//
	while(1)
	{
		for(i = 0; i < FP_ptr.size(); i++)
		{
			FBP = FP_ptr.at(i);

			
			
			if(FBP->val != -1)
			{
				if(FBP->high->high != NULL)
				{
					//go to next ball if there is one
					if( FBP->high->high->val == -1 )
					{
						count++;
						FBP->val-1;
					}
				}
				else 
				{
					count++;
					FBP->val-1;
				}

			}
		}

	}



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
