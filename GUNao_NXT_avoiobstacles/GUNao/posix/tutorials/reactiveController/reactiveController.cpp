

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include <string>
#include "reactiveController.h"
#include "gumessagecontent.h" 
#include "gunxtbridge_wbmsg.h"  
#include "Whiteboard.h"
#include "gunxtconstants.h"
#include "pthread.h" 



using namespace guWhiteboard;
using namespace std;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;



	//1.if no signal, robot moves forward //point 1
	//2.if right bumper pressed, go backwards and then turn to the left and keeps going forward //point 2
	//3.if left bumper pressed, go backwards and then turn to the right and keeps going forward //point 3
	//4.if sonar distance reached, go backwards and then turns randomly and keeps going forward //point 4
	//5.if surface is unusual, go backwards and then turns around and keeps going forward //point 5
	
	//leftBumperPressed-> environment="0XXX" if false, "1XXX" if true
	//rightBumperPressed-> environment="X0XX" if false, "X1XX" if true
	//sonarDistanceReached-> environment="XX0X" if false, "XX1X" if true
	//surfaceIsUnUsual-> environment="XXX0" if false, "XXX1" if true


 reactiveController::reactiveController()
 {
    wb = new Whiteboard();
    environmentCondition[0]=0;
    environmentCondition[1]=0;
    environmentCondition[2]=0;
    environmentCondition[3]=0;
    leftBumperPressed=false;
    rightBumperPressed=false;
    sonarDistanceReached=false;
    surfaceIsUnUsual=false;
    surface = 999; //token value
    cout << "Proba reactiveController" << endl;
    cout.flush();
 }


void reactiveController::setUpreactiveController()
{
   Whiteboard::WBResult wr;//WBResult
   
		for (theSensors i=SENSOR_ONE; i<=SENSOR_FOUR; i++)
	     {
		char theID = NXT_SENSOR_NAMES::getName(i) ;
		string sensorID(1, theID );
	
                cout << " ** Subscriptions for Sensor ID == " << sensorID << endl;
			wb->subscribeToMessage("gunxt_valueTouchSensor"+sensorID, WB_BIND(reactiveController::callBackTouch) , wr);
	    	wb->subscribeToMessage("gunxt_valueKFilteredLightSensor"+sensorID, WB_BIND(reactiveController::callBackSurface) , wr);
	    	wb->subscribeToMessage("gunxt_valueKFilteredSonarSensor"+sensorID, WB_BIND(reactiveController::callBackSonar) , wr);
		}
  
}

void reactiveController::runforward(){
	wb->addMessage("gunxt_runForwardMotor",WBMsg("B,50"),true);
	wb->addMessage("gunxt_runForwardMotor",WBMsg("C,50"),true);
}

void reactiveController::runbackward(){
	wb->addMessage("gunxt_runBackwardMotor",WBMsg("B,25"),true);
	wb->addMessage("gunxt_runBackwardMotor",WBMsg("C,25"),true);
}

void reactiveController::turnleft(){
	wb->addMessage("gunxt_runBackwardMotor",WBMsg("B,25"),true);
	wb->addMessage("gunxt_runForwardMotor",WBMsg("C,50"),true);
}

void reactiveController::turnright(){
	wb->addMessage("gunxt_runForwardMotor",WBMsg("B,50"),true);
	wb->addMessage("gunxt_runBackwardMotor",WBMsg("C,25"),true);
}

bool reactiveController::getleftBumperPressed(){
return leftBumperPressed;
}
 bool reactiveController::getrightBumperPressed(){
return rightBumperPressed;
}
 bool reactiveController::getsonarDistanceReached(){
return sonarDistanceReached;
}
 bool reactiveController::getsurfaceIsUnUsual(){
return sonarDistanceReached;
}
 int reactiveController::getenvironmentCondition(int position){
return environmentCondition[position];
}
int reactiveController::getsurface(){
return surface;
}

 void reactiveController::setleftBumperPressed(bool way){
leftBumperPressed = way;
}
 void reactiveController::setrightBumperPressed(bool way){
rightBumperPressed = way;
}
 void reactiveController::setsonarDistanceReached(bool way){
sonarDistanceReached = way;
}
 void reactiveController::setsurfaceIsUnUsual(bool way){
surfaceIsUnUsual = way;
}
 void reactiveController::setenvironmentCondition(int position, int way){
environmentCondition[position] = way;
}

void reactiveController::setsurface(int dist){
 surface =dist;
}


