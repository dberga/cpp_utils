/*************************************************************************
//	Class of Activities in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author:  Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have received a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/

#include <vector>
#include <string>

#include "Activity.h"

#include "Transition.h"

#include "gu_util.h"

#include <iostream>

//Forward declaration
typedef fsmTransition* p_fsmTransition;

/*!
 *	Class to model the state of a finite state machine. A state has a unique ID (works almost
 *	like a primary key). A name, an activity, and transitions leaving the state (as many as
 *	its out-degree. 
*/
class fsmState
{
	int stateID;
	string stateName;
	int outDegree;
	fmsActivity* myActivity;
	vector <p_fsmTransition>  outTransitions;
	vector <p_fsmTransition>::iterator internalIterator ;

public:
	fsmState (int theID, std::string name, fmsActivity* theActivity): outTransitions(),stateName(name)
		 { stateID=theID;  myActivity=theActivity; outDegree=0;
                   DBG(cerr << "Constructed "<< getName() << "\n"); 
                 }

/*!
 * Getter for the ID
*/
	int getID() {return stateID;}
/*!
 * Getter for the Name
*/
	string getName() {return stateName;}

/*!
 * Getter for the Activity
*/
        fmsActivity* getActivity();

/*!
 * Getter for the out-degree
*/
	int getOutdegree() {return stateID;}

/*!
 * When constructing a FSM, we add repeatedly to a state its transitions. Perhaps this should
 * not be public, but private and with friends.
*/
	void addTransition(p_fsmTransition theTransition)
	 {outDegree++; 
	 vector<p_fsmTransition>:: iterator it;
	 it=outTransitions.begin();
	 outTransitions.insert(it,theTransition);
	  //cerr << "In Add Transition\n";
	  theTransition->printTransition();
	 }

/*!
 * Provides a way to navigate transitions out of a state, this return the first transition out.
 * NULL is returned if there are no transitions out, a final state.
*/
	p_fsmTransition theFirstTransition()
	{  DBG( cerr << "State.h::theFirstTransition(): ");
	   if (0 < outTransitions.size()  ) 
		{ internalIterator=outTransitions.begin();
	   	  DBG((*internalIterator)->printTransition());
	   	  return (*internalIterator);
		}
	   else
		return NULL;
	}

/*!
 * Provides a way to navigate transitions out of a state, this return the next transition out.
 * PRECONDITION: theFirstTransition() must have been called.
 * Return NULL when the transitions out are exhausted.
*/
	p_fsmTransition theNextTransition()
	{  DBG( cerr << "State.h::theNEXTTransition(): ");
		internalIterator++;
	   if (internalIterator < outTransitions.end())
	   { DBG((*internalIterator)->printTransition());
		   return (*internalIterator);
	   }
	   else
		return NULL;
	}

};
