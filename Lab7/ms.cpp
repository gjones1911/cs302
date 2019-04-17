#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <cstdio>

using namespace std;

class node
{
	public:
		int id;
		set <node *> adj;
		void prnt_adj();
		int visited;

};

class graph
{
	public:
		vector <node *> nodev;
		set <node *> nodes;
		void dfs(node *np);
		void Prnt_ns();
		node * find_nd(int id);
		void Prnt_nv();
		bool found;
};


node * graph::find_nd(int id)
{
	set<node *>::iterator sit;
	
	for(sit = nodes.begin(); sit != nodes.end(); sit++)
	{
		if( (*sit)->id == id) return (*sit);

	}


	return NULL;;
}

void node::prnt_adj()
{

	set <node *>::iterator sit;

	for(sit = adj.begin(); sit != adj.end(); sit++)
	{
		printf("The the link is: %d\n",(*sit)->id);
	}

}


void graph::dfs(node * np)
{
	int i, j, k, l;

	set <node *> ::iterator sit;

	//check for the base case
	if(np->visited == 1) return;

	np->visited = 1;

	if(np->id == 0) 
	{
		found = true;
		printf("Path 0\n");
		return;
	}

	for(sit = np->adj.begin() ; sit != np->adj.end(); sit++)
	{
		dfs( (*sit) );

		if(found == true)
		{
			
			printf("Path %d\n",np->id);
			return ;

		}
	}
}

void graph::Prnt_nv()
{
	
}

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
	int i,j,k,l,n,n1,n2,id,id2,row,col;

	string wall, rc;

	set <int> ::iterator sit;
	set <node *> ::iterator nsit;
	set <node *> ::iterator nsit2;

	node * np;
	node * np2;

	graph G;

	//get the rows and cols from std in
	
	cin>>rc>>row>>rc>>col;

	//set up the nodes and the graph
	for(i = 0; i < row*col; i++)
	{
		np = new node;

		np->id = i;
		np->visited = -1;

		G.nodes.insert(np);
	//	G.nodev.push_back(np);

	}


/*

	//now set up each nodes adjacency list
	//initially all neighbors are linked 
	for(i = 0; i < G.nodev.size(); i++)
	{
		//get the id of this node(the element number
		id = G.nodev[i]->id;

		//add forward neighbor
		if(id + 1 < row*col - 1)
		{
			G.nodev[i]->adj.insert(id+1);
		}
		//add back neighbor
		if(id - 1 >= 0)
		{
			G.nodev[i]->adj.insert(id-1);
		}
		//add the up neighbor
		if(id + col < row*col)
		{
			G.nodev[i]->adj.insert(id + col);
		}
		//add down neighbor
		if(id - col >= 0)
		{
			G.nodev[i]->adj.insert(id - col);
		}
	}

*/
	
	//go through nodes set adding adjacency list
//	for(nsit = G.nodes.begin(); nsit != G.nodes.end(); nsit++)
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


//	G.Prnt_ns();

	//now take in wall info and delete links as needed
	while( cin>>wall>>n1>>n2)
	{
		np = G.find_nd(n1);
		np2 = G.find_nd(n2);

		np->adj.erase(np2);
		np2->adj.erase(np);

	}

//	G.Prnt_ns();

	//now solve the maze with depth first search
	
	//get the last element to start from;
	np = G.find_nd(row*col - 1);

	G.found = false;

	G.dfs(np);

	if(G.found == false)
	{
		cout<<"NO path found\n";
	}


}
