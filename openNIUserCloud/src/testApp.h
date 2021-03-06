#ifndef _TEST_APP
#define _TEST_APP

#include "ofxOpenNI.h"
#include "ofMain.h"

class testApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();
    void exit();

	void keyPressed  (int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);

    void userEvent(ofxOpenNIUserEvent & event);

	ofxOpenNI openNIDevice;

    ofTrueTypeFont verdana;

    ofPoint image_position_[5];
    ofPoint image_size_[5];
    float   image_rotation_[5];
    ofImage limb_image_[5];

};

#endif
