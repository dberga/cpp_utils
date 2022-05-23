#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup() {

    ofSetFrameRate(60);

    box2d.init();
    box2d.setGravity(0,9.8);
    box2d.setFPS(30);
    box2d.createBounds();
    box2d.registerGrabbing();

    left_foot.setPhysics(5.0,0.5,0.1);
    left_foot.setup(box2d.getWorld(),ofGetWindowWidth()/2,ofGetWindowHeight()/2, 50);

    right_foot.setPhysics(5.0,0.5,0.1);
    right_foot.setup(box2d.getWorld(),ofGetWindowWidth()/2,ofGetWindowHeight()/2, 50);


    ofSetLogLevel(OF_LOG_VERBOSE);

    ofEnableAlphaBlending();

    openNIDevice.setup();
    openNIDevice.addImageGenerator(); // we set Kinect to grab color image
    openNIDevice.addDepthGenerator(); // we set Kinect to grab depth image
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator(); // we set Kinect to track users
    openNIDevice.setMaxNumUsers(1); // to a maximum of just ONE user
    openNIDevice.start(); // we launch the Kinect tracking

    // set properties for all user masks and point clouds
    //openNIDevice.setUseMaskPixelsAllUsers(true); // if you just want pixels, use this set to true
    openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(2); // resolution of the mesh created for the point cloud eg., this will use every second depth pixel

    limb_image_[0].loadImage("head.png");
    image_size_[0].x = 120;
    image_size_[0].y = 120;
    limb_image_[1].loadImage("hand_r.jpg");
    image_size_[1].x = 120;
    image_size_[1].y = 120;
    limb_image_[2].loadImage("hand_l.jpg");
    image_size_[2].x = 120;
    image_size_[2].y = 120;
    limb_image_[3].loadImage("foot_r.jpg");
    image_size_[3].x = 120;
    image_size_[3].y = 120;
    limb_image_[4].loadImage("foot_l.jpg");
    image_size_[4].x = 120;
    image_size_[4].y = 120;

        verdana.loadFont(ofToDataPath("verdana.ttf"), 24);


}

//--------------------------------------------------------------
void testApp::update() {

    box2d.update();


    openNIDevice.update();

    int numUsers = openNIDevice.getNumTrackedUsers();

    // iterate through users (actually just one)
    for (int i = 0; i < numUsers; i++){

        // get a reference to this user
        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

        // Calculate position and orentation for head image
        image_position_[0] = user.getJoint(JOINT_HEAD).getProjectivePosition();
        ofxOpenNILimb user_neck_limb = user.getLimb(LIMB_NECK);
        ofxOpenNIJoint user_neck_start = user_neck_limb.getStartJoint();
        ofxOpenNIJoint user_neck_end = user_neck_limb.getEndJoint();
        image_rotation_[0] = atan2(user_neck_end.getProjectivePosition().x - user_neck_start.getProjectivePosition().x,
                                   user_neck_end.getProjectivePosition().y - user_neck_start.getProjectivePosition().y);
        image_rotation_[0] = image_rotation_[0] * 180/PI * -1;

        image_position_[1] = user.getJoint(JOINT_LEFT_HAND).getProjectivePosition();
        ofxOpenNILimb user_left_hand_limb = user.getLimb(LIMB_LEFT_LOWER_ARM);
        ofxOpenNIJoint user_left_hand_start = user_left_hand_limb.getStartJoint();
        ofxOpenNIJoint user_left_hand_end = user_left_hand_limb.getEndJoint();
        image_rotation_[1] = atan2(user_left_hand_end.getProjectivePosition().x - user_left_hand_start.getProjectivePosition().x,
                                   user_left_hand_end.getProjectivePosition().y - user_left_hand_start.getProjectivePosition().y);
        image_rotation_[1] = image_rotation_[1] * 180/PI * -1;


        image_position_[2] = user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition();
        ofxOpenNILimb user_right_hand_limb = user.getLimb(LIMB_RIGHT_LOWER_ARM);
        ofxOpenNIJoint user_right_hand_start = user_right_hand_limb.getStartJoint();
        ofxOpenNIJoint user_right_hand_end = user_right_hand_limb.getEndJoint();
        image_rotation_[2] = atan2(user_right_hand_end.getProjectivePosition().x - user_right_hand_start.getProjectivePosition().x,
                                   user_right_hand_end.getProjectivePosition().y - user_right_hand_start.getProjectivePosition().y);
        image_rotation_[2] = image_rotation_[2] * 180/PI * -1;

        image_position_[3] = user.getJoint(JOINT_LEFT_FOOT).getProjectivePosition();
        ofxOpenNILimb user_left_foot_limb = user.getLimb(LIMB_LEFT_LOWER_LEG);
        ofxOpenNIJoint user_left_foot_start = user_left_foot_limb.getStartJoint();
        ofxOpenNIJoint user_left_foot_end = user_left_foot_limb.getEndJoint();
        image_rotation_[3] = atan2(user_left_foot_end.getProjectivePosition().x - user_left_foot_start.getProjectivePosition().x,
                              user_left_foot_end.getProjectivePosition().y - user_left_foot_start.getProjectivePosition().y);
        image_rotation_[3] = image_rotation_[3] * 180/PI * -1;

        image_position_[4] = user.getJoint(JOINT_RIGHT_FOOT).getProjectivePosition();
        ofxOpenNILimb user_right_foot_limb = user.getLimb(LIMB_RIGHT_LOWER_LEG);
        ofxOpenNIJoint user_right_foot_start = user_right_foot_limb.getStartJoint();
        ofxOpenNIJoint user_right_foot_end = user_right_foot_limb.getEndJoint();
        image_rotation_[4] = atan2(user_right_foot_end.getProjectivePosition().x - user_right_foot_start.getProjectivePosition().x,
                                   user_right_foot_end.getProjectivePosition().y - user_right_foot_start.getProjectivePosition().y);
        image_rotation_[4] = image_rotation_[4] * 180/PI * -1;

		
		//agafem les posicions box2d al joint dels peus -> ultim user trackejat
        left_foot.setPosition(user_left_foot_start.getProjectivePosition());
        right_foot.setPosition(user_right_foot_start.getProjectivePosition());

		//agafem la posicio de les mans, en cas que pugin per sobre del cap, agreguem box2d al vector
		if(user_left_hand_end.getProjectivePosition().y <= user_neck_end.getProjectivePosition().y){
			float random_h1 = ofRandom(5,25);
			ofxBox2dCircle temp_circle1;
			temp_circle1.setPhysics(5.0,0.5,0.1);
			temp_circle1.setup(box2d.getWorld(),ofGetMouseX(),ofGetMouseY(), random_h1);
			circles.push_back(temp_circle1);
		}
		if(user_right_hand_end.getProjectivePosition().y <= user_neck_end.getProjectivePosition().y){
			float random_h1 = ofRandom(5,25);
			ofxBox2dCircle temp_circle1;
			temp_circle1.setPhysics(5.0,0.5,0.1);
			temp_circle1.setup(box2d.getWorld(),ofGetMouseX(),ofGetMouseY(), random_h1);
			circles.push_back(temp_circle1);
		}

    }

}

