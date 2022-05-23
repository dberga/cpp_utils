/*
 * File:         mybot_simple.c
 * Date:         August 8th, 2006
 * Description:  A really simple controller which moves the MyBot robot
 *               and avoids the walls
 * Author:       Simon Blanchoud
 * Modifications:
 *
 * Copyright (c) 2008 Cyberbotics - www.cyberbotics.com
 */

#include <webots/robot.h>
#include <webots/differential_wheels.h>
#include <webots/distance_sensor.h>

#define SPEED 60
#define TIME_STEP 64

int main()
{
  wb_robot_init(); /* necessary to initialize webots stuff */
  
  /* Get and enable the distance sensors. */
  WbDeviceTag ir0 = wb_robot_get_device("ir0");
  wb_distance_sensor_enable(ir0, TIME_STEP);
  int way1=0;
  int way2=0;
  int way3=0;
  int way4=0;
  int axis1=1;
  int axis2=0;
  int axis3=0;
  int axis4=0;
  
  while(wb_robot_step(TIME_STEP)!=-1) {
    
    /* Get distance sensor values */
    double ir0_value = wb_distance_sensor_get_value(ir0);

    /* Compute the motor speeds */
    double left_speed, right_speed;
    if (ir0_value > 300) { //detects obstacle

        //turns left
        left_speed = 1;
        right_speed = (ir0_value / 10) + 5;
        
        //gets the present axis
        if(axis1==1){
        axis1=0;
        axis2=1;
        axis3=0;
        axis4=0;
        }
        
        if(axis2==1){
        axis1=0;
        axis2=0;
        axis3=1;
        axis4=0;
        }
         
        if(axis3==1){
        axis1=0;
        axis2=0;
        axis3=0;
        axis4=1;
        }
        
        if(axis4==1){
        axis1=1;
        axis2=0;
        axis3=0;
        axis4=0;
        }
        
        
    } else {
                  //we turn around to the way we moved less
                  if(way1 > way2){
                  axis1=0;
                  axis2=1;
                  axis3=0;
                  axis4=0;
                    //turns left
                    left_speed = 0;
                    right_speed = (ir0_value / 10) + 5;
                  }else if(way1 > way3){
                  axis1=0;
                  axis2=0;
                  axis3=1;
                  axis4=0;
                  }else{
                  axis1=0;
                  axis2=0;
                  axis3=0;
                  axis4=1;
                  }
                  
                  if(way2 > way3 && way2 < way1){
                  axis1=0;
                  axis2=0;
                  axis3=1;
                  axis4=0;
                    //turns left
                    left_speed = 0;
                    right_speed = (ir0_value / 10) + 5;
                  }else if(way2 > way4){
                  axis1=0;
                  axis2=0;
                  axis3=0;
                  axis4=1;
                  }else{
                  axis1=1;
                  axis2=0;
                  axis3=0;
                  axis4=0;
                  }
                  
                  if(way3 > way4 && way3 < way2){
                  axis1=0;
                  axis2=0;
                  axis3=0;
                  axis4=1;
                    //turns left
                    left_speed = 0;
                    right_speed = (ir0_value / 10) + 5;
                  }else if(way3 > way1){
                  axis1=1;
                  axis2=0;
                  axis3=0;
                  axis4=0;
                  }else{
                  axis1=0;
                  axis2=1;
                  axis3=0;
                  axis4=0;
                  }
                  
                  if(way4 > way1 && way4 < way3){
                  axis1=1;
                  axis2=0;
                  axis3=0;
                  axis4=0;
                    //turns left
                    left_speed = 0;
                    right_speed = (ir0_value / 10) + 5;
                  }else if(way4 > way2){
                  axis1=0;
                  axis2=1;
                  axis3=0;
                  axis4=0;
                  }else{
                  axis1=0;
                  axis2=0;
                  axis3=1;
                  axis4=0;
                  }
    
    
    
    
        //moves forward
        left_speed = SPEED;
        right_speed = SPEED;
        
        //the way it gone is sumed
        if(axis1==1) way1++;
        
        if(axis2==1) way2++;
         
        if(axis3==1) way3++;
        
        if(axis4==1) way4++;
        
        
    }

    

    /* Set the motor speeds. */
    wb_differential_wheels_set_speed(left_speed, right_speed);
  }
  
  return 0;
}

