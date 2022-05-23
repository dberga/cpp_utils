DAVID BERGA GARRETA
NIA: 146716

PRACTICE 2 - ROBOTICS

////////////////
Objectives:
////////////////
1. Develop the foundations of a software architecture and implement it in C++ to the Lego NXT brick.
2. Develop a simple control middle-ware for the sensors of the robot as a bridge between the NXT++ and a whiteboard architecture.
3. Develop a reactive controller for a behavior that has different actions for different situations discovered trough light sensors, sonar sensors and touch sensors.
4. Gain an understanding on the issues of platform independence, simulation, validation, the reactive-control architecture, minimal knowledge representation, and simple sensors versus complex sensors.

////////////////
File implementations:
////////////////

//File1: GUNao/nxt/gunxtbridge/gunxtinterface.cpp

Here we have implemented the callback functions and the rest of the constructor. Here I've made little modifications:
-Modification1: I've added a mutex which locks and unlocks in every callback
-Modification2: I've changed the output adding message of reportActiveTouchSensors from int "newCount" value to the format string "SENSOR,newCount" to get which of the sensors left/right are we getting the information to make the users easier to not to get the starting sensor values in their programs

//File2: GUNao/nxt/gunxtbridge/gunxtinterfacemodule.cpp

Here we have implemented the main which executes the interface. All it does is to report the values the robot is getting.
We have done a modification, we have included an "#if connected" which includes or excludes the code of if the robot is connected (if we want to see the values that the robot retrieves) or it is not connected (if we post the values via messages into the whiteboard)
The way to connect the robot is to put the "#define connected 1". As default it is at 0.

//File3: entire folder-> GUNao/posix/tutorials/reactiveController

Here we have implemented all the project of the robot which uses the arquitecture based on subscribing/adding messages by callbacks in the whiteboard. As well as the reactive control philosophy and a  feedback control shown in the callback of callBackSurface (which uses the light sensor) which remembers the previous surface position following each step calculating its mean. The code is already explained by comments.



IMPORTANT: The projects can be build but I didn't post the build files because in some file retrievals in moodle get errors on retrieving executables. Another thing is that compiling some files it says the error of #include<NXT++.h> well I can't put entire NXT files because it doesn't fit on moodle.

-To compile the files, just need to compile the posix/guWhiteboardMonitor(to see the messages), the posix/gutexttowbd (in case we want to write), the nxt/gunxtbridge, and posix/tutorials/reactiveController

////////////////
Conclusions:
////////////////

The conclusions are followed by the following qüestion:
-Why to use the "modules" arquitecture?
1.Because it makes easier to the programmers to implement their programs without having knowledge of using NXT robots, as well as making easier to include its libraries.
2.Because it makes easier to include more modules without rearranging any code.
3.It breaks up the typical code implementation just using this module architecture.

