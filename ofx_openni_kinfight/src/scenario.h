#pragma once


#include "ofMain.h"
#include "ofxXmlSettings.h"


#define SCREEN_W 800
#define SCREEN_H 600

#define SUPRASCREEN_W 2000
#define SUPRASCREEN_H 1500


class Scenario{
	public:
		// Declaration of the basic methods for the scenario class.
		Scenario();
		~Scenario();

		void setup();
		void update();
		void draw();
		void drawsub(float posx, float posy);

	private:

        ofxXmlSettings XML;
        string message;

        ofImage background_;
        ofImage frustum_;

        ofSoundPlayer song;
};
