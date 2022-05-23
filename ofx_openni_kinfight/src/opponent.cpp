#include "opponent.h"

//--------------------------------------------------------------
Opponent::Opponent(){

}
Opponent::~Opponent(){

}


void Opponent::changeimageLEFTGLOVE(string path){
    left_glove_.loadImage(path);
    left_glove_.resize(LG_W,LG_H);
}

void Opponent::changeimageRIGHTGLOVE(string path){
    right_glove_.loadImage(path);
    right_glove_.resize(RG_W,RG_H);
}


void Opponent::changeimageLEFTFOOT(string path){
    left_foot_.loadImage(path);
    left_foot_.resize(LG_W,LG_H);
}


void Opponent::changeimageRIGHTFOOT(string path){
    right_foot_.loadImage(path);
    right_foot_.resize(RG_W,RG_H);
}

void Opponent::changeimageALL(string path){
    all.loadImage(path);
    all.resize(ALL_W,ALL_H);
}

bool Opponent::isdebile(){
    if(life <= 40) return true;
    else return false;
}

bool Opponent::istired(){
    if(stamina <= 40) return true;
    else return false;
}


void Opponent::moveLG(int px, int py){
    left_glove_pos.x = px;
    left_glove_pos.y = py;
}

void Opponent::moveRG(int px, int py){
    right_glove_pos.x = px;
    right_glove_pos.y = py;
}

void Opponent::setALLpos(float x, float y){
    all_pos.x = x;
    all_pos.y = y;

}

ofPoint Opponent::getALLpos(){
   return all_pos;
}


//--------------------------------------------------------------
void Opponent::setup(){



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

        ALL_W = scr_W*0.25;
        ALL_H = scr_H*0.75;

        left_glove_pos.x=scr_W*0.0312; // = 0+25
        left_glove_pos.y=scr_H * 0.667;



        right_glove_pos.x=scr_W*0.781; // = 800-150-25 = 625
        right_glove_pos.y=scr_H * 0.667;

        face_pos.x = scr_W*0.5-FACE_W*0.5;
        face_pos.y = scr_H*0.28-FACE_H*0.5;

        torso_pos.x = face_pos.x - FACE_W*0.2;
        torso_pos.y = face_pos.y + FACE_H*0.2;

        all_pos.x = scr_W*0.5-ALL_W*0.5;
        all_pos.y = scr_H*0.6-ALL_H*0.5;

    //paths & XML values
        cout << endl <<"LOADING Opponent"<< endl;
        if( XML.loadFile("xml/game_data.xml") ){
            message = "game_data.xml loaded!";
            //cout << endl << message << endl<< endl;
            face_.loadImage(XML.getValue("OPPONENTS:OPPONENT1:FACE","opponents/opponent1/face.PNG",0));
            face_.resize(scr_W*0.075,scr_H*0.1);


            right_glove_.loadImage(XML.getValue("OPPONENTS:OPPONENT1:RIGHTGLOVE","opponents/opponent1/right_glove.PNG",0));
            right_glove_.resize(RG_W,RG_H);

            left_glove_.loadImage(XML.getValue("OPPONENTS:OPPONENT1:LEFTGLOVE","opponents/opponent1/left_glove.PNG",0));
            left_glove_.resize(LG_W,LG_H);

            anim_standard.loadImage(XML.getValue("OPPONENTS:OPPONENT1:ANIMATION1","opponents/opponent1/standard.PNG",0));
            anim_standard.resize(ALL_W,ALL_H);
            anim_hurted.loadImage(XML.getValue("OPPONENTS:OPPONENT1:ANIMATION4","opponents/opponent1/hurted.PNG",0));
            anim_hurted.resize(ALL_W,ALL_H);
            anim_punchingR.loadImage(XML.getValue("OPPONENTS:OPPONENT1:ANIMATION2","opponents/opponent1/left_punch.PNG",0));
            anim_punchingR.resize(ALL_W,ALL_H);
            anim_punchingL.loadImage(XML.getValue("OPPONENTS:OPPONENT1:ANIMATION3","opponents/opponent1/right_punch.PNG",0));
            anim_punchingL.resize(ALL_W,ALL_H);
            anim_gonnapunch.loadImage(XML.getValue("OPPONENTS:OPPONENT1:ANIMATION5","opponents/opponent1/gonnapunch.PNG",0));
            anim_gonnapunch.resize(ALL_W,ALL_H);


            all = anim_standard;
            all.resize(ALL_W,ALL_H);

            life = XML.getValue("OPPONENTS:OPPONENT1:LIFE",250,0);
            stamina = XML.getValue("OPPONENTS:OPPONENT1:STAMINA",250,0);
            power = XML.getValue("OPPONENTS:OPPONENT1:POWER",20,0);
            maxstamina = XML.getValue("OPPONENTS:OPPONENT1:STAMINA",250,0);

            sound_punch1.loadSound(XML.getValue("OPPONENTS:OPPONENT1:SOUND_PUNCH1","opponents/opponent1/punch1.mp3",0));
            sound_punch2.loadSound(XML.getValue("OPPONENTS:OPPONENT1:SOUND_PUNCH2","opponents/opponent1/punch2.mp3",0));


            //int numDragTags = XML.getNumTags("TREES");
        cout << "DONE";
        }else{
            message = "unable to load game_data.xml check data/ folder";
            cout << endl << message << endl<< endl;
        }


    momentALL = 0;
    durationALL = 0;
    momentRG = 0;
    durationRG = 0;
    momentLG = 0;
    durationLG = 0;

    //cout << "Fdelims:" << delimFW << ","<<delimFH << endl;
    //cout << "Ldelims:" << delimLW << ","<<delimLH << endl;
}

