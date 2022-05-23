#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <string>

#include "Whiteboard.h"
#include "WBMsg.h"

using namespace std;

class reactiveController
{
  
 //Attributes declaration 
 private:
    int environmentCondition[4]; //default values
	bool leftBumperPressed;
	bool rightBumperPressed;
	bool sonarDistanceReached;
	bool surfaceIsUnUsual;
	int surface; //start default value
	guWhiteboard::Whiteboard *wb; //Whiteboard class instance(note that the constructor of Whiteboard class is not called yet)
	char findSensorID(std::string dataName);
 
 public:
 reactiveController();//Class constructor

  //Methods:
 bool getleftBumperPressed();
 bool getrightBumperPressed();
 bool getsonarDistanceReached();
 bool getsurfaceIsUnUsual();
 int getenvironmentCondition(int position);
 int getsurface();

 void setleftBumperPressed(bool way);
 void setrightBumperPressed(bool way);
 void setsonarDistanceReached(bool way);
 void setsurfaceIsUnUsual(bool way);
 void setenvironmentCondition(int position, int way);
 void setsurface(int dist);

 void setUpreactiveController();
 void runforward();
 void runbackward();
 void turnleft();
 void turnright();
 
 //Call back methods
virtual void callBackTouch(std::string dataName, WBMsg *msg);
virtual void callBackSonar(std::string dataName, WBMsg *msg);
virtual void callBackSurface(std::string dataName, WBMsg *msg);

  
  
};


