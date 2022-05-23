#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){

	camWidth 		= 320;	// try to grab at this size.
	camHeight 		= 240;

    vidGrabber.setVerbose(true);

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth,camHeight);

	videoInverted 	= new unsigned char[camWidth*camHeight*3];
	videoTexture.allocate(camWidth,camHeight, OF_IMAGE_COLOR);
	ofSetVerticalSync(true);

	color_image_.allocate(camWidth, camHeight);

	grayscale_image_.allocate(camWidth, camHeight);
    grayscale_background_.allocate(camWidth, camHeight);
	grayscale_diff_.allocate(camWidth, camHeight);

	capture_background_ = true;

	diff_threshold_ = 30;
	myfont1.loadFont("arial.ttf", 16);
	myfont2.loadFont("arial.ttf", 12);
}

//--------------------------------------------------------------
void testApp::update(){

	ofBackground(100,100,100);

	vidGrabber.update();

	if (vidGrabber.isFrameNew()){
		unsigned char * pixels = vidGrabber.getPixels();
		for (int i = 0; i < camWidth*camHeight; i++){
			videoInverted[i*3] = 255 - pixels[i*3];
			videoInverted[i*3+1] = 255 - pixels[i*3+1];
			videoInverted[i*3+2] = 255 - pixels[i*3+2];
		}
		videoTexture.setFromPixels(videoInverted, camWidth,camHeight, OF_IMAGE_COLOR);

		color_image_.setFromPixels(vidGrabber.getPixels(), camWidth, camHeight);

        grayscale_image_ = color_image_;
        //grayscale_image_.blurHeavily();

        if(capture_background_){
            grayscale_background_ = grayscale_image_;
            capture_background_ = false;
        }

        grayscale_diff_.absDiff(grayscale_background_, grayscale_image_);
        grayscale_diff_.threshold(diff_threshold_);

        contour_finder_.findContours(grayscale_diff_, 20, (camWidth*camHeight)/4, 10, true);
	}

}

//--------------------------------------------------------------
void testApp::draw(){

	ofSetHexColor(0xffffff);
	vidGrabber.draw(20, 20);

	videoTexture.draw(20+camWidth+20, 20, camWidth, camHeight);

	color_image_.draw(20+(camWidth+20)*2, 20, camWidth, camHeight);

	grayscale_background_.draw(20, 20+camHeight+20, camWidth, camHeight);
	grayscale_image_.draw(20+camWidth+20, 20+camHeight+20, camWidth, camHeight);
	grayscale_diff_.draw(20, 20+(camHeight+20)*2, camWidth, camHeight);

	for(int i = 0; i < contour_finder_.nBlobs; i++){
        //contour_finder_.blobs[i].draw(20, 20+(camHeight+20)*2);
        ofSetColor(255,0,0);
        ofRect(contour_finder_.blobs[i].boundingRect.getPosition().x+20,contour_finder_.blobs[i].boundingRect.getPosition().y+20+(camHeight+20)*2,contour_finder_.blobs[i].boundingRect.getWidth(),contour_finder_.blobs[i].boundingRect.getHeight());
    }
    ofSetColor(0,0,0);
    myfont1.drawString("Press B to get blobs info", 20+(camWidth+20),20+(camHeight+20)*2);
    myfont2.drawString(blobs_text_,40+(camWidth+20),40+(camHeight+20)*2);

}


//--------------------------------------------------------------
void testApp::keyPressed  (int key){

    if(key == ' '){
        capture_background_ = true;


    }


    if(key == 'a')
        diff_threshold_++;
    if(key == 'z')
        diff_threshold_--;

    if(key == 'b'){

        blobs_text_ = "";
        for(int i = 0; i < contour_finder_.nBlobs; i++){
            stringstream ss;
            ss << " found blob "<< i<< "; centroid:(X,Y)=(" << contour_finder_.blobs[i].centroid.x << ","  <<contour_finder_.blobs[i].centroid.y << "); size:(width,height)=("<< contour_finder_.blobs[i].boundingRect.getWidth() <<  ","  <<contour_finder_.blobs[i].boundingRect.getHeight() <<")"<< endl;
            blobs_text_ += ss.str();
        }
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}
