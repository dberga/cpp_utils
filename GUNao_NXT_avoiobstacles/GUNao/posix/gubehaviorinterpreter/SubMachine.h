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

#include "Machine.h"

const string SUBMACHINE_ON ="On";
const string SUBMACHINE_OFF ="Off";
/*!
 * Class to create SUB machines (finite state machines that can be called from another
 * finite state machine; see Machine.h). This should be the vehicle to add complexity to
 *  behaviours. For example, the guGameController FSM can be refined in its READY state
 *  by the current example (under GUNao/posix/gubehaviourinterpreter/guSeeker)
 * guPlacer (a FSM that will aim at placing a robot back to its field and position for
 * a re-start of the soccer match).
 *
 * Sub machines (fsmSubMachine) are a subclass of fsmMachine and can be started running
 *  (like a normal FSM of the superclass) but they can also be started dormant.
 *
 * If they are running, they can be set to dormant by a message on the Whiteboard
 *  <NAME_OF_MACHINE> (as the message type) and Off  as content.
 *
 * However, the sub-machine will continue until it finishes the cycle of the state it
 *  is in, namely, evaluate transitions, 
 * (a) if one fires do the on exit actions/activities
 *  (b) if none fires do the internal activities but will go dormant before staring
 *  to evaluate transitions again).
 *
 * If it is dormant, a whiteboard message <NAME_OF_MACHINE>(as the message type again; )
 * and On as content  restarts/resumes it from
 *  its initial state. There is nothing remembered from one pause to a resume except the
 *  subscription to the whiteboard and the reading of its A<NAME>.txt and T<NAME>.txt files,
 *  so there is no performance penalty and one does not need to call the constructors except
 * only once.
 *
 * Note the model now is by posting to the whiteboard the <NAME> On (resume) or the <NAME> OFF (suspend) 
 * messages; while the transitions of the fsm are queries to an expert. This is a conscious decision
 * to de-couple sub-machines from whatever super-state of another machine they are to run.
 *
*/

class fsmSubMachine : public fsmMachine
{

/*!
 *	The call-back to when we are stoped is now also the call-back to when we are re-started
 *	 from initial state.
 *	New approach to start execution of a FSM, does the global SetUp and then calls execute
*/
void onWhiteboardMessageResumeOrStop(std::string dataName, WBMsg *msg);


/*!
 *	Boolean to keep track if submachine is running, so later messages to be turned on
 *	are ignored
*/
	bool iAmRunning;

/*!
 *	Boolean to keep track if its first execution, so later messages to be turned on
 *	fo not do a set up
*/
	bool firstTime;

/*!
 *  we are overwriting setUp  to make sure we do this only once
 * even if we are started with run or with a whiteboard message
*/
	virtual void setUp(); 

/*!
 *	We are either executing the cycle of a FSM (our superclass)
 *	or we became dormant, where we put the thread to sleep
 *	this gets waken up by the call-back to resume
*/
	void executeNsleep();

/*!
 *	Where we sleep and wait for a notify from a call-back that was waken by the
 *	resume in the whiteboard
*/
	void dormant();

/*!
 *	A semaphore to wait in dormant, and to use for waking up when the proper
 *	message <NAME>On is posted on the whiteboard
*/
	sem_t forNotify;

/*!
 *	To protec the code in the callback, so not two resume and/or stop messages
 *	conflict (race condition) the boolean flags that control if the FSM is 
 *	runing or dromant.
*/
pthread_mutex_t stop_resume_mutex;


public:
/*!
 *	First aspect that makes a submachine special is that in our constructor we will
 *	subscribe to messages about being turned on, in which case we resume, and about
 *	being turned off, in which case we suspend.
*/
	fsmSubMachine (string theFileName, std::map<std::string,
				fsm_callback_f> *named_functions = NULL, void *context = NULL);
	~fsmSubMachine ()
	{
	}

/*!
 *  we are overwriting run  to make sure we do setUp only once
 * even if we are started with run or with a whiteboard message
*/
	virtual void run(); 

/*!
 *	We enable that after calling the constructor on s submachine you call this methof
 *	RATHER than run, to start the submachine in the dormant state waiting for
 *	<NAME>On so that it then catually starts, 
*/
	void startDormant(); 

};
