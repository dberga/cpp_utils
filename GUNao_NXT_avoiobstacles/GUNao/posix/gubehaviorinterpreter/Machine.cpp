//*************************************************************************
//	Class of Activitiees in the States of a finite state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro and Rene Hexel
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have recieved a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/>
//
//*************************************************************************/

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <libgen.h>

#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cerrno>

#include <gu_util.h>
//#include <cdlbridge_wbmsg.h>

#include "Expression.h"
#include "Machine.h"

using namespace std;
//using namespace CDL;
using namespace guWhiteboard;

	//transitions with a TRUE alwasy fire and we should not subscribe to them
const string SURELY_FIRES ="TRUE";

	// note the ~ at the end
const string ALPHAT ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~";
const string DIGIT ="0123456789";
const string ALPHAT_DIGIT ="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz~0123456789";
const string SPACE_SEPARATOR=" \t";
const string NAMESEPARATOR ="|";
const int LINE_SIZE=700;

/************************************************************************/
/*	THE HELPERS							*/
/************************************************************************/

void fsmMachine::postAllMessagesForStage(ActivityStages stage, fmsActivity *activity)
{
	Whiteboard::WBResult r;
	p_onePost toIterate;
	
	toIterate=activity->getFirstWhatToDo(stage);
	
	while (NULL!=toIterate)
	{
		string s(toIterate->getTheDoing() );
		//WBMsg msg(s);
		
		 DBG(cout <<  "  ==> Posting " << toIterate->getTheMessageType() );
		 DBG(cout <<  "  and content " << s << endl);
		wb.addMessage(toIterate->getTheMessageType(), WBMsg(s));
		//wb.addMessage(toIterate->getTheMessageType(), &msg, 1, r);
		toIterate=activity->getNextWhatToDo(stage);
	}
}


static void outputActivityByStage(ActivityStages stage, fmsActivity* theActivity)
{

	switch (stage)
                { case STAGE_ON_ENTRY: 
			DBG(cout << "ON ENTRY activities" << endl);
                        break;
                  case STAGE_ON_EXIT:
			DBG(cout << "ON EXIT activities" << endl);
                        break;
                  case STAGE_INTERNAL:
			DBG(cout << "INTERNAL activities" << endl);
                        break;
                }

        p_onePost toIterate;

        toIterate=theActivity->getFirstWhatToDo(stage);

	while (NULL!=	toIterate)
                      {
		  	DBG(cout <<  
		  	toIterate->getTheDoing()
                           << " of type " <<
		           toIterate->getTheMessageType() << endl);
                        toIterate=theActivity->getNextWhatToDo(stage);
                      }
}

static istream * open_file (string filename, filebuf * aFile)
{
	char *c_string;
	c_string = new char[filename.size()+1];
	strcpy(c_string, filename.c_str() );
	aFile->open(c_string,ios::in);
	delete[] c_string;

	if (aFile->is_open() )
	  { istream * theResult= new istream (aFile);
	    return theResult;
	  }
	else
	  {  cerr << "NAME UNKNOWN : "+filename+" emergency exit";
	     return NULL;
	     exit (EXIT_FAILURE);
	  }
}

bool fsmMachine :: exists_already (int theID)
{
	vector <fsmState*>::iterator  it;
	for (it=theStates.begin() ; it<theStates.end() ; it++)
	 if ( (*it)->getID() == theID) return true;
	return false;
}

fsmState* fsmMachine :: find(int theID)
{
	vector <fsmState*>::iterator  it;
	for (it=theStates.begin() ; it<theStates.end() ; it++)
	 if ( (*it)->getID() == theID) return (*it);
	return NULL;
	using namespace guWhiteboard;
}

/************************************************************************/
/*	THE CONSTRUCTOR							*/
/************************************************************************/

