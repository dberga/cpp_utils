/**
 * \file  gunxtnoisegenerator.cpp
 *  Created by
 *  \author Vlad Estivill-Castro 
 *  \date 9/03/2013.
 */

/*
 * 
 * This is amodule so posting to the whitebaord will produce soudns in
 * a LEGO mindstorm NXY
 * 
 *  Copyright 2012 Vlad Estivill-Castro. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above
 *    copyright notice, this list of conditions and the following
 *    disclaimer in the documentation and/or other materials
 *    provided with the distribution.
 *
 * 3. All advertising materials mentioning features or use of this
 *    software must display the following acknowledgement:
 *
 *        This product includes software developed by Rene Hexel.
 *
 * 4. Neither the name of the author nor the names of contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * -----------------------------------------------------------------------
 * This program is free software; you can redistribute it and/or
 * modify it under the above terms or under the terms of the GNU
 * General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see http://www.gnu.org/licenses/
 * or write to the Free Software Foundation, Inc., 51 Franklin Street,
 * Fifth Floor, Boston, MA  02110-1301, USA.
 *
 */


#include "gu_util.h"  // for DBG = DEBUG
#include <sstream>
#include <iostream>

#include "boxmuller.h"	// to generte gaussian distributed data

#include "Whiteboard.h" 
#include "gumessagecontent.h"   // to extract content from messages

#include "gunxtbridge_wbmsg.h"   // the constants we subscribe from nxtbridge
#include "gunxtnoisysensor_wbmsg.h"   // the constants we subscribe from nxtbridge
#include "gunxtconstants.h"   // the constants for sensor IDs

#include "gunxtnoisegenerator.h"   // the .h-file for this class

using namespace std;
using namespace guWhiteboard;


GUNxtNoiseGenerator ::GUNxtNoiseGenerator(double stdDev)
	{
	    wb = new Whiteboard();

	    myStdDev=stdDev;

	    Whiteboard::WBResult wr;//WBResult

	    for (theSensors i=SENSOR_ONE; i<=SENSOR_FOUR; i++)
	     {
		char theID = NXT_SENSOR_NAMES::getName(i) ;
		string sensorID(1, theID );
	
                DBG(cerr << " ** Subscriptions for Sensor ID == " << sensorID << endl;)
		// LIGHT message responses
	    	wb->subscribeToMessage(kNxtValueLightSensor+sensorID, WB_BIND(GUNxtNoiseGenerator::callBackLightSensorNoisy) , wr);

		// SONAR message responses
	    	wb->subscribeToMessage(kNxtValueSonarSensor+sensorID, WB_BIND(GUNxtNoiseGenerator::callBackSonarSensorNoisy) , wr);
		}

	}

void GUNxtNoiseGenerator ::banner()
	{   cout << " GU NXT Whiteboard Sensor Noise --- module to replace sensor values with noisy ones  " << endl;
	    cout << " (c) 2012  Vlad Estivill-Castro" << endl;
	}

void GUNxtNoiseGenerator::callBackLightSensorNoisy(std::string dataName, WBMsg *msg)
{
   // TODO: this is a different trhead, may need semaphore protection
	DBG(cerr << " ** Repalce LIGHT sensor value with noisy **" << endl;)

	string sensorID(1, findSensorID(dataName) );

        MessageContent theMessageContent (msg);

        int content = theMessageContent.theContentToInt();

        DBG (cout << "Message Received  ++++++>" << content << endl );

	content= content + BOX_MULLER:: box_muller(0.0,myStdDev);

        DBG (cout << "Message Generated  ++++++>" << content << endl );

	wb->addMessage(kNxtValueLightNoisySensor+sensorID ,WBMsg(content),true);


}

void GUNxtNoiseGenerator::callBackSonarSensorNoisy(std::string dataName, WBMsg *msg)
{
   // TODO: this is a different trhead, may need semaphore protection
	DBG(cerr << " ** Repalce SONAR sensor value with noisy **" << endl;)

	string sensorID(1, findSensorID(dataName) );

        MessageContent theMessageContent (msg);

        int content = theMessageContent.theContentToInt();

        DBG (cout << "Message Received  ++++++>" << content << endl );

	content= content + BOX_MULLER:: box_muller(0.0,myStdDev);

        DBG (cout << "Message Generated  ++++++>" << content << endl );

	wb->addMessage(kNxtValueSonarNoisySensor+sensorID ,WBMsg(content),true);

}

char  GUNxtNoiseGenerator::findSensorID(std::string dataName )
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