ofPoint Opponent::getLGPos(){
    return left_glove_pos;
}

ofPoint Opponent::getRGPos(){
    return right_glove_pos;
}

ofPoint Opponent::getFACEPos(){
    return face_pos;
}

ofPoint Opponent::getTORSOPos(){
    return torso_pos;
}

int Opponent::getLG_W(){
    return LG_W;
}

int Opponent::getLG_H(){
    return LG_H;
}

int Opponent::getRG_W(){
    return RG_W;
}

int Opponent::getRG_H(){
    return LG_H;
}


void Opponent::loadLGanimation(float moment, float sec, int type){
    momentLG = moment;
	durationLG = sec;
	animationLGtype = type;

}

void Opponent::loadRGanimation(float moment, float sec, int type){
    momentRG = moment;
	durationRG = sec;
	animationRGtype = type;

}

void Opponent::loadALLanimation(float moment, float sec, int type){
    momentALL = moment;
	durationALL = sec;
	animationALLtype = type;

}

void Opponent::animateLG(){


	if(animationLGtype != 0){
        if(momentLG + durationLG >= gametime && animationLGtype==1){
            changeimageLEFTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:LEFTGLOVE","opponents/opponent1/left_glove.PNG",0));
        }else if(momentLG + durationLG >= gametime && animationLGtype==2){
            changeimageLEFTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:LEFTGLOVE","opponents/opponent1/left_glove.PNG",0));
        }else{
            momentLG = 0;
            durationLG = 0;
            animationLGtype = 0;
        }
	}else{
        changeimageLEFTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:LEFTGLOVE","opponents/opponent1/left_glove.PNG",0));
	}


}

void Opponent::animateRG(){
	if(animationRGtype != 0){
        if(momentRG + durationRG >= gametime && animationRGtype==1){
            changeimageRIGHTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:RIGHTGLOVE","opponents/opponent1/right_glove.PNG",0));
        }else if(momentRG + durationRG >= gametime && animationRGtype==2){
            changeimageRIGHTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:RIGHTGLOVE","opponents/opponent1/right_glove.PNG",0));
        }else{
            momentRG = 0;
            durationRG = 0;
            animationRGtype = 0;
        }
	}else{
	    //back to normal
        changeimageRIGHTGLOVE(XML.getValue("OPPONENTS:OPPONENT1:RIGHTGLOVE","opponents/opponent1/right_glove.PNG",0));
	}
}


void Opponent::animateALL(){


	if(animationALLtype != 0){
        if(momentALL + durationALL >= gametime && animationALLtype==1){
             all = anim_punchingL;
        }else if(momentALL + durationALL >= gametime && animationALLtype==2){
            all = anim_punchingR;
        }else if(momentALL + durationALL >= gametime && animationALLtype==3){
            all = anim_hurted;
        }else if(momentALL + durationALL >= gametime && animationALLtype==4){
            all = anim_gonnapunch;
        }else{
            momentALL = 0;
            durationALL = 0;
            animationALLtype = 0;
        }
	}else{
        all = anim_standard;
	}


}

bool Opponent::checkHitFace(int px, int py, int sw, int sh){ //px,py,sw,sh are opponent punch pos and size
    if(ofInRange(px,face_pos.x,face_pos.x+FACE_W) && ofInRange(py,face_pos.y,face_pos.y+FACE_H) || ofInRange(px+sw,face_pos.x,face_pos.x+FACE_W) && ofInRange(py+sh,face_pos.y,face_pos.y+FACE_H) || ofInRange(px+sw,face_pos.x,face_pos.x+FACE_W) && ofInRange(py,face_pos.y,face_pos.y+FACE_H) || ofInRange(px,face_pos.x,face_pos.x+FACE_W) && ofInRange(py+sh,face_pos.y,face_pos.y+FACE_H))
    {
      sound_punch1.play();
      return true;
    }
    else return false;
}

