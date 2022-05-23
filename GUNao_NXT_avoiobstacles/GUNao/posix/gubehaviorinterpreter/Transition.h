/*************************************************************************
//	Class of Activities in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have received a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/

#ifndef _Transition_H_
#define _Transition_H_

class fsmExpression;

#include <string>

class fsmState;
typedef fsmState* p_fsmState;

/*!
 *      Class to model the transition between states of a finite-state machine. A transition has a
 *	source state (see State.h) and a target state (also an object in State.h). The expression
 *	labeling a transition is Expression because we want them to be queries to an expert
 *	The expert may be a logic engine that interprets a logic, or a wrapper for a sensor that
 *	provides information about what has been sensed. We expect that expression could be
 *	theorems to prove in a logic by an expert that always finishes declaring the expression
 *	as true or false.
*/
class fsmTransition
{

	p_fsmState	 sourceState;
	p_fsmState	 targetState;
	fsmExpression* logicExpression;

public:
	fsmTransition(fsmState * source, fsmState * target, fsmExpression * expression);

/*!
 * Getter for the source state
*/
	fsmState* getSource () { return sourceState; }

/*!
 * Getter for the source target
*/
	fsmState* getTarget  () { return targetState; }

/*!
 * Getter for the expression labeling the transition
*/
	fsmExpression* getExpression  () { return logicExpression; }

/*!
 * A utility function for debugging
*/
	void printTransition();

};

#endif
