
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

using namespace std;


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
	
		long long cvg(int &vel);														//will count the valid guesses for the given velocity 																 																			
																					
	    vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP

		vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP
}



long long OneDimensionlBalls::countValidGuesses(vector<int> &firstPicture, vector<int> &secondPicture)
{
	int i , d, j;

	long long rv = 0;			//will be used to return the number of guesses

	Ball * BP;					//will be used to grab pointers to ball instances

	vector <int> FP = firstPicture;		//will hold the original 
	vector <int> SP = secondPicture;	//will hold the original


//DB	vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP
//DB	vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP


	vector <vector<int> > diff;	//will hold the differences of the FP values and the SP values

	diff.resize( FP.size() );

	set <int> vels ;			//will store the possible legal velocitys 

	set <int>::iterator vit;	//will be used to iterate through the velocity's set

	///////////////////////////////////////////////////////////////////////////////////////////////////
	////The below segment of code will make the ball objects and store them in the appropriate vector//
	///////////////////////////////////////////////////////////////////////////////////////////////////
	
	//set up the firstpicture ball pointer vector
	for( i = 0; i < firstPicture.size() ; i++ )
	{
		BP = new Ball;		//create a new ball 
		
		BP->val =  firstPicture.at(i);	//set the balls value
		BP->low = NULL;					//set the balls low to NULL 
		BP->high = NULL;				//set the balls high to NULL

		FP_ptr.push_back(BP);			//store the ball pointer in the vector
	}	



	//set up the secondpicture ball pointer vector
	for( i = 0; i < secondPicture.size() ; i++ )
	{
		BP = new Ball;		//create a new ball 
		
		BP->val =  secondPicture.at(i);	//set the balls value
		BP->low = NULL;					//set the balls low to NULL 
		BP->high = NULL;				//set the balls high to NULL


		SP_ptr.push_back(BP);			//store the ball pointer in the vector
	}

//db	
//
//db print out the supposed vector val's

	cout<<"The supposed vectors are:\n";

	for(i = 0; i < FP_ptr.size(); i++)
	{
		cout << FP_ptr.at(i)->val <<" ";
	}


	cout<<"\n";
	
	for(i = 0; i < SP_ptr.size(); i++)
	{
		cout << SP_ptr.at(i)->val <<" ";
	}

	cout <<"\n";
//db
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	//find the possible values of the velocitys by getting the differences of the FP values and the SP values//
	///////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	for( i = 0 ; i < FP.size(); i++)
	{
		for( j = 0; j < SP.size(); j++)
		{
			d = FP.at(i) - SP.at(j);		//get the difference

			diff.at(i).push_back( d );		//put the difference in the vector


		}
	}

//db
	cout <<"the differece vector's size is: " << diff.size() <<" : "<< diff.at(0).size() <<"\n"; 

//db/////
	//print out the supposed diff vector
	//
	
	cout <<"The supposed difference vector is:\n";
	

//

	for(i = 0; i < diff.size();i++)
	{
		for( j = 0; j < diff.at(i).size(); j++)
		{
			cout << diff.at(i).at(j) <<" ";
		}

		cout <<"\n";
	}
//db


	///////////////////////////////////////////////////////////////////////////////////////////////
	///// The below segment of code will find all the possible velocity values and store them   ///
	///// in the vels vector. It is a little buggy on how it does the store and compare because ///
	///// it compares the last row by itself and store matches from just the last row. I think  ///
	///// I can work around this in the way i do the actuel guess counting.						///
	///////////////////////////////////////////////////////////////////////////////////////////////
	


	for(i = 0; i < diff.at(0).size(); i++)
	{



		if(diff.at(0).at(i) != 0)
		{

//db			cout <<"the top num is "<<diff.at(0).at(i)<<"\n";
			for(int r = 1; r < diff.size(); r++)
			{	
				for( int c = 0; c < diff.at(0).size(); c++)
				{
			
//db					cout <<"the maybe match is : "<<diff.at(r).at(c) <<"\n";
					
					//check for a match
					if(abs( diff.at(0).at(i) ) == abs( diff.at(r).at(c) ) )
					{
						//check for the end of the vector
						if(r == diff.size() - 1 )
						{
							d = diff.at(r).at(c);
							//get the positive of the given value if need be
							if( d < 0 ) d = -d;

//db							cout<<"the d is:" << d << "\n";
							vels.insert( d );		//store the velocity

							
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
	
//db//
		//print out the velocity vector
		//

		cout<<"the supposed velocitys are:\n";

		for(vit = vels.begin(); vit != vels.end(); vit++)
		{
			cout << *vit <<" ";
		}
		cout<<"\n";

	return rv;


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This block of code will call the cvg() function and calculate the valid guesses for each valid velocity //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	



}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//will count the valid guesses for the velocity given
///////////////////////////////////////////////////////////////////////////////////////////
long long OneDimensionalBalls::cvg( int velocity)
{
	//take velocity add and subtract it from the top row and the values that match are to be connected 
	//by there high and low pointers
	
	
}

//////themain/////////////////////
int main(int argc, char **argv)
{
		
	
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


	long long num = countValidGuesses(fp, sp); 
			
		
	return 0;
}
