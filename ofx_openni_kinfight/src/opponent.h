#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include <math.h>

#define SCREEN_W 800
#define SCREEN_H 600


class Opponent{
	public:

		Opponent();
		~Opponent();
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
        void changeimageALL(string path);
        bool isdebile();
        bool istired();
        int getPower();
        float getStamina();
        ofPoint getForeingRandPosOut(ofPoint value, int coef);
         ofPoint getForeingRandPosIn(ofPoint value, int coef);
        bool isAreahit(ofPoint entrance);

        void moveLG(int px, int py);
        void moveRG(int px, int py);

        void loadLGanimation(float moment, float sec, int type);
        void loadRGanimation(float moment, float sec, int type);
        void loadALLanimation(float moment, float sec, int type);
        void animateLG();
        void animateRG();
        void animateALL();

        ofPoint getLGPos();
        ofPoint getRGPos();
        int getLG_W();
        int getLG_H();
        int getRG_W();
        int getRG_H();
        void setALLpos(float x, float y);
        ofPoint getALLpos();
        ofPoint getTORSOPos();
        ofPoint getFACEPos();

        void setLastSUPRAdelims(float W, float H);
        void setFirstSUPRAdelims(float W, float H);

        void setScreen(float W, float H);

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
        ofImage all;
        ofImage anim_standard;
        ofImage anim_hurted;
        ofImage anim_punchingR;
        ofImage anim_punchingL;
        ofImage anim_gonnapunch;


        float		gametime;

        ofSoundPlayer sound_punch1;
        ofSoundPlayer sound_punch2;


        ofPoint left_glove_pos;
        ofPoint right_glove_pos;
        ofPoint all_pos;
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

        float ALL_W;
        float ALL_H;


        //animation
        float momentLG;
        float durationLG;
        int animationLGtype;

        float momentRG;
        float durationRG;
        int animationRGtype;

        float momentALL;
        float durationALL;
        int animationALLtype;





        ofxXmlSettings XML;
        string message;

        float delimFW;
        float delimFH;

        float delimLW;
        float delimLH;

        float scr_W;
        float scr_H;




        void restorestamina(float amount);
};
