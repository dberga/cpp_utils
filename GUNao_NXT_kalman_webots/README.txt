DAVID BERGA GARRETA
NIA: 146716

LABORATORY 3 - ROBOTICS

////////////////
Objectives:
////////////////
1. Using the software architecture of the whiteboard to introduce white noise into the sonar and light sensor.
2. Develop a module that filters white normal noise.
3. Explore the demo version of Webots (www.cyberbotics.com).
4. Optional: Develop a Webots bridge to one of the differential robots already provided by Webots.

////////////////
File implementations:
////////////////

//File1: GUNao/nxt/gunxtbridge/gunxtinterface.cpp

-I've modified the bridge, just to give the sensorID in the reports of the values of the sonar,light and touch. 


//File2: GUNao/posix/tutorials/reactiveController

-I've corrected two facts. One is that the surface callback already had a pseudo filter so now I corrected it. The other fact is that I did a pseudo code of getting the sensor from the message from the report of the touch sensor, now is not needed, I get it from the sensorID in the dataName.

-I've modified the subscriptions that now are able to get the sensorID. And I added the function findSensorID as well.


//File3: GUNao/nxt/gunxtnoisysensor/gunxtnoisysensormodule.cpp

-I've putted the correct deviation corresponding to the one after the usage of the filter.


//File4: added -> GUNao/nxt/gunxtkalmanfilter

-I've implemented the kalman filter, based on averages of every remembered value of the noisysensor and the new ones. It takes the values from the messages of the noisy sensor.


////////////////
IMPORTANT: The projects can be build but I didn't post the build files because in some file retrievals in moodle get errors on retrieving executables. Another thing is that compiling some files it says the error of #include<NXT++.h> well I can't put entire NXT files because it doesn't fit on moodle.

-To compile the files, just need to compile the posix/guWhiteboardMonitor(to see the messages), the posix/gutexttowbd (in case we want to write), the posix/gutexttowbd , the nxt/gunxtnoisysensor, the nxt/kalmanfilter, the nxt/gunxtbridge, and posix/tutorials/reactiveController

////////////////
Conclusions:
////////////////

ABOUT THE NOISY SENSOR

This conclusions are followed by the following qüestions:

-Why to use noisy sensor?
1.Because In most robotics application, sensors are noisy. What does this mean? Simply, the measurement produced by the sensors is not completely accurate. If that measurement is taken again, we may find that the sensor report a different value. If the environment conditions are shifting and the robot is moving, it is even more likely that the measurements reflect further oscillations. We say can model the noise as a Gaussian distribution G(mu, sigma), if we assume that the measurement we obtain V' is in fact the true value V plus some error E, where E is distributed as G(mu, sigma),
That is, 
->V' = V + E
If it is the case that mu = 0 we say that the noise is white-noise.

-Which is the best sigma to use?
2.I've noticed that the sigma to reduce the errors is better to put it less than 10.0. In fact, about 3.00 before using the kalman filter and 5.5 after using it.

ABOUT KALMAN FILTER

This conclusions are followed by the following qüestions:

-What is the kalman filter?
1.A very popular method to utilize the values of a sensor when we have strong reason to believe the noise can be modeled by a Gaussian is the Kalman filter. Is is also used for tracking a distance sensor of robots of objects that want to track position when the movement is linear.

-Why to use a kalman filter?
2.We use the kalman filter to reduce the error to make in time, the results to be similar as a Gaussian, as more mashed as possible. To do that we keep remembering every value and we average the latest value taken with the previous one so everytime we get a pseudo memory of the previous values, so if the new value is so hard, the error will be almost entirely prevented.


//////////////////////////////////////////////////////////////
WEBOTS

-I've made a world with some walls and a robot with a shape of a cilinder with a distance sensor
I plugged him a controller made by me, called robotlabyrinth.
The robotlabyrinth.c what does is to make the robot run, if he sees an obstacle, he turns on right. If not, he stills moving forward.
Ok so the robot can go in 4 ways (up, down, left, right) depending on his orientation axis. So, everytime he moves to a concrete way,
we remember that suming a value to the way. Then if the robot doesn't see an obstacle, when he's moving forward, he looks which way 
he has gone less, so he will spin to that way and keep walking. Maybe it's a kinky implementation but is a good idea specially to avoid
being in duplicated places. 

Before executing the robot, just put the robotlabyrinth folder inside "WEBOTS FOLDER/resources/projects/default/controllers"
Then, execute the Labyrinth.wbm. Then open the robot click on controller, edit and compile the file. Thats because I had a PROBLEM: Even having the 30 day license, the program didn't let me try to compile so if you execute it, the robot won't move. But it should after being compiled


