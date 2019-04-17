
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

//the class that holds the functions that will solve the problem
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

		bool  branch_check();

		vector <int> get_vs();								//will calculate the possible vectors for guessing

		vector <Ball *> FP_ptr;		//will hold pointers to the balls in FP

		vector <Ball *> SP_ptr;		//will hold pointers to the balls in SP


		vector <int> fpo;
		vector <int> spo;

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




//will calculate the possible velocities
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
	/*

	/////DB--17------Difference printer----------////////	


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


	////DB--17-------------------------------------------	

*/
	//find valid velocitys
	//
	//start at the top row and go through it
	for(i = 0; i < diff.at(0).size(); i++)
	{

		//ignore zeros
		if(diff.at(0).at(i) != 0)
		{

			//go through the rest of the rows starting at 1 comparing the element in the top row
			//to the current element in the lower rth row
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


bool OneDimensionalBalls::branch_check()
{
	int i,j,k,lcnt,rcnt;

	lcnt = 0;
	rcnt = 0;

	Ball * FBP;
	for( i = 0; i < FP_ptr.size(); i++)
	{
		FBP = FP_ptr.at(i);
		if(FBP->low != NULL && FBP->low->val != -1)
		{
			lcnt++;
		}
		if(FBP->high != NULL && FBP->high->val != -1)
		{
			rcnt++;
		}
	}
	/*
	   cout<<"\n\n";
	   cout<<"The left and right counts are: "<<lcnt<<" "<<rcnt<<"\n";
	   cout<<"\n\n";

*/	if(lcnt != rcnt)
	{
		//		cout<<"the are not equal\n";
		return false;
	}
	else
	{
		return true;
	}

}

//will set up FP and SP and call cvg to count the valid guesses for the possible velocitys
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


	cout<<"-----------------------------------------the velocity is:"<<velocity<<"\n";

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
cout<<"\n";
cout<<"The pointers before removing oneses is:\n";		
printptr();
cout<<"\n";
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
	cout<<"\n";
	cout<<"The pics after removing oneses is:\n";
		   print();
		   printptr();
	cout<<"\n";
		
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
		SBP = SP_ptr.at(i);		//grab pointer to sp ball
		count = 1;
		//if the SP ball is visible
		if(SBP->val != -1)
		{	cout<<"The sbp val is:"<<SBP->val<<"\n";

			SBP->val = -1;

			cout<<"the count is:"<<count<<"\n";
			FBP = SBP->high;
			if(FBP != NULL)
			{
				cout<<"The val of      FBP is:    "<<FBP->val<<"\n";
				count++;
				cout<<"the count is:"<<count<<"\n";
				FBP->val = -1;
				HBP = FBP->high;
				cout<<"the next SP ball is:"<<HBP->val<<"\n";
				HBP->val = -1;
				//check for a dead end
				if(HBP->high != NULL)
				{

					while(go)
					{
						count++;
						cout<<"the count is:"<<count<<"\n";
						FBP->val = -1;		//make current Fp invisible

						FBP = HBP->high;
						cout<<"The new FBP is:"<<FBP->val<<"\n";
						HBP = FBP->high;
						cout<<"The new SP is:"<<HBP->val<<"\n";
						FBP->val = -1;
						HBP->val = -1;
						//if at dead end end loop
						if(HBP->high == NULL)
						{	cout<<"\n\nThis is a dead end in it\n\n";
							go = false;
						}
					}
				}
			}
			else
			{
				cout<<"\n\n";
				cout<<"This is a dead end.....\n\n\n";
			}

		}
		print();
		printptr();
		tally *=count;	
	}

	cout<<"The tally is:"<<tally<<"\n";




	return tally;
}
