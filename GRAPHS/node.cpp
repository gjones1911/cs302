#include <iostream> 
#include <cstdio>
#include <cmath>
#include <string>
#include <cstdlib>
#include <vector>

using namespace std;

class node
{
	public:
		int val;
		vector<int> adj;
		int v;				//the nodes visited field
};


class graph
{
	public:
		vector <node *> graph;
		void Print();
		void Reset(int zo);
		int is_cycle(int idx);
		void  component_cnt(int idx, int cn);
};

int graph::is_cycle(int idx)
{
	int i, j, k;

	node * np;

	np = graph[idx];

	if(np->v != -1) return 1;
	
	np->v = 1;

	for(i = 0; i < np->adj.size(); i++)
	{
		if( is_cycle( np->adj[i] ) ) return 1;
	}

	return 0;
}

void graph::Reset(int zo)
{
	int i, j, k;

	node * np;

	for(i = 0 ; i < graph.size() ; i++)
	{
		np = graph[i];

		np->v = -1;

		if(zo == 1)
		{
			np->adj.clear();		
		}
	}

	return;
}

int graph::component_cnt(int idx, int cn)
{	
	node *np;

	np = graph[idx];

	if( np->v != -1 ) return ;

	np->v = cn;

	for(int j = 0 ; j < graph[idx]->adj.size() ; j++)
	{
		component_cnt(np->adj[j], cn );
	}

	return ;
	
}

void graph::Print()
{
	int i, j, k;

	if(graph.size() == 0)
	{
		cout<<"no nodes in graph\n";
		return;
	}

	for(i = 0; i < graph.size();i++)
	{
		cout<<"node : "<<graph.at(i)->val<<"\n";
		cout<<"Edges with : ";
		
		for(j = 0; j < graph.at(i)->adj.size();j++)
		{
			cout<<graph[i]->adj[j]<<" ";
		}
		
		cout<<"\n";
	}

	cout<<"\n";

	return;
}


int main(int argc, char** argv)
{
	int i, j, k, cnt, num,num2;

	graph  Gp;

	node * np;

	cnt = 0;

	string shldr,EDGE;

	//take in a graph on std input in the format
	//NNODES -the number of nodes
	//EDGE n1 n2...
	
	cin>>shldr>>num;

	if(shldr != "NNODES") 
	{
		cout<<"usage-- NNODES the number of nodes\n";
		exit(0);
	}

	if(num <= 0) 
	{
		cout<<"the number must be greater than zero\n";
		exit(0);
	}

	printf("There are %d nodes:\n",num);
	

	//create the number of nodes requested
	for(i = 0; i < num ;i++)
	{
		np = new node;
		np->val = i;
		np->v = -1;
		Gp.graph.push_back(np);
	}


	//use std input to set up edges
	while( cin>>EDGE>>num>>num2)
	{
		//take in EDGE node 1 node 2
		Gp.graph.at(num)->adj.push_back(num2);
		Gp.graph.at(num2)->adj.push_back(num);
	}
	
	Gp.Print();

	cnt = 0;



	for(i = 0; i < Gp.graph.size(); i++)
	{
		//if it hasnt been visited perform a DFS on it and increment the connected component count
		if(Gp.graph[i]->v == -1)
		{
			cnt++;
			Gp.component_cnt(i,cnt);

		}
	}

	cout<<"there are "<<cnt<<" components\n";

	Gp.Reset(0);

	for(i = 0; i < Gp.graph.size(); i++)
	{
		np = Gp.graph[i];

		if(np->v == -1)
		{
			if( Gp.is_cycle(i) )
			{
				cout<<"there is a cycle starting at "<<i<<"\n";
			}
			else
			{
				cout<<"no cycle reachable from node "<<i<<"\n";
			}
		}
	}


	return 0;
}