fsmMachine :: fsmMachine (string theFileName,
			  std::map<std::string, fsm_callback_f> *named_functions,
			  void *context): theStates(), callBackValue(false),
					  wantProof(false)
{
	shallIResume=true;

	activity_context = context;
	if (named_functions)
		init_context = (*named_functions)["initContext"];
	else
        {
                named_functions = new std::map<std::string, fsm_callback_f>;
		init_context = NULL;
        }
        (*named_functions)["sleep"] = fsm_activity_sleep;

	// Add files that end in .sdump to be scanned with the target filename
	string str=theFileName;
	str+=string(".sdump");
	 if ( ! load_theory ( (char *) str.c_str() ) )
		exit(EXIT_FAILURE) ;
	size=0;

        //cout<< "THE FILE NAME:" << theFileName << endl;

	istream *is_activities;
	istream *is_transitions;
	// Add and A and a T infront of the filename to scan the Activity and Transition files
	string activitiesFilename("A");
	string transitionFilename("T");
	// Add .txt at the end of the appended +A and +T files so that it reads the .txt files
     	theFileName+=string(".txt");
	activitiesFilename+=theFileName;  
	transitionFilename+=theFileName;

	filebuf bActivities, bTransitions;

	 if ( ( is_activities=open_file(activitiesFilename,&bActivities) ) == NULL )
		exit(EXIT_FAILURE);
	 if ( ( is_transitions=open_file(transitionFilename,&bTransitions) ) == NULL )
	        exit(EXIT_FAILURE);

	char aLine[LINE_SIZE];
	string theStateName;

	// While scaning the States and their Declartion
	while (is_activities->getline (aLine,LINE_SIZE) )
	  { 	/* Parse an int id, then an ALPHA starting string */
		string s(aLine);
		int first_digit = s.find_first_of(DIGIT);
		s=s.substr(first_digit);	// move to first digit

		char *c_string;
		c_string = new char[s.size()+1];
		strcpy(c_string, s.c_str() );
		int theID = atoi (c_string);
		delete[] c_string;

		int first_alpha = s.find_first_of(ALPHAT);
		s=s.substr(first_alpha);

		int first_separator = s.find_first_of(NAMESEPARATOR);
		if (string::npos!= first_separator) //this state has a name
		 { theStateName = s.substr(0,first_separator);
		   s=s.substr(first_separator+1); //skip the name
		 }
                else
		{ theStateName =string("NONAME");
		}

		fmsActivity * theActivity = new fmsActivity(s,named_functions);

		// check to see if this ID has not been used for an earlier state */
	
		if (exists_already (theID ) )
		{ cerr << "BAD ACTIVITY FILE: State duplicated \n";
		  exit (EXIT_FAILURE);
		}
		else
		{ size++;

		  DBG(cout << " State ID " << theID << 
		      " Named :" << theStateName << endl <<
                      " Activity Summary :" << endl);

                 outputActivityByStage(  STAGE_ON_ENTRY,  theActivity);
                 outputActivityByStage(  STAGE_ON_EXIT,  theActivity);
                 outputActivityByStage(  STAGE_INTERNAL,  theActivity);

		  fsmState* newState = new fsmState(theID, theStateName, theActivity);
		  vector <fsmState*>::iterator  it;
		  it=theStates.begin();
		  theStates.insert(it,newState);
		}
	  } 

	// While scanning the Transitions
	while (is_transitions->getline (aLine,LINE_SIZE) )
	  { 	/* Parse an int id, then an ALPHA starting string, then an int */
		string s(aLine);
		int first_digit = s.find_first_of(DIGIT);
		s=s.substr(first_digit);	// move to first digit

		char *c_string;
		c_string = new char[s.size()+1];
		strcpy(c_string, s.c_str() );
		int theSource = atoi (c_string);
		delete[] c_string;

		int first_alpha = s.find_first_of(ALPHAT);
		s=s.substr(first_alpha); 	// move to first alpha

		int last_alpha = s.find_first_of(SPACE_SEPARATOR);

		fsmExpression * theExpression = new fsmExpression(s.substr(0,last_alpha ) );
		s=s.substr(last_alpha);	// move to end of ALPHAT_DIGITS

		int second_digit = s.find_first_of(DIGIT);
		s=s.substr(second_digit);	// move to second digit

		c_string = new char[s.size()+1];
		strcpy(c_string, s.c_str() );
		int theTarget = atoi (c_string);
		delete[] c_string;

		//cerr << " Source State : " << theSource << " Logic expresson " << theExpression->getWhatToEvaluate() << " the taget " << theTarget << "\n";

		if (exists_already (theSource) && exists_already (theTarget))
		 { //source and target properly defined in activity file

		    fsmState * p_theSource = find(theSource);

		    //cerr << "The source ID foind is : " << p_theSource->getID() << "\n";

		    fsmTransition* theTransition = new fsmTransition( p_theSource,
						   find(theTarget),
					           theExpression
						 );
		    p_theSource->addTransition(theTransition);

		 }
		else
		 { cerr << "ERROR: Transition with ID of State not defined in activity file\n";
		   exit(EXIT_FAILURE);
		 }

	  } 

}

