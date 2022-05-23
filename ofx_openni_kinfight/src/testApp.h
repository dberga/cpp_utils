#pragma once
#include "ofxOpenNI.h"
#include "ofMain.h"
#include "ofxOpenNIUtils.h"

#include "player.h"
#include "opponent.h"
#include "scenario.h"
#include "menu.h"




class testApp : public ofBaseApp{
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		void drawFPS();
		void collisionPO();
		void collisionOP();
		void cameradepend(int px, int py);

	private:

		ofTrueTypeFont		font;
		float		readTime;

		Fighter			player_;
		Opponent		opponent_;
		Scenario	    scenario_;
        Menu            menu_;

        ofPoint camera_;

        int damagetoopponent;
        int damagetoplayer;

        int fatiguetoplayer;
        int fatiguetoopponent;

        int exptoplayer;

        ofPoint P_LG;
        ofPoint P_RG;
        ofPoint O_LG;
        ofPoint O_RG;
        ofPoint O_ALL;

        ofPoint P_FACE;
        ofPoint P_TORSO;
void exit();
    void userEvent(ofxOpenNIUserEvent & event);

	ofxOpenNI openNIDevice;

    ofTrueTypeFont verdana;





        ofxOpenNIJoint head_start;
       ofPoint head_pos;
        ofxOpenNIJoint left_hand_start;
       ofPoint left_hand_pos;
       float left_hand_width;
	   float left_hand_height;
       ofxOpenNIJoint right_hand_start;
       ofPoint right_hand_pos;
       float right_hand_width;
	   float right_hand_height;
};
