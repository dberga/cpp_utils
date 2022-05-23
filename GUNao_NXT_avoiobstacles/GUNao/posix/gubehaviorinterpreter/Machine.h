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

#include <vector>
#include <string>
#include <pthread.h>
#include <Whiteboard.h>
#ifdef USE_STD_SEM
#include <semaphore.h>          // lacks sem_init() on a Mac
#else
#include "GUsemaphore.h"
#endif
#include "State.h"

class fsmExpression;

/*!
 *  Implementation of a finite state machine that can have states performing activities as well
 *  as activities only performed on entry (on arrival to a state) as well as on exit (when
 *  departing a state). The internal activities may not be performed at all if a transition (leaving
 *  the state) fires (is true) immediately. ON ENTRY and ON EXIT activities are always performed.
 *  Transitions are questions to an expert, who should pronounce itself about the true/false
 *  of the predicate labeling the transition. The Machine will wait for the expert response
 *  and not evaluate another transitions (transition evaluation is blocking).
 *  a transition evaluated TRUE always fires and the machine moves on to the next state,
 *  The machine halts in a state that has no exiting transition.
 *  Transitions are evaluated in reverse order as they appear in T<MACHINE_NAME>.txt file,
 *  thus transitions are not required to be exclusive.
 *  The initial state is the last state in A<MACHINE_NAME>.
*/
class fsmMachine
{
/*!
 *	Number of states
*/
	int size;
/*!
 *	Data structure to stores "STATES", see State.h
*/
	vector <fsmState*>  theStates;

/*!
 *	When reading A<NAME> we check that each ID state is new and does not exist already.
*/
	bool exists_already (int IDtoCheck);

/*!
 *	Find a state given its ID.
*/
	fsmState*  find (int IDtoFind);

/*!
 *	Iterate and place in the whiteboard with addMessage
*/
	void postAllMessagesForStage(ActivityStages stage, fmsActivity *activity);


	bool load_theory (char *file);
	string theory_content;
	string theory_name;

	pthread_mutex_t mutex;
	sem_t  forWaitNotify;


/*!
 *	Every label of every transition is going to be of interest for us, so we subscribe
 *	to the whiteboard to have a call_back anytime someone pronounces themselves about
 *	this. We do this at the beginning. Perhaps should be done per state but we don't
 *	want to bombard the whiteboard with subscribe and UN-subscribe events every change
 *	of state. So a mechanism of registering what we are interested in follows.
*/
	void subscribeToAllOutputs(guWhiteboard::Whiteboard &wb);
/*!
 *	Variable that is a flag that is up when we asked an expert for a proof, so we
 *      can ignore in the call-Back other postings when we didn't ask for a proof of
 *	that item, but we indicated interest because it is a label of another transition.
*/
	bool wantProof;

/*!
 *	The actual expression I am waiting for a recommendation from an expert. It labels
 *	the current transition. So, executions of the callback from names of other transitions
 *	can be ignored.
*/
	fsmExpression*      whatIamWaitingfor;

/*!
 *	We send the message of the type kUpdateProofsForOutput and wait for an expert to
 *	wake us up in a call_back.
*/
	char doProof(fsmExpression * output);

/*!
 *	This is the call_back where the whiteboard reports someone has pronounced about
 *	a transition-label. We examine that is indeed the one we are interested in and then
 *	if it is a good one, extract the boolean value that will determine if we consider
 8	the transition has fired
*/
	void My_Callback(std::string dataName, WBMsg *msg);

	bool callBackValue;

	void *activity_context;
	fsm_callback_f init_context;

protected:
/*!
 *	Subscribe to all labels of transition, get the Context of C++ calls and everything
 *	globally needed to start a machine
 *	the setup of a submachine will change the flag that it has already done this
*/
	virtual void setUp();
/*!
 *	The actual interpreter. It always executes the On-Entry actions, the On-Exit Actions. It then
 * evaluates the transitions in the reverse order they appeared in the T<NAME> file (the later in the
 * file the earlier it is evaluated). If all transitions are false, then it does the internal activities
 * and sleeps for a bit.
*/
	void execute();
/*!
 *	Sub-machines can be suspended or can resume from their initial state, this variable
 *	is checked and modified by other modules that post to halt or to restart the submachine
 *
*/
	bool shallIResume;
/*!
 *	Our access to a whiteboard, interestingly most other modules use a pointer to a whiteboard
 *	and get it on the constructor, while here we have it as an member private variable
 *	so we don't need to delete it in our destructor
*/
	guWhiteboard::Whiteboard wb;

public:
/*!
 *     Create a Finite State Machine. It needs a NAME and there must exists at least two files named
 *	T<NAME>.txt and A<name>.txt. The A file is the ACTIONS file and describes the states and
 *	what happens in them. T<NAME> describes the transition. This constructor has inside the
 *	parser of this. The grammar is specified in the subdirectory LANGUAGE. 
 *	In order to run additional C++ code, there could be a table of known functions of C++
 *      code. Those are the other parameters
*/
	fsmMachine (string theFileName, std::map<std::string,
				fsm_callback_f> *named_functions = NULL, void *context = NULL);
	~fsmMachine ()
	{
	}
/*!
 *	New approach to start execution of a FSM, does the global SetUp and then calls execute
 *	A submachine will redefine (over-write) the method run because a submachine can be started
 *	with run or with a posting to the whiteboard (and then suspended), and therefore it
 *	must know if it is the first time it is run and execute its setUp which is different
*/
	virtual void run();
		// used in Machines with embedded C++
/*!
 *  The following functions should not be public, they should be private and what they return
 * should be used by friend classes that in this way can access our whiteboard to publish/listen
 * and the Activity Context in order to invoke C++ code.
*/
	guWhiteboard::Whiteboard *getWhiteboard() { return &wb; }
	pthread_mutex_t *getMutex() { return &mutex; }
	void *getActivityContext() { return activity_context; }

};
