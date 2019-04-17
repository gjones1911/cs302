
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: OneDimensionalBalls.cpp															   //
//Created By: Gerald Jones                                                                     //
//Purpose: This is the first part of Lab 2. It comes from a Top Coder problem. It creates a    //
//			class and a method used to solve this problem. It is used in conjunction with a    //
//			driver program called SRM-496-Driver.cpp supplied to me by DR. plank that will test//
//			my method . The methods will figure out the possible velocitys and count the valid //
//			guesses and return those as  long int. The Idea is that Te vector called FP        //
//			represents a picture of of balls in a one dimensional axis,                        //
//			where the numbers are the coordinates of the balls on some axis. The second vector //
//			is a second picture taken some time later. The second picture possibly contains the// 
//			first balls but moved at some constant velocity from there original coordinates.   //
//			The program will compare the to vectors of coordinates and give you the number of  //
//			possible guesses for positions of the balls in the first "picture" that appear in  //
//			the second picture.																   //	
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <set>
#include <cstdlib>
#include <sstream>

using namespace std;

//creates a instance representing a ball
class Ball
{
	public:
		int val;        //will present the balls position on the one dimensional axis
		Ball * high;	//will be used to point to the ball in the second pic that is a possibility for the ball to move higher on the axis
		Ball * low;		//will be used to point to the ball in the second pic that is a possibility for the ball to move lower on the axis
};

//the class that holds the functions that will solve the problem
class OneDimensionalBalls
{
	public:
		long long countValidGuesses(vector<int> &firstPic, vector<int> &secondPic );	//will set up the neccessarry vectors full of balls 
																						//and use the cvg() function to count valid guesses

		long long cvg(int &velocity);													//will count the valid guesses for the given velocity 																 																			
		void set_balls();																//will set up the FP/SP_ptr vectors

		void set_ptrs(int velocity);													//will set up the pointers from FP to Sp and vice/versa for
																						//the given velocity

		void print();																	//will print the FP and SP vectors , used for debugging

		void printptr();																//will print the FP and SP balls and what they point to


		vector <int> get_vs();															//will calculate the possible vectors for guessing

		vector <Ball *> FP_ptr;															//will hold pointers to the balls in FP

		vector <Ball *> SP_ptr;															//will hold pointers to the balls in SP


		vector <int> fpo;																//will hold the original First picture
		vector <int> spo;																//will hold the original second picture

};

//will print the pointers of each ball from FP and Sp
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


//will calculate the possible velocities and return them as a vector of integers
vector <int> OneDimensionalBalls::get_vs()								
{
	int i,j,k,num, sum,d;

	vector < vector <int> >  diff;							//will hold the difference values for FP-SP

	diff.resize(fpo.size() );

	set <int> vels;

	set <int>::iterator vit;

	vector <int> rvec;					//will be used to return the valid velocitys

	//create a vector of the differences of each ball in FP and each ball in SP
	//these will be the possible velocitys
	for( i = 0 ; i < fpo.size(); i++)
	{
		for( j = 0; j < spo.size(); j++)
		{
			d = fpo.at(i) - spo.at(j);       

			diff.at(i).push_back( d );      
		}
	}
	
	/////////////////////////////////////////////////////////
	//The below segment of code will find valid velocitys////
	/////////////////////////////////////////////////////////
	
	//Start at the top of the difference vector going from row looking for a value equal in magnitude to the current one if found
	//add it to the vels set, the use of a set removes duplicates
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
					else
					{
						if(c == diff.at(0).size() - 1 ) 
						{
							r = diff.size();
						}
					}
				}
			}
		}
	}
	
	for(vit = vels.begin(); vit != vels.end(); vit++)
	{
		rvec.push_back( (*vit ) );;
	}

	return rvec;
}


//will set up FP and SP and call cvg to count the valid guesses for the possible velocitys
long long OneDimensionalBalls::countValidGuesses(vector<int> &firstPicture, vector<int> &secondPicture)
{

	int i , d, j;

	long long rv = 0;           

	vector <int> vels;		

	fpo = firstPicture;         //Store the original firstpicture 
	spo = secondPicture;		//Store the original secondpicture

	//if there is only one ball the number of guesses is the number of balls in the second picture 
	//so return that
	if(fpo.size() == 1)
	{
		return spo.size();
	}

	vector <vector<int> > diff; //will hold the differences of the FP values and the SP values

	diff.resize( fpo.size() );


	set <int>::iterator vit;    //will be used to iterate through the velocity's set

	//set the values for the ball objects
	set_balls();

//db	print();

	//calculate the possible velocity values and store them in the vels vector
	vels = get_vs();
//	i = 2;
	//	bool equal_branches = true;
	//DB----
	/////setvel
		for(i = 0; i < vels.size();i++)
		{
	///SETPTR--	
	//set the pointers for the ball objects in FP to point to their possible positions in the SP vector
	set_ptrs(vels.at(i));

//db	printptr();



	//DB --print the pointers to check
	rv +=  cvg( vels.at(i) );

	set_balls();

	//		print();
		}
	return rv;

}



