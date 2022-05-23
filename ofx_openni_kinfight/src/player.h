#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <math.h>

#define SCREEN_W 800
#define SCREEN_H 600


class Fighter{
	public:

		Fighter();
		~Fighter();
		void setup();
		void update();
		void takestuff(int damagetaken, int staminataken, int exptaken);
		void draw();
		bool isdead();
        void hurt(int damage);
        void fatigue(int amount);
        void changeimageLEFTGLOVE(string path);
        void changeimageRIGHTGLOVE(string path);
        void changeimageLEFTFOOT(string path);
        void changeimageRIGHTFOOT(string path);
        bool isdebile();
        bool istired();
        int getPower();
        float getStamina();

        bool isAreahit(ofPoint entrance);

        void moveLG(int px, int py);
        void moveRG(int px, int py);
        void moveFACE(int px, int py);
        void moveBODY(int px, int py);


        void loadLGanimation(float moment, float sec, int type);
        void loadRGanimation(float moment, float sec, int type);
        void animateLG();
        void animateRG();

        ofPoint getLGPos();
        ofPoint getRGPos();
        int getLG_W();
        int getLG_H();
        int getRG_W();
        int getRG_H();

        ofPoint getFACEPos();
        ofPoint getTORSOPos();
        void setScreen(float W, float H);

        void playsound(int id);
        bool checkHitFace(int px, int py, int sw, int sh);
        bool checkHitTorso(int px, int py, int sw, int sh);

	private:

        int life;
        int stamina;
        int exp;
        int power;
        float maxstamina;

        ofImage face_;
        ofImage left_glove_;
        ofImage right_glove_;
        ofImage left_foot_;
        ofImage right_foot_;
        ofImage profile_;



        ofImage anim_L1;
        ofImage anim_L2;
        ofImage anim_L3;

        ofImage anim_R1;
        ofImage anim_R2;
        ofImage anim_R3;


        float		time;
        ofSoundPlayer sound_glass;


        ofPoint left_glove_pos;
        ofPoint right_glove_pos;
        ofPoint face_pos;
        ofPoint torso_pos;
        float LG_W;
        float LG_H;
        float RG_W;
        float RG_H;
        float FACE_W;
        float FACE_H;
        float TORSO_W;
        float TORSO_H;


        //animation
        float momentLG;
        float durationLG;
        int animationLGtype;

        float momentRG;
        float durationRG;
        int animationRGtype;

        float scr_W;
        float scr_H;


        ofxXmlSettings XML;
        string message;


        void restorestamina(float amount);
};
