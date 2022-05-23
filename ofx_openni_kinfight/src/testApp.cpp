#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){



    ofSetVerticalSync(true);
	//ofBackground(255);
	ofSetLogLevel(OF_LOG_VERBOSE);

	openNIDevice.setup();
    openNIDevice.addImageGenerator();
    openNIDevice.addDepthGenerator();
    openNIDevice.setRegister(true);
    openNIDevice.setMirror(true);
    openNIDevice.addUserGenerator();
    openNIDevice.setMaxNumUsers(2);
    openNIDevice.start();
	openNIDevice.setUseMaskTextureAllUsers(true); // this turns on mask pixels internally AND creates mask textures efficiently
    openNIDevice.setUsePointCloudsAllUsers(true);
    openNIDevice.setPointCloudDrawSizeAllUsers(2); // size of each 'point' in the point cloud
    openNIDevice.setPointCloudResolutionAllUsers(2);


	ofEnableAlphaBlending();

	font.loadFont("novasolid.ttf", 34);
	//ofEnableAlphaBlending();
	ofSetFrameRate(60);

	readTime = 0;
    camera_.x=SUPRASCREEN_W*0.5;
    camera_.y=SUPRASCREEN_H*0.5;

    opponent_.setScreen(SCREEN_W,SCREEN_H);
	player_.setScreen(SCREEN_W,SCREEN_H);

	scenario_.setup();

	opponent_.setFirstSUPRAdelims(0,0); //CAMBIAR!
	opponent_.setLastSUPRAdelims(SUPRASCREEN_W,SUPRASCREEN_H);

    player_.setup();
    opponent_.setup();

}

//--------------------------------------------------------------
void testApp::update(){

   readTime = ofGetSeconds();

   openNIDevice.update();



    int numUsers = openNIDevice.getNumTrackedUsers();

     for (int i = 0; i < numUsers; i++){


        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

        head_start = user.getJoint(JOINT_HEAD);
               if(head_start.isFound())
               {
                    head_pos = head_start.getProjectivePosition();
               }

        left_hand_start = user.getJoint(JOINT_LEFT_HAND);
               if(left_hand_start.isFound()){
                left_hand_pos = left_hand_start.getProjectivePosition();
               //left_hand_width = ofDist(user.getJoint(JOINT_LEFT_HAND).getProjectivePosition().x,user.getJoint(JOINT_LEFT_HAND).getProjectivePosition().y,user.getJoint(JOINT_LEFT_ELBOW).getProjectivePosition().x,user.getJoint(JOINT_LEFT_ELBOW).getProjectivePosition().y)*0.5;
               //left_hand_height = left_hand_width;
               //left_hand_pos.x -= left_hand_width*0.5;
               //left_hand_pos.y -= left_hand_height*0.5;
        }
        right_hand_start = user.getJoint(JOINT_RIGHT_HAND);

         if(right_hand_start.isFound()){
               right_hand_pos = right_hand_start.getProjectivePosition();
               //	   right_hand_width = ofDist(user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition().x,user.getJoint(JOINT_RIGHT_HAND).getProjectivePosition().y,user.getJoint(JOINT_RIGHT_ELBOW).getProjectivePosition().x,user.getJoint(JOINT_RIGHT_ELBOW).getProjectivePosition().y)*0.5;
               //right_hand_height = right_hand_width;
               //right_hand_pos.x -= right_hand_width*0.5;
               //right_hand_pos.y -= right_hand_height*0.5;
         }
        /*
        left_foot_start = user.getJoint(JOINT_LEFT_FOOT);

        if(left_foot_start.isFound()){
            left_foot_pos = left_foot_start.getProjectivePosition();
        }


        right_foot_start = user.getJoint(JOINT_RIGHT_FOOT);
        if(right_foot_start.isFound())
            {
            right_foot_pos = right_foot_start.getProjectivePosition();
           }
        */

     }
    //cout << "HEADPOS="<<head_pos.x<<","<<head_pos.y << endl;
    //cout << "LPOS="<<left_hand_pos.x<<","<<left_hand_pos.y << endl;
    //cout << "RPOS="<<right_hand_pos.x<<","<<right_hand_pos.y << endl;


    if(!head_pos.x == 0){
        player_.moveLG(left_hand_pos.x,left_hand_pos.y);
        player_.moveRG(right_hand_pos.x,right_hand_pos.y);
        player_.moveFACE(head_pos.x-100,head_pos.y+200);
        player_.moveBODY(head_pos.x-120,head_pos.y+400);
    }


    player_.update();
    opponent_.update();

    P_LG = player_.getLGPos();
    P_RG = player_.getRGPos();
    P_FACE = player_.getFACEPos();

     O_LG = opponent_.getLGPos();
    O_RG = opponent_.getRGPos();
    O_ALL = opponent_.getALLpos();

    cameradepend(head_pos.x, head_pos.y);


    collisionPO();
    collisionOP();

    player_.takestuff(damagetoplayer,fatiguetoplayer,exptoplayer);
    opponent_.takestuff(damagetoopponent,fatiguetoopponent,0);

    if(opponent_.isdead())
    {
        cout << endl << "YOU WON!!!"<< endl;
        exitApp();
    }
    else if(player_.isdead())
    {
        cout << endl << "YOU LOSE!!!"<< endl;
        exitApp();
    }


}

