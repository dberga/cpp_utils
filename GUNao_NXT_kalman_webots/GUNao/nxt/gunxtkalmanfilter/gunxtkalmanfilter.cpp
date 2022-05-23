
#include "gu_util.h"  
#include <sstream>
#include <iostream>

#include "kalman.h"	

#include "Whiteboard.h" 
#include "gumessagecontent.h"  

#include "gunxtbridge_wbmsg.h"   
#include "gunxtkalmanfilter_wbmsg.h"   
#include "gunxtconstants.h"
#include "pthread.h"

#include "gunxtkalmanfilter.h"   // the .h-file for this class

using namespace std;
using namespace guWhiteboard;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int previoussonarvalue = 999; //token value
int previouslightvalue = 999; //token value


GUNxtKalmanFilter ::GUNxtKalmanFilter()
	{
	    wb = new Whiteboard();

	    Whiteboard::WBResult wr;//WBResult

	    for (theSensors i=SENSOR_ONE; i<=SENSOR_FOUR; i++)
	     {
		char theID = NXT_SENSOR_NAMES::getName(i) ;
		string sensorID(1, theID );
	
                DBG(cerr << " ** Subscriptions for Sensor ID == " << sensorID << endl;)
		// LIGHT message responses
	    	wb->subscribeToMessage("gunxt_valueLightNoisySensor"+sensorID, WB_BIND(GUNxtKalmanFilter::callBackKFilterLightSensor) , wr);

		// SONAR message responses
	    	wb->subscribeToMessage("gunxt_valueSonarNoisySensor"+sensorID, WB_BIND(GUNxtKalmanFilter::callBackKFilterSonarSensor) , wr);
		}

	}

void GUNxtKalmanFilter ::banner()
	{   cout << " GU NXT Whiteboard Kalman Filter --- module to replace noisy sensor values with filtered ones  " << endl;
	}

void GUNxtKalmanFilter::callBackKFilterLightSensor(std::string dataName, WBMsg *msg)
{
   pthread_mutex_lock( &mutex1 );
	DBG(cerr << " ** Repalce LIGHT noisy sensor value with filtered value **" << endl;)

	string sensorID(1, findSensorID(dataName) );

        MessageContent theMessageContent (msg);

        int content = theMessageContent.theContentToInt();

        DBG (cout << "Message Received  ++++++>" << content << endl );

		if(previouslightvalue == 999){
			previouslightvalue = content;
			DBG (cout << "First Message received value ++++++>" << content << endl );
		}else{
			previouslightvalue = KALMAN_FILTER:: kfilter(content,previouslightvalue);
			DBG (cout << "Message received value ++++++>" << content << endl );
			DBG (cout << "Filtered Value         ++++++>" << previouslightvalue << endl );
			wb->addMessage(kNxtValueKFilterLightSensor+sensorID ,WBMsg(previouslightvalue),true);
		}
		pthread_mutex_unlock( &mutex1 );
}

void GUNxtKalmanFilter::callBackKFilterSonarSensor(std::string dataName, WBMsg *msg)
{
    pthread_mutex_lock( &mutex1 );
	DBG(cerr << " ** Repalce SONAR noisy sensor value with filtered value **" << endl;)

	string sensorID(1, findSensorID(dataName) );

        MessageContent theMessageContent (msg);

        int content = theMessageContent.theContentToInt();

        DBG (cout << "Message Received  ++++++>" << content << endl );

		if(previoussonarvalue == 999){
			previoussonarvalue = content;
			DBG (cout << "First Message received value ++++++>" << content << endl );
		}else{
			previoussonarvalue = KALMAN_FILTER:: kfilter(content,previoussonarvalue);
			DBG (cout << "Message received value ++++++>" << content << endl );
			DBG (cout << "Filtered Value         ++++++>" << previoussonarvalue << endl );
			wb->addMessage(kNxtValueKFilterSonarSensor+sensorID ,WBMsg(previoussonarvalue),true);
		}
		pthread_mutex_unlock( &mutex1 );
}

char  GUNxtKalmanFilter::findSensorID(std::string dataName )
{

	DBG(cerr << " ** dataName == " << dataName << endl;)
	char theID=SENSOR_1;
	
	for (theSensors i=SENSOR_ONE; i<=SENSOR_FOUR; i++)
	     {
		theID = NXT_SENSOR_NAMES::getName(i) ;
		//* if foudn break the for*/
	        if (string::npos!=  dataName.find(theID)) break;
	     }

	return theID;

}

