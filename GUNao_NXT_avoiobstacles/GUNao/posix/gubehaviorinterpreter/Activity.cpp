/*
 *  Activity.cpp
 *  gubehaviourinterpreter
 *
 *  Created by Rene Hexel on 1/06/10.
 *  Copyright 2010 Rene Hexel. All rights reserved.
 *
 */
#include <ctype.h>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include "Activity.h"

using namespace std;

const string SECTIONSEPARATOR="/";
const string ACTIVITYSEPARATOR=";";
const string MESSAGESEPARATOR=":";
const string PARAMSTART="(";
const string PARAMEND=")";
const string ONENTRY="OnEntry";
const string ONEXIT="OnExit";
const string BLANKS=" \t\r";

bool fsm_activity_sleep(fsmMachine *, void *, const char *param)
{
        long value = (long) param;

        if (value <= 0) value = 1;

	std::cerr << "Sleeeping " << value << std::endl;
	sleep(value);

	return true;
}


static char *fsm_strdup(const char *s)
{
        int n = 1;

        if (s) n = strlen(s)+1;

        char *dest = static_cast<char *>(calloc(1, n));

        if (s && dest) strcpy(dest, s);

        return dest;
}


void fmsActivity::addOnePostToPossiblePostsByStage
                (ActivityStages stage, p_onePost newPost)
        {    
             switch (stage)
                { case STAGE_ON_ENTRY:  postingOnEntry.addOnePost(newPost);
                        break;
                  case STAGE_ON_EXIT: postingOnExit.addOnePost(newPost);
                        break;
                  case STAGE_INTERNAL: postingInternal.addOnePost(newPost);
                        break;
                }
        }



bool fmsActivity::fmsActivityParsing(ActivityStages stage, std::string theInput,
                          std::map<std::string, fsm_callback_f> *table)
{
	//cerr << "Extracting Activities from:" << theInput << "\n";
	do {
	  int semiColonPos = theInput.find_first_of(ACTIVITYSEPARATOR);
	  if (semiColonPos == string::npos) //there is no colon
		break;
	  string item = theInput.substr(0, semiColonPos);
		int firstAlpha = item.find_first_not_of(BLANKS);
		item=item.erase(0,firstAlpha);
	  p_onePost newPost = new onePost();
	  int colonPos = item.find_first_of(MESSAGESEPARATOR);
	  if (colonPos == string::npos) //there is no colon
	     {
                newPost-> setDoing(item);
		newPost->setMessageType("");
	        addOnePostToPossiblePostsByStage(stage,newPost);

             }
	   else  // there is a value
            {   string action = item.substr(0, colonPos);
													  
	        string content = item.substr(colonPos+1, semiColonPos-colonPos-1);
				firstAlpha = content.find_first_not_of(BLANKS);
				content=content.erase(0,firstAlpha);
				
	        if ("C++"==action)
                {
                        char *param = NULL;
                        int bracketPos = content.find_first_of(PARAMSTART);
                        int closingPos = content.find_first_of(PARAMEND);

                        if (bracketPos != string::npos)
                        {
                                if (closingPos == string::npos ||
                                    closingPos <= bracketPos)
                                        cerr << "content '" << content <<
                                                "' not well formed, trying to ignore parameter"
                                             << endl;
                                else
                                {
                                        string p = content.substr(bracketPos+1,
                                                                  closingPos-bracketPos-1);
                                        if (isdigit(p[0]) ||
                                            p[0] == '-'   ||
                                            p[0] == '+')
                                                param = (char *) atol(p.c_str());
                                        else if (p[0] == '"' && p.length() > 1)
                                        {
                                                param = fsm_strdup(p.c_str()+1);
                                                int n = p.length()-1;
                                                if (param[n] == '"')
                                                        param[n] = '\0';
                                        }
                                        else
                                        {
                                                param = fsm_strdup(p.c_str());
                                                cerr << "Warning: unquoted string '"
                                                     << param << "' as a parameter in "
                                                     << content << endl;
                                        }
                                        content = content.erase(bracketPos);
                                }
                        }
                        if (table)
                           { switch (stage)
                		{ case STAGE_ON_ENTRY:
                                        onEntry = (*table)[content];
                                        onEntryParam = param;
                        		break;
                  		  case STAGE_ON_EXIT:
                                        onExit = (*table)[content];
                                        onExitParam = param;
                        		break;
                  		  case STAGE_INTERNAL:
                                        myActivity = (*table)[content];
                                        activityParam = param;
                        		break;
                	    } //switch
                           }//if
			else
			{
				cerr << "Parsing C++ and BAD MACHINE: Afile.txt \n";
				cerr << "Content :"<< content << endl;
				return false;
			}
                  } 
                else
		  { newPost-> setDoing(content);
		    newPost->setMessageType(action);
	            addOnePostToPossiblePostsByStage(stage,newPost);
                  }
            }
	  //newPost->print();
	  theInput=theInput.substr(semiColonPos+1);
	} while (theInput.length()>0 );

	return true;
}

fmsActivity::fmsActivity (string theThingToDo,
                          std::map<std::string, fsm_callback_f> *table):
			myActivity(fsm_activity_sleep), onEntry(NULL), onExit(NULL),
                        activityParam(NULL), onEntryParam(NULL), onExitParam(NULL),
			   postingOnEntry(),
			   postingOnExit(),
			   postingInternal()
{
	//cerr << "Parsing>" << theThingToDo << "\n";

        int startOnEntry=theThingToDo.find(ONENTRY);
	if (string::npos!=startOnEntry)
	   {    theThingToDo=theThingToDo.substr(startOnEntry+ONENTRY.size());
		//cerr << "Processing <On Entry> :" << theThingToDo << "\n";
		int endOnEntryPart = theThingToDo.find_first_of(SECTIONSEPARATOR);
		string onEntryPart = theThingToDo.substr(0,endOnEntryPart);
		// process the OnEntryPart
		fmsActivityParsing(STAGE_ON_ENTRY,onEntryPart,table);

		// advance the parsing
		theThingToDo=theThingToDo.substr(endOnEntryPart+1);
	   }

	//cerr << "Parsing>" << theThingToDo << "\n";

        int startOnExit=theThingToDo.find(ONEXIT);
	if (string::npos!=startOnExit)
	   {    theThingToDo=theThingToDo.substr(startOnExit+ONEXIT.size());
		//cerr << "Processing <On Exit> :" << theThingToDo << "\n";
		int endOnExitPart = theThingToDo.find_first_of(SECTIONSEPARATOR);
		string onExitPart = theThingToDo.substr(0,endOnExitPart);
		// process the OnEntryPart
		fmsActivityParsing(STAGE_ON_EXIT,onExitPart,table);
		// advance the parsing
		theThingToDo=theThingToDo.substr(endOnExitPart+1);
	   }

	//cerr << "Parsing>" << theThingToDo << "\n";
	//cerr << "Processing <Internal> :" << theThingToDo << "\n";
	fmsActivityParsing(STAGE_INTERNAL,theThingToDo,table);


}