//--------------------------------------------------------------
void testApp::draw() {

    ofSetHexColor(0xFF8822);

    for(int i = 0; i < circles.size(); i++){
        circles[i].draw();
    }

    ofDrawBitmapString(ofToString((box2d.getBodyCount())),20,20);

    ofSetColor(255, 255, 255);
    ofPushMatrix();
    //openNIDevice.drawDebug();

    ofPopMatrix();

    ofPushMatrix();
    // use a blend mode so we can see 'through' the mask(s)
    ofEnableBlendMode(OF_BLENDMODE_ALPHA);
    ofSetRectMode(OF_RECTMODE_CENTER);

    // get number of current users
    int numUsers = openNIDevice.getNumTrackedUsers();

    // iterate through users
    for (int i = 0; i < numUsers; i++){

        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

        //user.drawMask();
        openNIDevice.drawSkeleton(i);
        ofPushMatrix();
            ofSetColor(255, 255, 255);

            ofTranslate(image_position_[0].x, image_position_[0].y);
            ofRotate(image_rotation_[0], 0, 0, 1);

            limb_image_[0].draw(0, 0, image_size_[0].x, image_size_[0].y);
        ofPopMatrix();

        ofPushMatrix();
            ofSetColor(255, 255, 255);

            ofTranslate(image_position_[1].x, image_position_[1].y);
            ofRotate(image_rotation_[1], 0, 0, 1);

            limb_image_[1].draw(0, 0, image_size_[1].x, image_size_[1].y);
        ofPopMatrix();

        ofPushMatrix();
            ofSetColor(255, 255, 255);

            ofTranslate(image_position_[2].x, image_position_[2].y);
            ofRotate(image_rotation_[2], 0, 0, 1);

            limb_image_[2].draw(0, 0, image_size_[2].x, image_size_[2].y);
        ofPopMatrix();
		
		//desactivem els peus de luigi
		/*
        ofPushMatrix();
            ofSetColor(255, 255, 255);

            ofTranslate(image_position_[3].x, image_position_[3].y);
            ofRotate(image_rotation_[3], 0, 0, 1);

            //limb_image_[3].draw(0, 0, image_size_[3].x, image_size_[3].y);
            
            //ofSetHexColor(0xFFFFFF);
        ofPopMatrix();

        ofPushMatrix();
            ofSetColor(255, 255, 255);

            ofTranslate(image_position_[4].x, image_position_[4].y);
            ofRotate(image_rotation_[4], 0, 0, 1);

            //limb_image_[4].draw(0, 0, image_size_[4].x, image_size_[4].y);
        
        //ofSetHexColor(0xFFFFFF);
            ofPopMatrix();
		*/
		
		//dibuixem els peus com box2d blancs
		left_foot.draw();
		right_foot.draw();

    }

    ofSetRectMode(OF_RECTMODE_CORNER);
    ofDisableBlendMode();
    ofPopMatrix();

    // draw some info regarding frame counts etc
	ofSetColor(0, 255, 0);
	string msg = " MILLIS: " + ofToString(ofGetElapsedTimeMillis()) + " FPS: " + ofToString(ofGetFrameRate()) + " Device FPS: " + ofToString(openNIDevice.getFrameRate());

	verdana.drawString(msg, 20, openNIDevice.getNumDevices() * 480 - 20);

}

//--------------------------------------------------------------
void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

//--------------------------------------------------------------
void testApp::exit(){
    openNIDevice.stop();
}

//--------------------------------------------------------------
void testApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void testApp::keyReleased(int key) {
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ) {

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button) {
}

//--------------------------------------------------------------
void testApp::resized(int w, int h){
}

