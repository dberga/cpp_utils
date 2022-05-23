
//

#include "NXT++.h"
#include <iostream>
#include <signal.h>

using namespace std;

void signal_callback_handler(int signum)
{
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
	cout << "EXITING NXT++ avoidObstacles project" << endl;
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

void setBothBack(int seconds){
	NXT::Motor::SetReverse(OUT_B, 50);
	NXT::Motor::SetReverse(OUT_C, 50);
	sleep(seconds);
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

void setBothForward(){
	NXT::Motor::SetForward(OUT_B, 50);
	NXT::Motor::SetForward(OUT_C, 50);
}

void setBothForward(int seconds){
	NXT::Motor::SetForward(OUT_B, 50);
	NXT::Motor::SetForward(OUT_C, 50);
	sleep(seconds);
	NXT::Motor::Stop(OUT_B, false);
	NXT::Motor::Stop(OUT_C, false);
}

int main()
{
	cout << "NXT++ avoidObstacles project" << endl;
	if(NXT::Open()) //initialize the NXT and continue if it succeds
	{
		signal(SIGINT, signal_callback_handler);
		NXT::Sensor::SetTouch(IN_1);
		NXT::Sensor::SetTouch(IN_2);
		NXT::Sensor::SetSonar(IN_3);
		bool rightTouched = false;
		bool leftTouched = false;
		bool obstacleFound = false;
		int distance = 0;
		int rangeSonar = 27;

		while(1) {
			while(rightTouched == false && leftTouched == false && obstacleFound == false){
				setBothForward(1);
				rightTouched = NXT::Sensor::GetValue(IN_1);
				leftTouched = NXT::Sensor::GetValue(IN_2);
				distance = NXT::Sensor::GetSonarValue(IN_3);
				if(distance < rangeSonar) obstacleFound = true;
				
			}
			if(rightTouched == true && leftTouched == true){ //tocado dos botones
				cout <<"tocados dos botones -> Dando vuelta"<< endl;
				setBothBack(1);
				setTurnAround();
			}else if(rightTouched == false && leftTouched == true){ //tocado boton izquierdo
				cout <<"tocado boton izquierdo -> Girando a izquierda"<< endl;
				setBothBack(1);
				setLeftSpin();
			}else if(rightTouched == true && leftTouched == false){ //tocado boton derecho
				cout <<"tocado boton derecho -> Girando a derecha"<< endl;
				setBothBack(1);
				setRightSpin();
			}else { //no tocado pero obstaculo encontrado
				cout <<"Obstaculo encontrado -> Dando vuelta"<< endl;
				setBothBack(1);
				setTurnAround();
			}
		rightTouched = false;
		leftTouched = false;
		obstacleFound = false;
		usleep(500000);
		}
	}
	else
	cout << "NXT++ No connection" << endl;
	return 0;
}


