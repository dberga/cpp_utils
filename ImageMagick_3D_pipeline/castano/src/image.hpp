/*** image.h  Javi Agenjo (javi.agenjo@gmail.com) 2013
	This file defines the class Image that allows to manipulate images.
	It defines all the need operators for Color and Image.
	It has a TGA loader and saver.
***/

#ifndef IMAGE_H
#define IMAGE_H

#include <string.h>
#include <stdio.h>
#include <iostream>
#include <algorithm>

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

float clamp(float x, float a, float b);

#pragma warning(disable : C4996)

class Color
{
public:
	union {
		struct { float r; float g; float b; float z;};
		float v[4];
	};

	Color();
	Color(float r, float g, float b);
	void set(float r, float g, float b);
};

Color operator * (const Color& c,float v);
void operator *= (Color& c,float v) ;
Color operator / (const Color& c,float v) ;
void operator /= (Color& c,float v);
Color operator * (float v, const Color& c);
Color operator + (const Color& a, const Color& b) ;
void operator += (Color& a,const Color& b) ;
Color operator - (const Color& a, const Color& b) ;
void operator -= (Color& a,const Color& b) ;

class Image
{
	//a general struct to store all the information about a TGA file
	typedef struct sTGAInfo 
	{
		unsigned int width;
		unsigned int height;
		unsigned int bpp; //bits per pixel
		unsigned char* data; //bytes with the pixel information
	} TGAInfo;

public:
	unsigned int width;
	unsigned int height;
	Color* pixels;
	float* zBuffer;

	/* CONSTRUCTORS */
	Image() ;

	Image(unsigned int width, unsigned int height);

	//copy constructor
	Image(const Image& c);

	//assign operator
	Image& operator = (const Image& c)
	{
		if(pixels) delete pixels;
		pixels = NULL;
		if(zBuffer) delete zBuffer;
		zBuffer = NULL;

		width = c.width;
		height = c.height;
		if(c.pixels)
		{
			pixels = new Color[width*height*sizeof(Color)];
			memcpy(pixels, c.pixels, width*height*sizeof(Color));
		}
		if(c.zBuffer)
		{
			zBuffer = new float[width*height];
			memcpy(zBuffer, c.zBuffer, width*height*sizeof(float));
		}
		return *this;
	}

	~Image();

	//get the pixel at position x,y
	Color getPixel(unsigned int x, unsigned int y) const;

	//get the pixel at position x,y
	float getZbuffer(unsigned int x, unsigned int y) const;
	//set the pixel at position x,y with value C
	void setZbuffer( unsigned int x, unsigned int y , const float z);
	
	
	//set the pixel at position x,y with value C
	void setPixel(const Color& c, unsigned int x, unsigned int y);

	//change image size (the old one will remain in the top-left corner)
	void resize(unsigned int width, unsigned int height);


	//Loads an image from a TGA file
	bool loadTGA(const char* filename);

	// Saves the image to a TGA file
	bool saveTGA(const char* filename);

};



#endif
