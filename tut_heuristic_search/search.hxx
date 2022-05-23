#ifndef _SEARCH
#define _SEARCH

#include <iostream>
#include <cstdlib>
#include <sstream>
#include <utility>
#include <queue>
#include <algorithm>
#include <fstream>
#include "blocks_world.hxx"
using namespace std;


class Node{

	public:
	 State *s;
	 unsigned H;
	 unsigned G;

 	Node(State* newstate){ //constructor
		s = newstate;
		G = 0;
		H = 0;
	}
	Node(){ //constructor void
		s = new State();
		G = 0;
		H = 0;
	}
	void setState(State* newstate){ //setter state
		s = newstate;
	}
	void setH(unsigned h){ //setter H
		H=h;
	}
	void setG(unsigned g){ //setter G
		G=g;
	}
};

struct CompareNode : public std::binary_function<Node*, Node*, bool>                                                                                       
{  
  bool operator()(const Node* lhs, const Node* rhs) const  
  {  
     return (lhs->G+lhs->H) < (rhs->G+rhs->H);  
  }  
};


class Search{

	public:
	void solve(Blocks_World ourworld);
	
	
	private: 
	//valors de blocks_world
	std::vector<Action*> actions;
	State* m_initial_state;
	State* m_final_state;
	
	//variables globals
	priority_queue<Node*,vector<Node*>, CompareNode > p_queue;
	std::vector<State*> closedstates;
	
	//funcions
	unsigned calcH(State &s1,State &s2);
	void getvalues(Blocks_World ourworld);
	unsigned recorrer(Node *enew);
};

#endif

