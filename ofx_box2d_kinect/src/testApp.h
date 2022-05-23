#pragma once
#include "ofMain.h"

// new addons
#include "ofxBox2d.h"
#include "ofxOpenNI.h"

// -------------------------------------------------

class testApp : public ofBaseApp {

public:

	void setup();
	void update();
	void draw();
	void exit();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void resized(int w, int h);

    // static instance of openNI device for Kinect use
	void userEvent(ofxOpenNIUserEvent & event);

	ofxOpenNI openNIDevice;

    ofTrueTypeFont verdana;

    ofPoint image_position_[5];
    ofPoint image_size_[5];
    float   image_rotation_[5];
    ofImage limb_image_[5];

    // static instance of a Box2D world
	ofxBox2d    box2d;

	//peus
	ofxBox2dCircle    left_foot;
	ofxBox2dCircle    right_foot;
	
	//pilotes creades
	vector<ofxBox2dCircle>  circles;


};

