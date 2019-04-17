
/////////////////////////////////////////////////////////////////////////////////////////////////
//Program: //
//Created By: Gerald Jones                                                                     //
//Purpose: //
/////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>

using namespace std;

class People
{
	public:
		vector <string> people;	//will hold the people to choos from
		vector <string> team;	//the team of given size
		void GenTeams(int index, int npeople);	//recurisive funtions takes an index to pull from the people vector and the number of people left to pull
};

void People::GenTeams(int index, int npeople)
{
	int i;

	//if you have no more people to place on the team
	//go ahead and print the current team
	//this is the base case for the function
	if(npeople == 0)
	{
		cout<<team[0];
		for(i = 1;i < team.size();i++) cout<<" "<<team[i];
		cout<<"\n";
		return;
	}

	//check for a number of people on the team greater than the number of people	//we have just return
	//makes sure the number of people left doesnt exceed the number of people wh	//have left to use
	if(npeople > people.size() - index) return;
	
	//if you still have people to place put the current person(at current index)
	//on the team
//	team.push_back(people[index]);
	team.push_back(people[index]);

	//call the recursive funct with the next index to get the next person
	//and a decremented number of people
	GenTeams(index + 1, npeople -1);

	team.pop_back();
	
	//now remove the current person from the team and do the team on the next person without the curren person. and with the current number of people left
	GenTeams(index+1, npeople);
}

int main(int argc, char** argv)
{	
	People P;
	string s;
	int i , j, t_num;

	if(argc < 2 ||  atoi( argv[1] ) <  0)    
	{
		cerr<<"usage: ./enm2 number to choose\n";
		exit(0);
	}
	t_num = atoi(argv[1]);

	printf("your number of tesms is %d\n",t_num);

	//until told otherwise just take in names from std input and store in thee people vector
	
	while(cin>>s) P.people.push_back(s);

	P.GenTeams(0,t_num);

		return 0;
}
