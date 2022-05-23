DAVID BERGA GARRETA
NIA: 146716

LABORATORY 4 - ROBOTICS - SOFTWARE README

////////////////
Background:
////////////////
You should have some experience with the following.
1. POSIX tools and UNIX shell (specially Ubuntu).
2. Programming of signal handling in C/C++.
3. Setting up a module under the whiteboard software architecture.
4. C++ programming. You may need background in threads and tools that avoid busy waiting
of the CPU.

////////////////
Tasks:
////////////////

TASK1 -> FSM Avoid obstacles with touch sensor: Move the robot and use touch sensors to detect obstacles. Back of from obstacles and spin from away from them. You should attempt the other behaviors of Laboratory 1

TASK2 -> FSM Keeps at a distance: Keep the robot at a given distance (using sonar). The distance is supplied as input and the robot should try to keep up with the object, moving closer when it is too far, moving away when it is too close.

TASK3 -> FSM keeps at a distance with switch Switch on and switch off the behavior of tracking a distance
using touch sensors.


////////////////
File implementations:
////////////////

//File1: GUNao/nxt/gunxtbridge/gunxtinterface.cpp

-I've modified the bridge, just added a reportDistanceSonar that does the following: If the sensor takes a distance of 30, the it adds a message of "GoodDistance+SensorID". Else, if the senson takes a distance bigger than 30, it adds a message of "BigDistance+SensorID". Else, it adds a message of "LowDistance+SensorID".

//File2: GUNao/nxt/gunxtbridge/gunxtinterfacemodule.cpp

-Just added the reportDistanceSonar() on the loop

//File3: added-> GUNao/posix/tutorials/gubehaviorinterpreter

-I've added the files for this practice.

//File4: GUNao/nxt/gubehaviorinterpreter/examples/AAvoidObstaclesOnTouch.txt

-Tries to simulate states of the task1

//File5: GUNao/nxt/gubehaviorinterpreter/examples/TAvoidObstaclesOnTouch.txt

-Tries to simulate the changes of state of the task1

//File6: GUNao/nxt/gubehaviorinterpreter/examples/AKeepDistance.txt

-Tries to simulate states of the task2

//File7: GUNao/nxt/gubehaviorinterpreter/examples/TKeepDistance.txt

-Tries to simulate the changes of state of the task2

//File8: GUNao/nxt/gubehaviorinterpreter/examples/AKeepDistanceBehavior.txt

-Tries to simulate states of the task3

//File9: GUNao/nxt/gubehaviorinterpreter/examples/TKeepDistanceBehavior.txt

-Tries to simulate the changes of state of the task3

////////////////
IMPORTANT: The projects can be build but I didn't post the build files because in some file retrievals in moodle get errors on retrieving executables. Another thing is that compiling some files it says the error of #include<NXT++.h> well I can't put entire NXT files because it doesn't fit on moodle.

-To compile the files, just need to compile the posix/guWhiteboardMonitor(to see the messages), the posix/gutexttowbd (in case we want to write), the posix/gutexttowbd , the nxt/gunxtbridge, and posix/gubehaviorinterpreter with the parameters of the desired task in the examples
