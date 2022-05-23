
//

#include "NXT++.h"
#include <iostream>
#include <signal.h>
#include <pthread.h>

using namespace std;

namespace myvolatiles
{
	float distance;
}
using namespace myvolatiles;

void signal_callback_handler(int signum)
{
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
	cout << "EXITING NXT++ distancethreadcontrol project" << endl;
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

void *maintainDistance(void * arg)
{
	signal(SIGINT, signal_callback_handler);
	float difference = 0;
	float scaleFactor = 10;
	myvolatiles::distance = 27; //preterminada inicial
	while(1){
		difference = myvolatiles::distance - NXT::Sensor::GetSonarValue(IN_3);
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
	}
	return 0;
}

void *touchingGetDistance(void * arg)
{
	signal(SIGINT, signal_callback_handler);
	bool rightTouched = false; 
	bool leftTouched = false; 
	while(1){
		rightTouched = NXT::Sensor::GetValue(IN_1);
		leftTouched = NXT::Sensor::GetValue(IN_2);
		if(rightTouched == true || leftTouched == true){
			cout << "Boton apretado -> capturando nuevo obstaculo" << endl;
			myvolatiles::distance = NXT::Sensor::GetSonarValue(IN_3);
		}
		usleep(250000);
	}
	return 0;
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
		myvolatiles::distance = NXT::Sensor::GetSonarValue(IN_3);
		pthread_t thread1, thread2;
   		pthread_create( &thread1, NULL, &maintainDistance, NULL);
   		pthread_create( &thread2, NULL, &touchingGetDistance, NULL);
		
		sleep(120); //ejecutar el programa main por 120 sec
		return 0;
	}
	else
	cout << "NXT++ No connection" << endl;
	return 0;
}