void testApp::cameradepend(int px, int py){


    if(px < SCREEN_W*0.1 && (camera_.x-5) > 0) camera_.x -=5;

    if(px > SCREEN_W*0.9 && (camera_.x+5) < SUPRASCREEN_W-SCREEN_W) camera_.x +=5;

    if(py < SCREEN_H*0.1 && (camera_.y-5) > 0) camera_.y -=5;

    if(py > SCREEN_H*0.9 && (camera_.y+5) < SUPRASCREEN_H-SCREEN_H) camera_.y +=5;


}

void testApp::collisionPO(){

    if(opponent_.checkHitFace(P_LG.x,P_LG.y,player_.getLG_W(),player_.getLG_W())){ //player LG hit opponent face

        //effects to player
        player_.loadLGanimation(readTime,1,2);
        fatiguetoplayer = 25;
        //effects to opponent
        damagetoopponent = player_.getPower()*(player_.getStamina()*0.0035);
        opponent_.loadALLanimation(readTime,1,3);

    }else if(opponent_.checkHitFace(P_RG.x,P_RG.y,player_.getRG_W(),player_.getRG_W())){ //player RG hit opponent face

        //effects to player
        player_.loadRGanimation(readTime,1,2);
        fatiguetoplayer = 25;
        //effects to opponent
        damagetoopponent = player_.getPower()*(player_.getStamina()*0.0035);
        opponent_.loadALLanimation(readTime,1,3);

    }else if(opponent_.checkHitTorso(P_LG.x,P_LG.y,player_.getLG_W(),player_.getLG_W())){ //player LG hit opponent torso

         //effects to player
       player_.loadLGanimation(readTime,1,1);
        fatiguetoplayer = 25;
        //effects to opponent
        damagetoopponent = player_.getPower()*(player_.getStamina()*0.003);
        opponent_.loadALLanimation(readTime,1,3);

    }else if(opponent_.checkHitTorso(P_RG.x,P_RG.y,player_.getRG_W(),player_.getRG_W())){ //player RG hit opponent torso

        //effects to player
        player_.loadRGanimation(readTime,1,1);
        fatiguetoplayer = 25;
        //effects to opponent
        damagetoopponent = player_.getPower()*(player_.getStamina()*0.003);
        opponent_.loadALLanimation(readTime,1,3);

    }else{

        //restore values
        damagetoopponent = 0;
        fatiguetoplayer = 0;
        exptoplayer = 0;

    }
}