void fsmMachine::My_Callback(std::string dataName, WBMsg *msg)
{
	int err;

	pthread_mutex_lock(&mutex);
	int val;
  	if (0!= (err= sem_getvalue(&forWaitNotify, &val) )) 
	{
		cerr << "cannot post semaphore: " << strerror(errno) << endl;
		pthread_mutex_unlock(&mutex);
		return;
	}
	if (!wantProof)
	{
		DBG(cerr<<"wantProof value is " << wantProof << " -- discarding callback" << endl);
		pthread_mutex_unlock(&mutex);
		return;	// ignore rogue messages
	}

	// should check thet the message if for the output i asked for
	// If it is for the one waiting for, do what is here
	// otherwise ignore this theard and DO NOT wake up main tread
	fsmExpression *waitExpression = whatIamWaitingfor;
	
	if ( !waitExpression || waitExpression->atom4Expression()!=dataName)
	{
		DBG(cerr<<"wait4 is not " << dataName << " -- discarding callback" << endl);
		pthread_mutex_unlock(&mutex);
		return;	// ignore non-matching messages
	}

	// set the value
	bool value = msg->getType() == WBMsg::TypeBool && 
			msg->getBoolValue();
	if ( waitExpression->isNegation() )
		callBackValue=!value;
	else
		callBackValue=value;

		//wake up the main thread in execute, doProof

	DBG(cerr<<"* waking up main thread for " << dataName << " msgval = " << value << ", callbackval = " << callBackValue << endl);

	wantProof = false;

    	if (0!= (err= sem_post(&forWaitNotify) )) 
	{
		pthread_mutex_unlock(&mutex);
		cerr << "cannot post semaphore: " << strerror(errno) << endl;
		return;
	}
	pthread_mutex_unlock(&mutex);
}

void fsmMachine :: setUp ()
{

		// WHITEBOARD set up
	if (0 != sem_init(&forWaitNotify,0,0))
	{
		cerr << "cannot init semaphore: " << strerror(errno) << endl;
		return;
	}

	// whiteboard control
	Whiteboard::WBResult r;
		// Theory set up, link to CDL bridge
	// temporary do not use theories, just use whiteboard Vlad Nov 4th 2013
	//DBG(cerr<<"* First Mesaage to Whitebaord => Contnet :" << kUploadTheory << " WBMsg= " << theory_name  << endl);
       //wb.addMessage(kUploadTheory, WBMsg(theory_name), 1);
		// Communciation to SET UP
		// from our input files we subscribe to all outputs
		// Run trough all transtition of all states
	subscribeToAllOutputs(wb);
	if (init_context && !init_context(this, activity_context, theory_name.c_str()))
	{
		cerr << "Machine Initialized failed" << endl;
		exit(EXIT_FAILURE);
	}
}

void fsmMachine :: run ()
{
	setUp();
	execute ();
}

