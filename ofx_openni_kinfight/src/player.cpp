#include "player.h"

//--------------------------------------------------------------
Fighter::Fighter(){

}
Fighter::~Fighter(){

}


void Fighter::changeimageLEFTGLOVE(string path){
    left_glove_.loadImage(path);
    left_glove_.resize(LG_W,LG_H);
}

void Fighter::changeimageRIGHTGLOVE(string path){
    right_glove_.loadImage(path);
    right_glove_.resize(RG_W,RG_H);
}


void Fighter::changeimageLEFTFOOT(string path){
    left_foot_.loadImage(path);
    left_foot_.resize(LG_W,LG_H);
}


void Fighter::changeimageRIGHTFOOT(string path){
    right_foot_.loadImage(path);
    right_foot_.resize(RG_W,RG_H);
}

bool Fighter::isdebile(){
    if(life <= 40) return true;
    else return false;
}

bool Fighter::istired(){
    if(stamina <= 40) return true;
    else return false;
}


void Fighter::moveLG(int px, int py){
    left_glove_pos.x = px;
    left_glove_pos.y = py;
}

void Fighter::moveRG(int px, int py){
    right_glove_pos.x = px;
    right_glove_pos.y = py;
}

void Fighter::moveFACE(int px, int py){
    face_pos.x = px;
    face_pos.y = py;
}

void Fighter::moveBODY(int px, int py){
    torso_pos.x = px;
    torso_pos.y = py;
}

void Fighter::setScreen(float W, float H){
    scr_W = W;
    scr_H = H;
}

//--------------------------------------------------------------
void Fighter::setup(){



     exp = 30; //to see, by default


    //setup values - sizes and positions
        LG_W = scr_W*0.1875;
        LG_H = scr_H*0.25;

        RG_W = scr_W*0.1875;
        RG_H = scr_H*0.25;

        FACE_W=scr_W*0.1875;
        FACE_H=scr_H*0.25;

        TORSO_W=scr_W*0.25;
        TORSO_H=scr_H*0.50;

        left_glove_pos.x=scr_W*0.0312; // = 0+25
        left_glove_pos.y=scr_H * 0.667;

        right_glove_pos.x=scr_W*0.781; // = 800-150-25 = 625
        right_glove_pos.y=scr_H * 0.667;

        face_pos.x = SCREEN_W*0.5-FACE_W*0.5;
        face_pos.y = SCREEN_H*0.6-FACE_H*0.5;

        torso_pos.x = SCREEN_W*0.5-TORSO_W*0.5;
        torso_pos.y = SCREEN_H*0.9-TORSO_H*0.5;

    //paths & XML values
        cout << endl <<"LOADING Fighter"<< endl;
        if( XML.loadFile("xml/game_data.xml") ){
            message = "game_data.xml loaded!";
            //cout << endl << message << endl<< endl;
            face_.loadImage(XML.getValue("PLAYERS:PLAYER1:FACE","players/player1/face.PNG",0));
            face_.resize(scr_W*0.075,scr_H*0.1);

            anim_L1.loadImage(XML.getValue("PLAYERS:PLAYER1:LEFTGLOVE","players/player1/left_glove.PNG",0));
            anim_L1.resize(LG_W,LG_H);
            anim_L2.loadImage(XML.getValue("PLAYERS:PLAYER1:LG_ANIMATION_HIT1","players/player1/left_glove_a1.PNG",0));
            anim_L2.resize(LG_W,LG_H);
            anim_L3.loadImage(XML.getValue("PLAYERS:PLAYER1:LG_ANIMATION_HIT2","players/player1/left_glove_a2.PNG",0));
            anim_L3.resize(LG_W,LG_H);

            anim_R1.loadImage(XML.getValue("PLAYERS:PLAYER1:RIGHTGLOVE","players/player1/right_glove.PNG",0));
            anim_R1.resize(RG_W,RG_H);
            anim_R2.loadImage(XML.getValue("PLAYERS:PLAYER1:RG_ANIMATION_HIT1","players/player1/right_glove_a1.PNG",0));
            anim_R2.resize(RG_W,RG_H);
            anim_R3.loadImage(XML.getValue("PLAYERS:PLAYER1:RG_ANIMATION_HIT2","players/player1/right_glove_a2.PNG",0));
            anim_R3.resize(RG_W,RG_H);

            right_glove_ = anim_R1;
            right_glove_.resize(RG_W,RG_H);

            left_glove_ = anim_L1;
            left_glove_.resize(LG_W,LG_H);

            life = XML.getValue("PLAYERS:PLAYER1:LIFE",250,0);
            stamina = XML.getValue("PLAYERS:PLAYER1:STAMINA",250,0);
            power = XML.getValue("PLAYERS:PLAYER1:POWER",20,0);
            maxstamina = XML.getValue("OPPONENTS:OPPONENT1:STAMINA",250,0);


            profile_.loadImage(XML.getValue("PLAYERS:PLAYER1:PROFILE","players/player1/profile.PNG",0));

             sound_glass.loadSound(XML.getValue("PLAYERS:PLAYER1:SOUND_GLASS","players/player1/glass.mp3",0));

            //int numDragTags = XML.getNumTags("TREES");
        cout << "DONE";
        }else{
            message = "unable to load game_data.xml check data/ folder";
            cout << endl << message << endl<< endl;
        }


    momentLG = 0;
    durationLG = 0;
    momentRG = 0;
    durationRG = 0;
}

