/*************************************************************************
//	Class of Array of Posts in the Acivities of States of a finite 
//	state machine
//	(c) 2010 Mi-PAL Laboratory, Griffith University
//	Author: Vladimir Estivill-Castro
//	-----------------------------------------------
//	This code is released under GPL v3.0
//	You should have recieved a copy of the GNU General
//	Public License along with this program. If not, see
//	<http://www.gnu.org/license/> 
*************************************************************************/


#include <string>
#include <iostream>
#include <sstream>
//#include <gu_util.h>

using namespace std;


class onePost
{

	std::string doing;
	std::string messageType;

public:
	onePost (){}

	std::string getTheDoing() const {return doing;}

	std::string getTheMessageType() const {return messageType;}

	void setDoing (std::string theDoing) {doing=theDoing;}

	void setMessageType (std::string theMessageType) {messageType=theMessageType;}

	void print() {
		cerr << "Type: " << getTheMessageType()
		     << " Doing " << getTheDoing() << endl;
	/*
		DBG(cerr << "Type: " << getTheMessageType()
		     << " Doing " << getTheDoing() << endl);
	*/
	}
};