bool Opponent::checkHitTorso(int px, int py, int sw, int sh){

    if(ofInRange(px,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px+sw,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py+sh,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px+sw,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py,torso_pos.y,torso_pos.y+TORSO_H) || ofInRange(px,torso_pos.x,torso_pos.x+TORSO_W) && ofInRange(py+sh,torso_pos.y,torso_pos.y+TORSO_H))
    {
      sound_punch2.play();
      return true;
    }
    else return false;
}

ofPoint Opponent::getForeingRandPosOut(ofPoint value, int coef){


    ofPoint retorna;
    bool bueno = false;


    while(!bueno){
        retorna.x = value.x + (rand() % (coef*2) - coef);
        retorna.y = value.y + (rand() % (coef*2) - coef);
        if(ofInRange(retorna.x,delimFW,delimLW) && ofInRange(retorna.y,delimFH,delimLH))
            bueno = true;
    }
    return retorna;
}

ofPoint Opponent::getForeingRandPosIn(ofPoint value, int coef){


    ofPoint retorna;
    bool bueno = false;

    while(!bueno){

        retorna.x = value.x + (rand() % (coef*2) - coef);
        retorna.y = value.y + (rand() % (coef*2) - coef);
        if(ofInRange(retorna.x,0,scr_W) && ofInRange(retorna.y,0,scr_H))
            bueno = true;
    }
    return retorna;
}


void Opponent::setLastSUPRAdelims(float W, float H){
    delimLW = W;
    delimLH = H;
}

void Opponent::setFirstSUPRAdelims(float W, float H){
    delimFW = W;
    delimFH = H;
}


void Opponent::setScreen(float W, float H){
    scr_W = W;
    scr_H = H;
}

//--------------------------------------------------------------
void Opponent::update(){
    gametime = ofGetSeconds();
    //animateLG();
    //animateRG();
    animateALL();

    //points moving random
    all_pos = getForeingRandPosIn(all_pos,3);
    face_pos = all_pos;
    torso_pos.x = face_pos.x - FACE_W*0.2;
    torso_pos.y = face_pos.y + FACE_H*1.2;

    left_glove_pos = getForeingRandPosIn(left_glove_pos,50);
    right_glove_pos = getForeingRandPosIn(right_glove_pos,50);


    restorestamina(1);
}

 bool Opponent::isAreahit(ofPoint entrance){


}


float Opponent::getStamina(){
    return stamina;

}

void Opponent::restorestamina(float amount){
    if(stamina < maxstamina) stamina +=amount;
}

void Opponent::takestuff(int damagetaken, int staminataken, int exptaken){
    hurt(damagetaken);
    fatigue(staminataken);
}

bool Opponent::isdead(){

    if(life<=0) return true;
    else return false;
}

void Opponent::hurt(int damage){
    if(life > 0) life -= damage;

    if(life < 0 ) life = 0;
}

void Opponent::fatigue(int amount){
    if(stamina>0)
            stamina -= amount;

    if(stamina <0)
            stamina = 0;
}

int Opponent::getPower(){
    return power;
}


//--------------------------------------------------------------
void Opponent::draw(){

    //panel
    ofPushStyle();
        ofSetColor(255,0,0);
        ofRect(scr_W-scr_W * 0.125,scr_H * 0.0834,-life,scr_H * 0.034); //draw life panel
        ofSetColor(255,255,210);
        ofRect(scr_W-scr_W * 0.125,scr_H*0.1167, -stamina, scr_H * 0.034); //draw stamina panel
        //ofSetColor(0,0,255);
        //ofRect(scr_W * 0.125,scr_H * 0.15,exp,scr_H * 0.034); //draw exp panel
    ofPopStyle();
    face_.draw(scr_W-scr_W*0.1,scr_H*0.0834); //draw face panel




    //cout<< "opp pos="<<all_pos.x<< "," << all_pos.y << endl;

    //gloves
    ofPushStyle();
    ofSetColor(200,100,100);
    left_glove_.draw(left_glove_pos);
    right_glove_.draw(right_glove_pos);
    ofPopStyle();

    all.draw(all_pos);


    //areas (head+torso) check
    /*
    ofPushStyle();
    ofSetColor(0,0,254);
    ofRect(face_pos,FACE_H,FACE_W);
    ofRect(torso_pos,TORSO_W,TORSO_H);
    ofPopStyle();
    */

}
