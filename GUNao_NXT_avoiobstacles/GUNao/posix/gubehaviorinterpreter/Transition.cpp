/*************************************************************************
//	Class of Activitiees in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have recieved a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
//
*************************************************************************/


#include "Transition.h"

#include "State.h"
#include "Expression.h"

#include <iostream>

using namespace std;

fsmTransition:: fsmTransition(fsmState * source, fsmState * target, fsmExpression * expression)
		{ sourceState=source;
		  targetState=target; 
		  logicExpression=expression;
		}

	
void fsmTransition:: printTransition()
	{
           cerr << "\nSource: **> " << getSource()->getID() << "-" << getSource()->getName()
<< " <** " <<  " Label: " << getExpression()->getWhatToEvaluate() << " Target: **>" << getTarget()->getID() <<
"-" << getTarget()->getName() << " <**\n";
	}