ofPoint Fighter::getLGPos(){
    return left_glove_pos;
}

ofPoint Fighter::getRGPos(){
    return right_glove_pos;
}

void Fighter::playsound(int id){

    if(id==1) sound_glass.play();

}


int Fighter::getLG_W(){
    return LG_W;
}

int Fighter::getLG_H(){
    return LG_H;
}

int Fighter::getRG_W(){
    return RG_W;
}

int Fighter::getRG_H(){
    return LG_H;
}

ofPoint Fighter::getFACEPos(){
    return face_pos;
}

ofPoint Fighter::getTORSOPos(){
    return torso_pos;
}

void Fighter::loadLGanimation(float moment, float sec, int type){
    momentLG = moment;
	durationLG = sec;
	animationLGtype = type;

}

void Fighter::loadRGanimation(float moment, float sec, int type){
    momentRG = moment;
	durationRG = sec;
	animationRGtype = type;

}

void Fighter::animateLG(){


	if(animationLGtype != 0){
        if(momentLG + durationLG >= time && animationLGtype==1){
            left_glove_ = anim_L2;
        }else if(momentLG + durationLG >= time && animationLGtype==2){
            left_glove_ = anim_L3;
        }else{
            momentLG = 0;
            durationLG = 0;
            animationLGtype = 0;
        }
	}else{
        left_glove_ = anim_L1;
	}


}

void Fighter::animateRG(){
	if(animationRGtype != 0){
        if(momentRG + durationRG >= time && animationRGtype==1){
            right_glove_ = anim_R2;
        }else if(momentRG + durationRG >= time && animationRGtype==2){
            right_glove_ = anim_R3;
        }else{
            momentRG = 0;
            durationRG = 0;
            animationRGtype = 0;
        }
	}else{
	    //back to normal
       right_glove_ = anim_R1;
	}
}

bool Fighter::checkHitFace(int px, int py, int sw, int sh){ //px,py,sw,sh are opponent punch pos and size
    if(ofInRange(px,face_pos.x,face_pos.x+FACE_W) && ofInRange(py,face_pos.y,face_pos.y+FACE_H) || ofInRange(px+sw,face_pos.x,face_pos.x+FACE_W) && ofInRange(py+sh,face_pos.y,face_pos.y+FACE_H) || ofInRange(px+sw,face_pos.x,face_pos.x+FACE_W) && ofInRange(py,face_pos.y,face_pos.y+FACE_H) || ofInRange(px,face_pos.x,face_pos.x+FACE_W) && ofInRange(py+sh,face_pos.y,face_pos.y+FACE_H))
    {
        //sound_glass.play();
        return true;

    }
    else return false;
}

bool Fighter::checkHitTorso(int px, int py, int sw, int sh){

    if(ofInRange(px,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px+sw,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py+sh,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px+sw,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py+sh,torso_pos.y,torso_pos.y+TORSO_H))
    {
        //sound_glass.play();
        return true;

    }
    else return false;
}

//--------------------------------------------------------------
void Fighter::update(){
    time = ofGetSeconds();
    animateLG();
    animateRG();

    restorestamina(1);
}

 bool Fighter::isAreahit(ofPoint entrance){


}

float Fighter::getStamina(){
    return stamina;

}

void Fighter::restorestamina(float amount){
    if(stamina < maxstamina) stamina +=amount;
}

void Fighter::takestuff(int damagetaken, int staminataken, int exptaken){
    hurt(damagetaken);
    fatigue(staminataken);
}

bool Fighter::isdead(){

    if(life<=0) return true;
    else return false;
}

void Fighter::hurt(int damage){
    if(life > 0) life -= damage;

    if(life < 0 ) life = 0;
}

void Fighter::fatigue(int amount){
    if(stamina>0)
            stamina -= amount;

    if(stamina <0)
            stamina = 0;
}

int Fighter::getPower(){
    return power;
}


//--------------------------------------------------------------
void Fighter::draw(){



    //panel
    ofPushStyle();
        ofSetColor(255,0,0);
        ofRect(scr_W * 0.125,scr_H * 0.0834,life,scr_H * 0.034); //draw life panel
        ofSetColor(255,255,210);
        ofRect(scr_W * 0.125,scr_H*0.1167, stamina, scr_H * 0.034); //draw stamina panel
        ofSetColor(0,0,255);
        ofRect(scr_W * 0.125,scr_H * 0.15,exp,scr_H * 0.034); //draw exp panel
    ofPopStyle();
    face_.draw(scr_W*0.0375,scr_H*0.0834); //draw face panel







    //gloves
    left_glove_.draw(left_glove_pos);
    right_glove_.draw(right_glove_pos);

    //areas (head+torso) check
        ofPushStyle();
        ofSetColor(100,100,100);
        //ofRect(face_pos,FACE_W,FACE_H);
        //ofRect(torso_pos,TORSO_W,TORSO_H);
        profile_.draw(face_pos,TORSO_W*0.7,(FACE_H+TORSO_H)*0.7);
        ofPopStyle();

}
