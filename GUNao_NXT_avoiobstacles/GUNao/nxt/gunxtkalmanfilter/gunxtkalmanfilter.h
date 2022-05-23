

#ifndef GUNXTKALMANFILTER
#define GUNXTKALMANFILTER
#include <iostream>
#include "Whiteboard.h"
#include "WBMsg.h"

#include "gunxtbridge_wbmsg.h" 


class GUNxtKalmanFilter {
  public:

    GUNxtKalmanFilter();                         
	void banner();
	virtual void callBackKFilterLightSensor(std::string dataName, WBMsg *msg);
	virtual void callBackKFilterSonarSensor(std::string dataName, WBMsg *msg);
	
  private:
	guWhiteboard::Whiteboard *wb; 
	char findSensorID(std::string dataName);

};



#endif // GUNXTNOISYSENSOR