//My call back methods
void reactiveController::callBackTouch(std::string dataName, WBMsg *msg)
{
			pthread_mutex_lock( &mutex1 );	

			MessageContent theMessageContent (msg);

			int content = theMessageContent.theContentToInt();
			string sensorID(1, findSensorID(dataName) );
			stringstream converts1;
			stringstream converts2;
			converts1 << SENSOR_1;
			converts2 << SENSOR_2;
			if(sensorID == converts1.str() && content==1){
				setleftBumperPressed(true);
				cout << "leftBumper has been Pressed " << endl;
			}else if(sensorID == converts2.str() && content==1){
				setrightBumperPressed(true);
				cout << "rightBumper has been Pressed " << endl;
			}else{
				setleftBumperPressed(false);
				setrightBumperPressed(false);
			}
			
			pthread_mutex_unlock( &mutex1 );
}


void reactiveController::callBackSonar(std::string dataName, WBMsg *msg)
{
			pthread_mutex_lock( &mutex1 );
				
			MessageContent theMessageContent (msg);

			int content = theMessageContent.theContentToInt();
			
			if(content < 25){
				setsonarDistanceReached(true);
				cout << "Sonardistance has been reached" << endl;
			}else{
				setsonarDistanceReached(false);
			}
			 
			pthread_mutex_unlock( &mutex1 );
}


void reactiveController::callBackSurface(std::string dataName, WBMsg *msg)
{
			pthread_mutex_lock( &mutex1 );
				
			MessageContent theMessageContent (msg);

			int content = theMessageContent.theContentToInt();
			
			if(getsurface() == 999){
				setsurface(content);
			}else{
				if(content == getsurface()){
					setsurfaceIsUnUsual(true);
					cout << "Surface value is unusual " << endl;
				}else{
					setsurfaceIsUnUsual(false);
				}
				setsurface(content); //remembering the usual value
			}
						
			pthread_mutex_unlock( &mutex1 );
}


char  reactiveController::findSensorID(std::string dataName )
{

	cout << " ** dataName == " << dataName << endl;
	char theID=SENSOR_1;
	
	for (theSensors i=SENSOR_ONE; i<=SENSOR_FOUR; i++)
	     {
		theID = NXT_SENSOR_NAMES::getName(i) ;
		//* if foudn break the for*/
	        if (string::npos!=  dataName.find(theID)) break;
	     }

	return theID;

}




int main(int argc, char **argv)
{

  reactiveController *hw = new reactiveController();//reactiveController constructor
  hw->setUpreactiveController();//Set Up method call
  
  while(1){ //environment check
	usleep(10000);
	
	//checking environment
	if(hw->getleftBumperPressed()) hw->setenvironmentCondition(0,1); else hw->setenvironmentCondition(0,0);
	if(hw->getrightBumperPressed()) hw->setenvironmentCondition(1,1); else hw->setenvironmentCondition(1,0);
	if(hw->getsonarDistanceReached()) hw->setenvironmentCondition(2,1); else hw->setenvironmentCondition(2,0);
	if(hw->getsurfaceIsUnUsual()) hw->setenvironmentCondition(3,1); else hw->setenvironmentCondition(3,0);
	
	//respoding to conditions
		switch(hw->getenvironmentCondition(0)){ //touch left
			case 1:{
				cout << "Response on touch: go back, move left and keep going forward" << endl;
				hw->runbackward();
				hw->turnleft();
				hw->runforward();
				break;
			}case 0:{
				hw->runforward();
				break;
			}default:
				cout << "Error checking environment of touch " << endl;
		}
		
		switch(hw->getenvironmentCondition(1)){ //touch right
			case 1:{
				cout << "Response on touch: go back, move right and keep going forward" << endl;
				hw->runbackward();
				hw->turnright();
				hw->runforward();
				break;
			}case 0:{
				hw->runforward();
				break;
			}default:
				cout << "Error checking environment of touch " << endl;
		}
		
		switch(hw->getenvironmentCondition(2)){ //sonar
			case 1:{
				cout << "Response on sonar reached: go back, turn around and keep going forward" << endl;
				hw->runbackward();
				
					int randomvalue1 = rand() % 2;
					int randomvalue2 = rand() % 2 + 1;
					for(int i=0; i<randomvalue1;i++){
						hw->turnright();
					}
					for(int i=0; i<randomvalue2;i++){
						hw->turnleft();
					}
				hw->runforward();
				break;
			}case 0:{
				hw->runforward();
				break;
			}default:
				cout << "Error checking environment of sonar " << endl;
		}
		
		switch(hw->getenvironmentCondition(3)){ //light
			case 1:{
					
				cout << "Response on sonar reached: go back, turn around and keep going forward" << endl;
				hw->runbackward();
				hw->turnright();
				hw->turnright();
				hw->runforward();
				break;
			}case 0:{
				hw->runforward();
				break;
			}default:
				cout << "Error checking environment of light " << endl;
		}
	
  }
  
  
  //This is not recommended, but now we need to non stop the running on the Whiteboard
   //while(1){sleep(300);};
  
}

