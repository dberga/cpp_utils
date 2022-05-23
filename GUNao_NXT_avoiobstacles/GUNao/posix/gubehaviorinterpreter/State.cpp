/*************************************************************************
//	Class of Activitiees in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author:  Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have recieved a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/

#include "State.h"

using namespace std;


//Forward declaration
typedef fsmTransition* p_fsmTransition;

fmsActivity* fsmState :: getActivity() {return myActivity;}
