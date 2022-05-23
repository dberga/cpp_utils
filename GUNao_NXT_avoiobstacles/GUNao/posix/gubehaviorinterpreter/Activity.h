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

#ifndef _ACTIVITY_H_
#define _ACTIVITY_H_

#include <string>
#include <map>
#include "FSMCodeType.h"
#include "OnePost.h"
#include "Posts.h"

extern bool fsm_activity_sleep(fsmMachine *, void *, const char *param);

enum ActivityStages { STAGE_ON_ENTRY, STAGE_ON_EXIT, STAGE_INTERNAL};

/*!
 *      Class to model the activities inside a state of a finite state machine.
 *	There are activities (posting to do to the whiteboard), and are grouped in 3
 *	containers, (1) on Entry (2) on Exit (3) Internal.
 *	In addition a known C++ function can be called after the postings for each
 *	of these 3 types (ie on Entry, On Exit and Internal).
*/

class fmsActivity
{
	PossiblePosts postingOnEntry;   /// posts to be put on the wb on entry
	PossiblePosts postingOnExit;    /// posts to be put on the wb on exit
	PossiblePosts postingInternal;  /// posts to be put on the wb as activity
	fsm_callback_f myActivity;      /// activity to call (defaults to sleep(1))
	fsm_callback_f onEntry;         /// callback on entry (defaults to NULL)
	fsm_callback_f onExit;          /// callback on exit (defaults to NULL)
        const char *activityParam;      /// parameter to pass to activity callback (default: NULL)
        const char *onEntryParam;       /// parameter to pass to onEntry callback (default: NULL)
        const char *onExitParam;        /// parameter to pass to onExit callback (default: NULL)

public:
	fmsActivity (std::string theThingToDo, 
		std::map<std::string, fsm_callback_f> *table = NULL);

/*!
 *  Get the first element of a container indicating the container (as a stage), the possibilities
 * are the enum ActivityStages. 
 * INVARIANT: The container does not change.
 * POSTCONDITION: Now we can call the corresponding Next function.
 * Return NULL if the container is empty.
*/
	p_onePost getFirstWhatToDo(ActivityStages stage) 
            { switch (stage)
                { case STAGE_ON_ENTRY: return postingOnEntry.theFirstPosting();
                        break;
                  case STAGE_ON_EXIT: return postingOnExit.theFirstPosting();
                        break;
                  case STAGE_INTERNAL: return postingInternal.theFirstPosting();
                        break;
                  default: return postingInternal.theFirstPosting();
                        break;
                }
            }

/*!
 *  Get the next element of a container indicating the container (as a stage), the possibilities
 * are the enum ActivityStages. 
 * PRECONDITION: The getFirst function must be called.
 * INVARIANT: The container does not change.
 * Return NULL if the container is empty.
*/
	p_onePost getNextWhatToDo(ActivityStages stage) 
            { switch (stage)
                { case STAGE_ON_ENTRY: return postingOnEntry.theNextPosting();
                        break;
                  case STAGE_ON_EXIT: return postingOnExit.theNextPosting();
                        break;
                  case STAGE_INTERNAL: return postingInternal.theNextPosting();
                        break;
                  default: return postingInternal.theNextPosting();
                        break;
                }
            }



/*!
 * Getter of the named (known) C++ function for the stage Internal
*/
	fsm_callback_f getActivity() {return myActivity;}

/*!
 * Getter of the named (known) C++ function for the stage onEntry
*/
	fsm_callback_f getOnEntry() {return onEntry;}

        /*!
         * Getter of the named (known) C++ function for the stage onExit
         */
	fsm_callback_f getOnExit() {return onExit;}
/*!
 * Getter for the parameters for the C++ function for the stage onExit
*/
const char *getOnExitParam() {return onExitParam;}

/*!
 * Getter for the parameters for the C++ function for the stage Internal
 */
const char *getActivityParam() {return activityParam;}
        
/*!
 * Getter for the parameters for the C++ function for the stage onEntry
 */
const char *getOnEntryParam() {return onEntryParam;}
        
        
private:

	bool fmsActivityParsing(ActivityStages stage, std::string theInput,
		std::map<std::string, fsm_callback_f> *table = NULL);

	void  addOnePostToPossiblePostsByStage (ActivityStages stage, p_onePost newOne);

};

#endif // _ACTIVITY_H_
