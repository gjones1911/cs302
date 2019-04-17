
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

		vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP

		vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP
};



long long OneDimensionalBalls::countValidGuesses(vector<int> &firstPicture, vector<int> &secondPicture)
{

	///////////cntvld varss----

	int i , d, j;

	long long rv = 0;			//will be used to return the number of guesses

	string shld;

	stringstream ss;


	Ball * BP;					//will be used to grab pointers to ball instances

	vector <int> FP = firstPicture;		//will hold the original 
	vector <int> SP = secondPicture;	//will hold the original


	//DB	vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP
	//DB	vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP


	vector <vector<int> > diff;	//will hold the differences of the FP values and the SP values

	diff.resize( FP.size() );

	set <int> vels ;			//will store the possible legal velocitys using a set because i dont want duplicates 

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



	//	if(FP == SP) return 0; 

	for( i = 0 ; i < FP.size(); i++)
	{
		for( j = 0; j < SP.size(); j++)
		{
			d = FP.at(i) - SP.at(j);		//get the difference

			if(d != 0)
			{
				diff.at(i).push_back( d );		//put the difference in the vector
			}

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


		if(diff.size() == 1)
		{
			return diff.at(0).size();
		}


		//db			cout <<"the top num is "<<diff.at(0).at(i)<<"\n";
		for(int r = 1; r < diff.size(); r++)
		{	
			for( int c = 0; c < diff.at(0).size(); c++)
			{

				//db					cout <<"the maybe match is : "<<diff.at(r).at(c) <<"\n";


	//			if(abs( diff.at(0).at(i) ) != abs( diff.at(r).at(c) ) )
	//			{
	//				r = diff.size();
	//				c = diff.at(0).size();
	//			}
				//check for a match
				if(abs( diff.at(0).at(i) ) == abs( diff.at(r).at(c) ) )
				{
					//check for the end of the vector if there store the velocity
					if(r == diff.size() - 1 )
					{
						d = diff.at(r).at(c);
						//get the positive of the given value if need be
						if( d < 0 ) d = -d;

						//db							cout<<"the d is:" << d << "\n";
						vels.insert( d );		//store the velocity


						c = diff.at(0).size();		//go to next row
					}
					//if not at the end go on to next row
					else
					{
						c = diff.at(0).size();
					}
				}
				else
				{
					//if we made it to the end of the column without a match move to next top number
					if(c == diff.at(0).size()- 1 )
					{
						r = diff.size();
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
	cout<<"f\n";



	i = 0;

	for(vit = vels.begin(); vit != vels.end(); vit++)
	{
		cout << i++ <<" ";
	}
	cout<<"\n";

	cout<<"Give me your choice from above ^^^\n";
	cout<<"or f for them all at once\n";

	int nmb = 0;

	bool keepgoing = true;

	if(getline(cin,shld))
	{
		ss.clear();
		ss.str(shld);

		while(keepgoing)
		{
			if( !(ss>>nmb) )
			{
				ss.clear();


				ss>>shld;

				cout<<"you chose f i hope\n";



				if(shld == "f")
				{
					keepgoing = false;
				}
				else
				{
					cerr<<"your choice does not match above\n";
				}	
			}
			else if( nmb > i || nmb < 0)
			{

				cerr<<"your choice does not match above\n";

			}
			else
			{
				keepgoing = false;
				cout<<"you chose :"<<nmb<<"\n";
			}

		}

	}


	if( shld != "f")
	{


		vit = vels.begin();

		////////////////////////////choose v--------------	
		for(int cnt = 0; cnt < nmb; cnt++)
		{
			vit++;
		}
		i = (*vit);

		rv = cvg(i);

		return rv;


	}
	else
	{
		for(vit = vels.begin(); vit != vels.end(); vit++ )
		{
			i = (*vit);
			rv += cvg(i);


		}


		return rv;
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// This block of code will call the cvg() function and calculate the valid guesses for each valid velocity //
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cvg( ) ---  will count the valid guesses for the velocity given
///////////////////////////////////////////////////////////////////////////////////////////
long long OneDimensionalBalls::cvg( int &velocity)
{
	//cvg vars
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

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up appropriate pointeres //
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	//go through the FPr vector of pointers to the balls in the first picture
	for(i = 0; i < FPr.size(); i++)
	{
		//take velocity add and subtract it from the first picture  and  the values of the balls in the SPr vector
		//that match this value are to be connected 
		//by there high and low pointers
		for( j = -1 ; j <= 1 ; j += 2 )
		{
			FBP = FPr.at(i);
			val = FBP->val + velocity * j;

			//			printf("the val for FP ball[%d] is %d\n",i,val);

			//go through the trying to match the val to the num
			for(cnt = 0; cnt < SPr.size(); cnt++)
			{
				SBP = SPr.at(cnt);		//grab pointer
				num = SBP->val;

				//				printf("the num for SP ball[%d] is %d\n",cnt,num);

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
	//db312	/////////////-------pointer printer----------
	

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
	///////////////////////////////////---------------------------------------------------------------
		


		/////////////////////////////////////////////////////////////////////////////////////////////////
		// look through both FPr and SPr vector looking for balls with no pointers and removing them 
		// i.e setting thier val = -1 so i see them as invisible/
		/////////////////////////////////////////////////////////////////////////////////////////////////

		bool found = true;
	
		int top_cnt = 0;

		vector <int> allneg;
		vector <int> test;
		allneg.resize(FPr.size(), -1);
		

	//go through FPr until you dont find any single pointer balls
	while(found)
	{
		cout<<"GET it\n";
	
		//check for all invisivlble top
		
		for(i = 0; i < FPr.size();i++)
		{

			test.push_back(FPr.at(i)->val);
		}
		if( test == allneg) 
		{
			found = false;	

			return 1;

		}

		for(i = 0; i < FPr.size();i++)
		{

			if(FPr.at(i)->val != -1)
			{
				//if a top ball has no pointers or guesses no guess can be made
				if( FPr.at(i)->high == NULL && FPr.at(i)->low == NULL)
				{
					cout<<"no guess can be made\n";
					return 0;
				}
				//if it only has one pointer make it invisible to counter
				else if(  (FPr.at(i)->high == NULL && FPr.at(i)->low != NULL) || (FPr.at(i)->high != NULL && FPr.at(i)->low == NULL) )
				{
					FPr.at(i)->val = -1;
					
					top_cnt++;

					if(top_cnt == FPr.size() ) return 1;
					//db					printf("Ball %d has been set to invisible\n",i);

				}
				else
				{
					found = false;
					//db					cout<<"no single pointer balls found\n";
				}
			}
		}

	}


	//now go through SP balls i making neccessary balls invisible
	for(i = 0; i < SPr.size();i++)
	{


		SBP = SPr.at(i);

		//		vector<Ball*>::iterator B_it;

		//look for a non connected bottom ball and remove it/print it
		if( SBP->high == NULL && SBP->low == NULL)
		{
			//db			printf("Sp Ball %d is %d and it's h/l are Null\n", i,SBP->val);

			//set this pointers val to -1 or invisible
			SPr.at(i)->val = -1;

			//db			printf("SP Ball %d has been set to invisible\n",i);

			//find a iterator to this pointer
			//			SPr.erase( SPr.begin() + i );

		}
		else if(SBP->high == NULL)
		{
			//get it's low and set it to invisible
			//	SBP->low->val = -1;

			//db			printf("SP Ball %d is %d and it's high is null\n", i,SBP->val);
		}
		else if(SBP->low == NULL)
		{
			//get it's high and set it to invisible
			//	SBP->high->val = -1;

			//db			printf("SP Ball %d is %d and it's low is null\n", i,SBP->val);
		}
		else
		{
			//db			printf("SP Ball %d is %d and it's h/l are %d and %d\n", i,SBP->val,SBP->high->val,SBP->low->val);
		}

	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Now do the count 

	long long gcnt = 1;
	int crnt = 1;
	int next = 0;
	long long tally = 1;
	Ball * NXT_B ;
	for(i = 0 ; i < SPr.size(); i++)
	{
		gcnt = 1;
		if(SPr.at(i)->val != -1 && SPr.at(i) != NXT_B && SPr.at(i)->high != NULL)
		{
			//db			cout<<"call count "<<i+1<<"\n";

			//db			cout<<"The val is: "<<SPr.at(i)->val<<"\n";


			NXT_B = SPr.at(i)->high->high;
			SPr.at(i)->val = -1;

			//look  ahead and check for pointers for null or a value of invisible

			if(NXT_B != NULL)
			{


				gcnt += 1;
				bool go = true;

				while(go)
				{
					if(NXT_B->high == NULL) 
					{
						go = false;	
					}
					else if(NXT_B->high->val == -1)
					{
						go = false;
					}
					else
					{

						gcnt += 1;
						NXT_B->val = -1;

						NXT_B = NXT_B->high->high;

						//db						cout<<"get here a\n";
					}
				}

			}
			tally *= gcnt;

		}



	}


	cout<<"the tally is:"<<tally<<"\n";
	//if it gets here they are all zero so return 1

	return tally;

}

//////themain/////////////////////
int main(int argc, char **argv)
{

	int i , nm;
	long long num;

	vector <int> sp ;
	vector <int> fp ;
	//create instance of OneDimensionalBalls class
	OneDimensionalBalls  ODB;

	string strg;

	bool going = true;

	//	string sh = arcv[1];


	vector <vector<string> >  svc;

	svc.resize(2);

	stringstream ss;
	ss.clear();
	ss.str("");

	if( argc > 1 && atoi( argv[1] ) == -1)
	{


		//get the fp line if you can
		if(getline(cin,strg) )
		{

			ss.clear();
			ss.str(strg);
			while( ss>>i)
			{
				fp.push_back( i );

			}
		}

		//get the sp line
		if(getline(cin,strg) )
		{

			ss.clear();
			ss.str(strg);
			while( ss>>i)
			{
				sp.push_back( i );

			}
		}


		num = ODB.countValidGuesses(fp, sp); 

	}
	else if( argc > 1 && atoi( argv[1] ) != -1)
	{
		cerr<<"put -1 on the comand line to enter the FP an SP on two seperate input lines\n";
	}
	else
	{



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


		num = ODB.countValidGuesses(fp, sp); 
	}

	printf("The answer is : %lld \n",num);
	return 0;
}
