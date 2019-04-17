//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Program:    maze_solve.cpp									                                                            //
//Created by: Gerald Jones																									//	
//Purpose:    This is lab 7 for cs302. It uses depth first search to solve a maze specified on std input,and outputs the	//
//		      path that solves the maze on std out.																			//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		      


#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <cstdio>

using namespace std;


//a class representing a node on a graph
class node
{
	public:
		int id;
		set <node *> adj;		//the adjacency list for the node
		void prnt_adj();		//prints the adjacency list for this node to std out
		int visited;			

};

//a class representing a graph
class graph
{
	public:
		set <node *> nodes;						//the nodes that make up the graph
		void dfs(node *np);						//performs a depth first search on the graph
		void Prnt_ns();
		node * find_nd(int id);					//returns a pointer to the node with the given id
		bool found;								//used to tell when the zero node has been found
};


//will find a pointer to a node with this id
//if the node is not found it will return NULL
node * graph::find_nd(int id)
{
	set<node *>::iterator sit;
	
	for(sit = nodes.begin(); sit != nodes.end(); sit++)
	{
		if( (*sit)->id == id) return (*sit);
	}

	return NULL;;
}


//will print the nodes adjacency list
void node::prnt_adj()
{
	set <node *>::iterator sit;

	for(sit = adj.begin(); sit != adj.end(); sit++)
	{
		printf("The the link is: %d\n",(*sit)->id);
	}
}


//will perform a depth first search  and by doing so attempts to solve the maze
void graph::dfs(node * np)
{
	int i, j, k, l;

	set <node *> ::iterator sit;

	if(np->visited == 1) return;

	np->visited = 1;

	//if i have found 0 then i have solved the maze so start returning
	//and printing where you are as you go
	if(np->id == 0) 
	{
		found = true;
		printf("PATH 0\n");
		return;
	}

	for(sit = np->adj.begin() ; sit != np->adj.end(); sit++)
	{
		dfs( (*sit) );

		//if found has been set to true then we have solved the maze
		//so start spitting out the path back
		if(found == true)
		{
			
			printf("PATH %d\n",np->id);
			return ;

		}
	}
}

//This will print the nodes set stored in the graph
void graph::Prnt_ns()
{
	set <node *>::iterator sit;

	for(sit = nodes.begin(); sit != nodes.end(); sit++)
	{
		printf("The element is: %d\n",(*sit)->id);
		(*sit)->prnt_adj();
	}
}


int main(void)
{
	char buff[50];
	
	int i,j,k,l,n,n1,n2,id,id2,row,col;

	string wall, rc,spec;

	set <int> ::iterator sit;
	set <node *> ::iterator nsit;
	set <node *> ::iterator nsit2;

	vector < string > smaze;

	node * np;
	node * np2;

	graph G;

	cin>>rc>>row>>rc>>col;

	//set up the nodes and the graph
	for(i = 0; i < row*col; i++)
	{
		np = new node;

		np->id = i;
		np->visited = -1;

		G.nodes.insert(np);
	}

	//go through nodes set adding adjacency list info
	for(i = 0; i < row;i++)
	{
		for(j = 0; j < col; j++)
		{

			np = G.find_nd(i*col+j);

			id = np->id;

			if(j+1 < col)
			{
				//add the forward buddy
				np2 = G.find_nd(i*col+j+1);
				if(np2 != NULL)
				{
					np->adj.insert( np2 );
				}
			}


			if(j-1>=0)
			{
				//add the back buddy
				np2 = G.find_nd(i*col+j-1);
				if(np2 != NULL)
				{
					np->adj.insert( np2 );
				}
			}


			if( (i-1) >=0)
			{
				//add the down buddy
				np2 = G.find_nd( (i-1)*col +j);
				if(np2 != NULL)
				{
					np->adj.insert( np2 );
				}
			}


			if( (i+1) < row)
			{
				//add the up buddy
				np2 = G.find_nd( (i+1)*col+j);
				if(np2 != NULL)
				{
					np->adj.insert( np2 );
				}
			}
		}
	}




	//now take in wall info and delete links as needed
	//and store the maze as we go
	while( cin>>wall>>n1>>n2)
	{

		sprintf(buff,"WALL %d %d\n",n1,n2);
		
		spec = buff;

		smaze.push_back(spec);

		np = G.find_nd(n1);
		np2 = G.find_nd(n2);

		np->adj.erase(np2);
		np2->adj.erase(np);

	}



	printf("ROWS %d COLS %d\n",row,col);

	for(i = 0; i < smaze.size(); i++)
	{
		printf("%s",smaze[i].c_str());
	}

	
	//get the last element to start from;
	np = G.find_nd(row*col - 1);

	G.found = false;

	G.dfs(np);

	return 0;
}
