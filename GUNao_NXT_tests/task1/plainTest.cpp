
//

#include "NXT++.h"
#include <iostream>
using namespace std;
int main()
{
	cout << "NXT++ test project" << endl;
	int turnCount =0;
	if(NXT::Open()) //initialize the NXT and continue if it succeds
	{
		int oldCount = NXT::Motor::GetRotationCount(OUT_B); // obtain rotation
		NXT::Sensor::SetTouch(IN_1); //tell the NXT that the touch sensor is in port 1
		while(1) //main loop
		{
			if(NXT::Sensor::GetValue(IN_1) == true) //if the touch sensor is pressed down...
				NXT::Motor::SetForward(OUT_B, 50); //turn the motor in port 1 on 50% power
			else
				NXT::Motor::Stop(OUT_B, false); //if the touch sensor is not pressed down turn the motor off
			int newCount = NXT::Motor::GetRotationCount(OUT_B); // obtain rotation
			if (newCount > oldCount)
			{ 
				turnCount+= newCount-oldCount;
				oldCount=newCount;
				cout << "Number of turns so far is .... " << turnCount << endl;
			}
		usleep(500000);
		}
	}
	else
	cout << "NXT++ No connection" << endl;
	return 0;
}

