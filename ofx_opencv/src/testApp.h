#pragma once

#include "ofMain.h"

#include "ofxOpenCv.h"

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		ofVideoGrabber 		vidGrabber;
		unsigned char * 	videoInverted;
		ofImage			    videoTexture;
		int 				camWidth;
		int 				camHeight;

		ofxCvColorImage     color_image_;

		ofxCvGrayscaleImage grayscale_image_;
		ofxCvGrayscaleImage grayscale_background_;
		ofxCvGrayscaleImage grayscale_diff_;

		bool                capture_background_;
		int                 diff_threshold_;

		ofxCvContourFinder  contour_finder_;
		ofTrueTypeFont myfont1;
		ofTrueTypeFont myfont2;
		string blobs_text_;
};
