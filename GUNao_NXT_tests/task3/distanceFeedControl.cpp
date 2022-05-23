
//

#include "NXT++.h"
#include <iostream>
#include <signal.h>

using namespace std;

void signal_callback_handler(int signum)
{
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
	cout << "EXITING NXT++ distancefeedcontrol project" << endl;
	usleep(1000000);
	exit(signum);
}

void setRightSpin(){
	NXT::Motor::ResetRotationCount(OUT_B,false);
	NXT::Motor::ResetRotationCount(OUT_C,false);
	float rightforwardcount = 0;
	float leftbackwardcount = 0;
	NXT::Motor::SetForward(OUT_B, 50);
	NXT::Motor::SetReverse(OUT_C, 50);
	float fortyfivedegrees = 0.125*0.5; //0.125 = 45/360
	while(leftbackwardcount + rightforwardcount < fortyfivedegrees )
	{ 
		leftbackwardcount += NXT::Motor::GetRotationCount(OUT_B);
		rightforwardcount += abs(NXT::Motor::GetRotationCount(OUT_C));
		usleep(250000);
	}
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

void setTurnRight(){
	int i;
	for(i=0; i < 2; i++){
	setRightSpin();
	}
}

void setLeftSpin(){
	NXT::Motor::ResetRotationCount(OUT_B,false);
	NXT::Motor::ResetRotationCount(OUT_C,false);
	float rightforwardcount = 0;
	float leftbackwardcount = 0;
	NXT::Motor::SetForward(OUT_C, 50);
	NXT::Motor::SetReverse(OUT_B, 50);
	float fortyfivedegrees = 0.125*0.5; //0.125 = 45/360
	while(leftbackwardcount + rightforwardcount < fortyfivedegrees )
	{ 	
		leftbackwardcount += abs(NXT::Motor::GetRotationCount(OUT_B));
		rightforwardcount += NXT::Motor::GetRotationCount(OUT_C);
		usleep(250000);
	}
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

void setTurnLeft(){
	int i;
	for(i=0; i < 2; i++){
	setLeftSpin();
	}
}

void setTurnAround(){
	int i;
	for(i=0; i < 4; i++){
	setLeftSpin();
	}
}

void setBothBack(){
	NXT::Motor::SetReverse(OUT_B, 50);
	NXT::Motor::SetReverse(OUT_C, 50);
}

void setBothBacktime(int seconds){
	NXT::Motor::SetReverse(OUT_B, 50);
	NXT::Motor::SetReverse(OUT_C, 50);
	sleep(seconds);
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

void setBothBack(int power){
	NXT::Motor::SetReverse(OUT_B, power);
	NXT::Motor::SetReverse(OUT_C, power);
}

void setBothForward(){
	NXT::Motor::SetForward(OUT_B, 50);
	NXT::Motor::SetForward(OUT_C, 50);
}

void setBothForward(int power){
	NXT::Motor::SetForward(OUT_B, power);
	NXT::Motor::SetForward(OUT_C, power);
}

void setBothForwardtime(int seconds){
	NXT::Motor::SetForward(OUT_B, 50);
	NXT::Motor::SetForward(OUT_C, 50);
	sleep(seconds);
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

void stopBoth(){
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}


int main()
{
	cout << "NXT++ distancefeedcontrol project" << endl;
	if(NXT::Open()) //initialize the NXT and continue if it succeds
	{
		signal(SIGINT, signal_callback_handler);
		NXT::Sensor::SetTouch(IN_1);
		NXT::Sensor::SetTouch(IN_2);
		NXT::Sensor::SetSonar(IN_3);
		bool rightTouched = false;
		bool leftTouched = false;
		bool obstacleFound = false;
		int distance = 27; //preterminada inicial
		int rangeSonar = 0;
		int difference = 0;
		int scaleFactor = 10;
		while(1) {
			if(rightTouched == false && leftTouched == false){
				difference = distance - NXT::Sensor::GetSonarValue(IN_3);
				difference = abs(difference);
				if (difference > 0){
					cout << "Tirando atras" << endl;
				 	setBothBack(difference*scaleFactor);
				}else if (difference < 0){
					cout << "Tirando adelante" << endl;
				 	setBothForward(difference*scaleFactor);
				}else{
					cout << "No mover" << endl;
					stopBoth();
				}
				sleep(1);
				stopBoth();
			}else{
				cout << "Boton apretado -> capturando nuevo obstaculo" << endl;
				distance = NXT::Sensor::GetSonarValue(IN_3);
			}
			usleep(250000);
			rightTouched = NXT::Sensor::GetValue(IN_1);
			leftTouched = NXT::Sensor::GetValue(IN_2);
		}
	}
	else
	cout << "NXT++ No connection" << endl;
	return 0;
}