//just prints the values of the balls in FP and Sp
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

	FP_ptr.clear();			//clear the vector

	SP_ptr.clear();			//clear the vector

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

	long long count = 1;


	vector< Ball *> FPr = FP_ptr;	//make a copy for manipulation
	vector< Ball *> SPr = SP_ptr;	//make a copy for manipulation

	Ball * FBP;

	Ball * SBP;

	Ball * HBP;		//pointer to a ball's high

	Ball * LBP;		//pointer to a ball's low

	Ball * GBP;		//pointer to a general ball

	Ball Bhld;
/*

	cout<<"-----------------------------------------the velocity is:"<<velocity<<"\n";
*/
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


	//DB	print();

	//go through FP looking for balls with : a)no pointers and if so no guesses can be made
	//										 b)only one pointer and if so set its val to -1	

	while( !(done) )
	{
		//look through FP for unconnected balls if found no guesses can be made
		for(i = 0; i < FP_ptr.size() ;i++)
		{
			FBP = FP_ptr.at(i);

			if(FBP->val != -1)
			{
				if(FBP->high == NULL && FBP->low == NULL)
				{
//					cout<<"the answer is 0\n";
					return 0;
				}
			}
		}
//cout<<"\n";
//cout<<"The pointers before removing oneses is:\n";		
//printptr();
//cout<<"\n";
		//look through FP for balls with only one pointer and remove
		for(i = 0; i < FP_ptr.size(); i++)
		{
			FBP = FP_ptr.at(i);
			//if the ball has a low but now high
			if( FBP->high == NULL && FBP->low !=NULL)
			{
				FBP->val = -1;
				LBP = FBP->low;

				//check the low ball to see if it has a low
				if(LBP->low != NULL)
				{
					LBP->low->high = NULL;		//dissconnect the low balls low from it
					LBP->low = NULL;			//disconnect the low ball from its low
				}
				//otherwise it's low is null so go from there
				LBP->high = NULL;				//dissconnect the low ball from the FP ball
				LBP->val = -1;
				FBP->low = NULL;				//disconnect the FBP from its low
			}
			//check for the ball to have a high and no low
			else if(FBP->high != NULL && FBP->low == NULL)
			{
				FBP->val = -1;
				HBP = FBP->high;

				//check for the high ball to have a high
				if(HBP->high != NULL)
				{
					HBP->high->low = NULL;		//dissconnect the high balls high from it
					HBP->high = NULL;			//dissconnect the high ball from its high
				}

				HBP->low = NULL;				//dissconnect the high ball from the FP
				HBP->val = -1;
				FBP->high = NULL;				//dissconnect the FP from its high

			}

		}
//	cout<<"\n";
//	cout<<"The pics after removing oneses is:\n";
//		   print();
//		   printptr();
//	cout<<"\n";
		
		bool test = 0;

		//check for empty set of FP balls
		for(i = 0; i < FP_ptr.size();i++)
		{
			FBP = FP_ptr.at(i);

			//if you find a ball without a val of -1 set test to 1
			if(FBP->val != -1) test = 1;
		}
		//DB		cout<<"The result of test is:"<<test<<"\n";
		//if test is still 0 then all FP's have a value of -1 so all have been removed so return 1
		if(test == 0)
		{
//			cout<<"the answer is one\n";
			return 1;
		}

		done = true;

		//check for a non double pointer ball if found set done to false
		for(i = 0; i < FP_ptr.size();i++)
		{
			FBP = FP_ptr.at(i);

			if(FBP->val != -1)
			{
				if(FBP->high == NULL || FBP->low == NULL) done = false;
			}
		}

		//		cout<<"the value of done is:"<<done<<"\n";	


	}
/*
	print();
	printptr();
*/
	bool go = true;
	long long tally = 1;
	/*
	   print();
	   printptr();
	   */	

	//check the ball values
	//	print();

	//DB	printptr();
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//now loop through chains counting once a FP ball has been counted set it to -1
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	tally = 1;
	//
	for(i = 0; i < SP_ptr.size() ;i++)
	{
		SBP = SP_ptr.at(i);
		count = 1;
		if(SBP->val != -1)
		{//	count++;
	//		cout<<"THe SP ball is:"<<SBP->val<<"\n";
	//		cout<<"The FP ball is:"<<SBP->high->val<<"\n";
	//		cout<<"the count is:"<<count<<"\n";
			while(SBP->high != NULL)
			{
				count++;
//				cout<<"THe SP ball is:"<<SBP->val<<"\n";
//				cout<<"The FP ball is:"<<SBP->high->val<<"\n";
//				cout<<"the count is:"<<count<<"\n";
				SBP->val = -1;
				SBP->high->val = -1;
				SBP = SBP->high->high;
			}

			
		}
		
//		print();
//		printptr();
		tally *=count;	
	}

//	cout<<"The tally is:"<<tally<<"\n";




	return tally;
}