void testApp::collisionOP(){

    if(player_.checkHitFace(O_LG.x,O_LG.y,opponent_.getLG_W(),opponent_.getLG_W())){ //opponent LG hit player face

        //effects to opponent
        if(opponent_.getStamina() >= 5){
            opponent_.loadALLanimation(readTime,1,1);
            player_.playsound(1);
        }else{
            opponent_.loadALLanimation(readTime,1,4);
        }
        fatiguetoopponent = 25;
        //effects to player
        damagetoplayer = opponent_.getPower()*(opponent_.getStamina()*0.0035);

    }else if(player_.checkHitFace(O_RG.x,O_RG.y,opponent_.getRG_W(),opponent_.getRG_W())){ //player RG hit player face

        //effects to opponent
        if(opponent_.getStamina() >= 5){
            opponent_.loadALLanimation(readTime,1,2);
            player_.playsound(1);
        }else{
            opponent_.loadALLanimation(readTime,1,4);
        }fatiguetoopponent = 25;
        //effects to player
        damagetoplayer = opponent_.getPower()*(opponent_.getStamina()*0.0035);


    }else if(player_.checkHitTorso(O_LG.x,O_LG.y,opponent_.getLG_W(),opponent_.getLG_W())){ //player LG hit opponent torso

         //effects to opponent
        if(opponent_.getStamina() >= 5){
            opponent_.loadALLanimation(readTime,1,1);
            player_.playsound(1);

        }else{
            opponent_.loadALLanimation(readTime,1,4);
        }

        fatiguetoopponent = 25;
        //effects to opponent
        damagetoplayer = opponent_.getPower()*(opponent_.getStamina()*0.003);

    }else if(player_.checkHitTorso(O_RG.x,O_RG.y,opponent_.getRG_W(),opponent_.getRG_W())){ //player RG hit opponent torso

        //effects to player
        if(opponent_.getStamina() >= 5){
            opponent_.loadALLanimation(readTime,1,2);
            player_.playsound(1);
        }else{
            opponent_.loadALLanimation(readTime,1,4);
        }
        fatiguetoopponent = 25;
        //effects to opponent
       damagetoplayer = opponent_.getPower()*(opponent_.getStamina()*0.003);

    }else{

        //restore values
        damagetoplayer = 0;
        fatiguetoopponent = 0;

    }
}

//--------------------------------------------------------------
void testApp::draw(){

    /*ofPushMatrix();

    int numUsers = openNIDevice.getNumTrackedUsers();


    for (int i = 0; i < numUsers; i++){


        ofxOpenNIUser & user = openNIDevice.getTrackedUser(i);

    }

    ofPopMatrix();
    */

	scenario_.drawsub(camera_.x,camera_.y);

	opponent_.draw();
    player_.draw();


    drawFPS();

}

void testApp::userEvent(ofxOpenNIUserEvent & event){
    // show user event messages in the console
    ofLogNotice() << getUserStatusAsString(event.userStatus) << "for user" << event.id << "from device" << event.deviceID;
}

void testApp::exit(){
    openNIDevice.stop();
}

void testApp::drawFPS(){
    ofPushStyle();
    string info = "";
    info += "FPS: "+ofToString(ofGetFrameRate())+"\n";
    ofSetColor(40,40,90);
    ofDrawBitmapString(info, 30, 15);
	ofPopStyle();
}
//--------------------------------------------------------------
void testApp::keyPressed(int key){ //only keyboard here

    /*
     if(key == 'w')
        player_.moveFACE(P_FACE.x,P_FACE.y - 5),
        player_.moveBODY(P_TORSO.x, P_TORSO.y - 5);
    if(key == 's')
        player_.moveFACE(P_FACE.x,P_FACE.y + 5),
        player_.moveBODY(P_TORSO.x, P_TORSO.y + 5);
    if(key == 'a')
       player_.moveFACE(P_FACE.x - 5,P_FACE.y ),
       player_.moveBODY(P_TORSO.x - 5, P_TORSO.y);
    if(key == 'd')
        player_.moveFACE(P_FACE.x + 5,P_FACE.y ),
        player_.moveBODY(P_TORSO.x + 5, P_TORSO.y);
    */
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){ //only keyboard here

    if(key == 'w')
        ;
    if(key == 's')
        ;
    if(key == 'a')
        ;
    if(key == 'd')
        ;

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){


    /*
    if(x < SCREEN_W*0.2 && (camera_.x-10) > 0) camera_.x -=10;

    if(x > SCREEN_W*0.8 && (camera_.x+10) < SUPRASCREEN_W-SCREEN_W) camera_.x +=10;

    if(y < SCREEN_H*0.2 && (camera_.y-10) > 0) camera_.y -=10;

    if(y > SCREEN_H*0.8 && (camera_.y+10) < SUPRASCREEN_H-SCREEN_H) camera_.y +=10;
    */

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){
    player_.moveLG(x,y);

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
