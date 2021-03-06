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

#include "SubMachine.h"
#include "gumessagecontent.h"
#include <cerrno>

//************************
// The constructor
//************************
fsmSubMachine:: fsmSubMachine (string theFileName, std::map<std::string,
				fsm_callback_f> *named_functions  , void *context  )
		:fsmMachine(theFileName, named_functions,context)

	{
		DBG(cout << "Constructing Submachine " << theFileName << endl);
	
		pthread_mutex_init(&stop_resume_mutex,NULL);

		// set up flag to communicate between threads.
		if (0 != sem_init(&forNotify,0,0))
        	{ cerr << "cannot init semaphore: " << strerror(errno) << endl;
        	}

		firstTime=true;
		
		guWhiteboard::Whiteboard::WBResult r;
		wb.subscribeToMessage(theFileName,
                                       WB_BIND(fsmSubMachine::onWhiteboardMessageResumeOrStop), r);

	}

//************************
// The endless cycle of executing or dormant
//************************

void fsmSubMachine::executeNsleep()
{
	while (1)
	{ fsmMachine::execute();  // we do the FSM (parent class) execute
	  iAmRunning=false;    // we came out of a final state
	  dormant();     		// we sleep	
	}
}

void fsmSubMachine::startDormant()
{
	fsmSubMachine::setUp();	// we do our set up; BUT
	dormant();
	executeNsleep(); // we execute the cycle of the Parent Class and if finished we seelp
}

	// go to sleep until a notify
void fsmSubMachine::dormant()
{
     int err;
     if ( 0!= (     err=  sem_wait(&forNotify) ))
        { cerr << "cannot wait for semaphore: " << strerror(errno) << endl;
        }
}

//************************
// The re-definitions / over-writing
//************************

void fsmSubMachine::setUp()
{
	firstTime=false; // we are no longer a first time run
	fsmMachine::setUp();  // we do the FSM (parent class) setUP
}

void fsmSubMachine::run()
{
	fsmSubMachine::setUp();	// we do our set up; BUT
	executeNsleep(); // we execute the cycle of the Parent Class and if finished we seelp
}

//************************
// The call backs
//************************

void fsmSubMachine::onWhiteboardMessageResumeOrStop(std::string dataName, WBMsg *msg)
{

	pthread_mutex_lock(&stop_resume_mutex);
	string askingFor;

       /* Extract the Message          */

	MessageContent theMessageContent(msg);
	askingFor=theMessageContent.theContent();

	
	ostringstream text;
        if (msg->getType() == WBMsg::TypeString)
                text << msg->getStringValue();
        else if (msg->getType() == WBMsg::TypeInt)
                text << msg->getIntValue();
        else if (msg->getType() == WBMsg::TypeFloat)
                text << msg->getFloatValue();
        else if (msg->getType() == WBMsg::TypeBool)
                text << ( msg->getBoolValue() ? "Yes." : "No." );
        else text << msg->getSizeInBytes() << " bytes of gibberish";

        askingFor= text.str();
	

        DBG (cout << "Message Received ++++++>" << askingFor << endl);


	if (0==askingFor.compare(SUBMACHINE_ON))
	{

		if (!iAmRunning) // additional messages to run while running should be ignored
		{ shallIResume=true;
	  	  iAmRunning=true;

		  DBG(cout << "Resuming Submachine " <<  endl);

	  	  if (firstTime) { fsmSubMachine::run();} // our run makes sure we change the flag
	  	  else // need to do wake from being dormant
			{   // a notify
     			int err;
			if (0!= (err= sem_post(&forNotify) ))
        			{ cerr << "cannot post semaphore: " << strerror(errno) << endl;
        			}
			}
		}
	} //end of case when we Rseum
	else // we stop
	if (0==askingFor.compare(SUBMACHINE_OFF))
	{
		DBG(cout << "Suspending Submachine " <<  endl);
		// the loop in execute will stop.
		shallIResume=false;
		iAmRunning=false;
	}
	pthread_mutex_unlock(&stop_resume_mutex);

}