void fsmMachine :: execute ()
{

		// MACHINE  set up
	vector <fsmState*>::iterator it;
	it=theStates.begin();
	fsmState* current = (*it);
	fsmState* previous = current;

	int currentID = current -> getID();
	
	DBG(cout << "** INITIAL State is State Number " << current->getID()<< endl);

	bool transitionFired = true;

	while (shallIResume) // run for ever
	{
		// Evaluate labels of transitions going out of current state
		// and may change state
		p_fsmTransition p_itTransitions;
		if (NULL == (p_itTransitions = current->theFirstTransition()))
		   {shallIResume=false; // no transitions out, shall go to a dormat thread
				       // for submachines and hals for superclass
		   }

		// ostringstream will allow my input event to change into a string and
		// then transfer to the whiteboard speech module to speak the event
		if( transitionFired ) // when fired do the posting
		{
			// Output the event and state to the user
			DBG(cerr << "\nAfter evalaution the state is :" <<find(current->getID())->getID() );
			DBG(cerr  <<find(current->getID())->getName() << "\n");
			cerr  <<find(current->getID())->getName() << "\n";
			//cout  << " We are `" << ( current->getActivity() )->getWhatToDo() << "'\n";
			
			// posting to  whiteboard 

			fmsActivity *activity = current->getActivity();
			postAllMessagesForStage(STAGE_ON_ENTRY, activity);

			// On Entry Activity
			if (current->getActivity()->getOnEntry() )
			{
				if (!(current->getActivity()->getOnEntry()) (this, activity_context, activity->getOnEntryParam()))
				{
					cerr << "OnEntry returned false, exiting!" << endl;
					exit(EXIT_FAILURE) ;
				}
			}
			else
			{
				DBG(cerr << "No OnEntry for state" << endl);
			}
		}
		
		transitionFired = false;

		while ((!transitionFired) &&  (NULL!= p_itTransitions))
		{
		    char response;

		     //cout << "Does this expression evaluate to true (Y/N)?\n";
                    response= doProof( 
		  ((p_itTransitions)->getExpression() ) 
		 );
  
		     //cin >> response;
		     if (response == 'Y') // we need to execute the transition
			{
				// set the new state
				previous = current;
				current = p_itTransitions->getTarget();
				currentID=current->getID();
				// break out
				transitionFired = true;
			}
		     else // advance to next transition
			{
				p_itTransitions = current ->theNextTransition();
			}
		} // of != NULL

		// if the transitions fired, do the exit
		if (transitionFired)
		{
			fmsActivity *activity = previous->getActivity();
			postAllMessagesForStage(STAGE_ON_EXIT, activity);
			
			if (activity->getOnExit() )
			{
				if (!(activity->getOnExit()) (this, activity_context, activity->getOnExitParam()))
				{
					cerr << "OnExit returned false, exiting!" << endl;
					exit(EXIT_FAILURE) ;
				}
			}
			else
			{
				DBG(cerr << "No OnExit for state" << endl);
			}
		}
		else // no transition fired: do the activity
		{
			fmsActivity *activity = current->getActivity();
			postAllMessagesForStage(STAGE_INTERNAL, activity);
			
			if (activity->getActivity() )
			{
				if (!(activity->getActivity()) (this, activity_context, activity->getActivityParam()))
				{
					cerr << "Activity returned false, exiting!" << endl;
					exit(EXIT_FAILURE) ;
				}
			}
			else
			{
				sleep(1);
				DBG(cerr << "No activity for state" << endl);
			}
		}

		// send message to Actuators of My Activity by posting to whitebaord

	}
}

char fsmMachine:: doProof(fsmExpression *output)
{

	// if the expression is 'TRUE', return a 'Y' to continue
	//if (getWhatToEvaluate().isTrue() ) return 'Y';
	if (0==output->getWhatToEvaluate().compare(SURELY_FIRES) ) return 'Y';
#ifdef DEBUG
	 DBG(cout << "Evaluate :" << output->getWhatToEvaluate() << "<*\n");
#elif !defined(NDEBUG)
	putchar('.'); fflush(stdout);
#endif
	 cout << "Evaluate :" << output->getWhatToEvaluate() << "<*\n";
	putchar('.'); fflush(stdout);
	putchar('.'); fflush(stderr);

	 whatIamWaitingfor=output;
	wantProof = true;
	// ask for only to do the proof of this output
	// UPDATE POOFs will be repalced by look up into teh whiteboard
	//wb.addMessage(kUpdateProofsForOutput, WBMsg(output->atom4Expression() ), 1);

	int err;
         if ( 0!= (	err=  sem_wait(&forWaitNotify) ))
	{
		cerr << "cannot wait for semaphore: " << strerror(errno) << endl;
		return 'n';
	}

		// The callback does the job of extracting
		// mesage from wb and setting callback value
         if (callBackValue)
        {
        	return ('Y');
        }
         else
        {
		return ('n');
        }
}

bool fsmMachine :: load_theory(char *file)
{
        try
        {
                theory_content = string_from_file(file);
                string filename(basename(file));
                int offs = filename.find('.', 0);
                theory_name = filename.substr(0, offs);
                theory_name += '\\' + theory_content;

		return true;

        }
        catch (...)
        {
                cerr << "unable to load " << file << endl;
		return false;
        }
}

void fsmMachine::subscribeToAllOutputs(Whiteboard &wb)
{
	Whiteboard::WBResult r;
	p_fsmTransition p_itTransition;
	vector <fsmState*>::iterator it;
	it=theStates.begin();

	for( fsmState* current = (*it); it!=theStates.end(); current = (*++it))
	{
		p_itTransition = current->theFirstTransition();
		 while (p_itTransition!=NULL)
	 	{ 
		   // do not subscribe to TRUE
		string whatToSubscribe= p_itTransition->getExpression()->atom4Expression();

		  if (0 !=whatToSubscribe.compare( SURELY_FIRES))
			{
                     wb.subscribeToMessage( whatToSubscribe ,WB_BIND(fsmMachine::My_Callback)  , r  );
		        }
	   	  p_itTransition = current ->theNextTransition();
         	}
	} // for States
}
