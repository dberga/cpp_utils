#include "scenario.h"

//--------------------------------------------------------------
Scenario::Scenario(){




}

//--------------------------------------------------------------
Scenario::~Scenario(){

}

//--------------------------------------------------------------
void Scenario::setup(){


	cout << endl <<"LOADING SCENARIO" << endl;
    message = "loading game_data.xml";


    if( XML.loadFile("xml/game_data.xml") ){
        message = "game_data.xml loaded!";
        cout << endl << message << endl<< endl;

    cout << "SCENARIO: LOADING BACKGROUND" << endl;

        background_.loadImage(XML.getValue("BACKGROUNDS:SAND:PATH","backgrounds/sand/sand.PNG",0));
        background_.resize(SUPRASCREEN_W,SUPRASCREEN_H);

    song.loadSound(XML.getValue("BACKGROUNDS:SAND:SONG","backgrounds/sand/guile.mp3",0));

    cout << "DONE";

    }else{
        message = "unable to load game_data.xml check data/ folder";
        cout << endl << message << endl<< endl;
    }


    song.play();
    song.setVolume(song.getVolume()*0.5);





}

//--------------------------------------------------------------
void Scenario::update(){

    if(!song.getIsPlaying()) song.play();

}

//--------------------------------------------------------------
void Scenario::draw(){

    background_.resize(SCREEN_W,SCREEN_H);
    background_.draw(0,0);

}

void Scenario::drawsub(float posx, float posy){


        frustum_.cropFrom(background_,posx,posy,SCREEN_W,SCREEN_H);

        frustum_.draw(0,0);

}

