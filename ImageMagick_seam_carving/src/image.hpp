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
#include <sstream>

#include <cmath>
#include <cfloat>
#include "mask.hpp"
#include "seam.hpp"
#include "gradient.hpp"

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif



float clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }


#pragma warning(disable : C4996)



class Color
{
public:
	union {
		struct { float r; float g; float b; };
		float v[3];
	};

	Color() : r(0.0), g(0.0), b(0.0) { }
	Color(float r, float g, float b) : r(r), g(g), b(b) {}
	void set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
	// Clampeamos el color entre 0 y 1
	void clampColor() { for (int i = 0; i < 3; ++i)v[i] = clamp(v[i],0.0,1.0);}
	// valor absoluto de color (para negativos)
	void absColor() {r = fabs(r); g = fabs(g); b = fabs(b);}
	// devolver grey BT.709 television digital
	float getGreyValue(){return 0.2125*r+0.7154*g+0.072*b;}

	
};

Color operator * (const Color& c,float v) { return Color(c.r*v, c.g*v, c.b*v); }
void operator *= (Color& c,float v) { c = c * v; }
Color operator / (const Color& c,float v) { return Color(c.r/v, c.g/v, c.b/v); }
void operator /= (Color& c,float v) { c = c / v; }
Color operator * (float v, const Color& c) { return Color(c.r*v, c.g*v, c.b*v); }
Color operator + (const Color& a, const Color& b) { return Color(a.r+b.r, a.g+b.g, a.b+b.b); }
void operator += (Color& a,const Color& b) { a = a + b; }
Color operator - (const Color& a, const Color& b) { return Color(a.r-b.r, a.g-b.g, a.b-b.b); }
void operator -= (Color& a,const Color& b) { a = a - b; }
//assign operator

// Definimos el operador << para outputear colores
std::ostream& operator<<(std::ostream &strm, const Color &a) { return strm << "Color(R=" << a.r << "G=" << a.g << "R=" << a.b << ")"; }


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

	/* CONSTRUCTORS */
	Image() : width (0), height(0), pixels(NULL)
	{
	}

	Image(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;
		pixels = new Color[width*height];
		memset(pixels, 0, width * height * sizeof(Color));
	}

	//copy constructor
	Image(const Image& c) {
		if(pixels) delete pixels;
		pixels = NULL;

		width = c.width;
		height = c.height;
		if(c.pixels)
		{
			pixels = new Color[width*height];
			memcpy(pixels, c.pixels, width*height*sizeof(Color));
		}
	}

	//assign operator
	Image& operator = (const Image& c)
	{
		if(pixels) delete pixels;
		pixels = NULL;

		width = c.width;
		height = c.height;
		if(c.pixels)
		{
			pixels = new Color[width*height*sizeof(Color)];
			memcpy(pixels, c.pixels, width*height*sizeof(Color));
		}
		return *this;
	}

	~Image()
	{
		if(pixels) delete pixels;
	}

	//para crear una imagen a partir de un gradiente 
	Image(Gradient * c) {
		width = c->width;
		height = c->height;
		pixels = new Color[width*height];
		for(unsigned int x = 0; x < width; ++x)
			for(unsigned int y = 0; y < height; ++y){
				//misma energia para cada canal porque es escala de grises
				Color color(c->getEnergy(x,y),c->getEnergy(x,y),c->getEnergy(x,y));
				pixels[ y * width + x ] = color;
			}
	}

	//get the pixel at position x,y
	Color getPixel(const unsigned int x, const unsigned int y) const 
	{
		return pixels[ y * width + x ];
	}

	//set the pixel at position x,y with value C
	void setPixel(const Color& c, unsigned int x, unsigned int y)
	{
		pixels[ y * width + x ] = c;
	}

	//change image size (the old one will remain in the top-left corner)
	void resize(const unsigned int width, const unsigned int height)
	{
		Color* new_pixels = new Color[width*height];
		unsigned int min_width = this->width > width ? width : this->width;
		unsigned int min_height = this->height > height ? height : this->height;

		

		for(unsigned int x = 0; x < min_width; ++x)
			for(unsigned int y = 0; y < min_height; ++y)
				new_pixels[ y * width + x ] = getPixel(x,y);

		delete [] pixels;
		this->width = width;
		this->height = height;
		pixels = new_pixels;
	}

	void resizeCarving(const unsigned int new_width, const unsigned int new_height)
	{

		// incrementos o decrementos de altura y ancho
		int inc_w,inc_h;

		inc_w=width-new_width;
		inc_h=height-new_height;

		Gradient * gradient = new Gradient(width,height);
		
		Mask mask;
				

		/////////////////////////// RESIZE HORIZONTAL

		//miramos si es positivo el n??mero, si no es negativo quiere decir que hacemos reduccion del ancho de la imagen
		if(inc_w>0){		
			
			std::cout<<"REDUCCI??N "<<inc_w<<" PIXELES DEL ANCHO DE LA IMAGEN.."<<std::endl;

			// seams ver
			Seam * s_vert = new VerticalSeam(height);
			Seam * s_ver_aux = new VerticalSeam(height);
			for(int i=0;i<inc_w;++i)
			{
				getPartialGradient(mask , gradient , s_vert);
				gradient->getMinSeamVertical(s_vert,s_ver_aux);	
				removeSeamVertical(s_vert);
				gradient->removeSeamVertical(s_vert);
			}

			delete s_vert;
			delete s_ver_aux;
		}
		//////////////////////// END RESIZE HORIZONTAL


		////////////////////////// RESIZE VERTICAL

		//miramos si es positivo el n??mero, si no es negativo quiere decir que hacemos reduccion del alto de la imagen
		if(inc_h>0){		
			
			std::cout<<"REDUCCI??N "<<inc_h<<" PIXELES DEL ALTO DE LA IMAGEN.."<<std::endl;

			// seams hors
			Seam * s_hor = new HorizontalSeam(width);
			Seam * s_hor_aux = new HorizontalSeam(width);
			for(unsigned int i=0;i<inc_h;++i)
			{
				getPartialGradient(mask , gradient , s_hor);
				gradient->getMinSeamHorizontal(s_hor,s_hor_aux);		
				removeSeamHoritzontal(s_hor);		
				gradient->removeSeamHoritzontal(s_hor);
			}
			delete s_hor;
			delete s_hor_aux;
		
		}
		/////////////////////// END RESIZE VERTICAL


		
		/////////////////////////// EXPAND  HORIZONTAL

		//miramos si es positivo el n??mero, si es negativo quiere decir que hacemos ampliacion del ancho de la imagen
		if(inc_w<0){	

			int numseams=inc_w*(-1);

			std::cout<<"EXPANSI??N "<<numseams<<" PIXELES DEL ANCHO DE LA IMAGEN.."<<std::endl;

			Seam *s=new VerticalSeam[numseams];
			Seam *s_vert=new VerticalSeam(height);
			Seam *s_ver_aux=new VerticalSeam(height);
			getGradient(mask , gradient);

			//calcular el n??mero de seams a expandir
			for(unsigned int i=0;i<numseams;++i)
			{	
				gradient->getMinSeamVertical(s_vert , s_ver_aux);
				s[i] = *s_vert;
				//ponemos m??xima energ??a al seam para no volverlo a coger
				gradient->drawMaxGradient(&s[i]);
			}


			int dist;
			for(int i=numseams-1;i>=0;--i)
			{

				dist=0;
				resize(width+1,height);		
				
				for(int k=i+1;k<numseams-1;++k)
				{
					// miramos cuantos pixels hay que desplazar por los seams a??adidos
					if(s[i].getPixel(0).x > s[k].getPixel(0).x) dist+=1;
				}	
				expandSeamVertical(s[i],dist);	
			}
			delete s_vert;
			delete s_ver_aux;
			delete [] s;			

		}
		////////////////////////////// END EXPAND HORIZONTAL
		


		/////////////////////////// EXPAND  VERTICAL

		//miramos si es positivo el n??mero, si es negativo quiere decir que hacemos ampliacion del alto de la imagen
		if(inc_h<0){	

			int numseams=inc_h*(-1);

			std::cout<<"EXPANSI??N "<<numseams<<" PIXELES DEL ALTO DE LA IMAGEN.."<<std::endl;			

			delete gradient;
			gradient = new Gradient(width,height);

			Seam *s=new HorizontalSeam[numseams];
			Seam *s_hor=new HorizontalSeam(width);
			Seam *s_hor_aux=new HorizontalSeam(width);
			getGradient(mask , gradient);
			for(int i=0;i<numseams;++i)
			{	
				gradient->getMinSeamHorizontal(s_hor , s_hor_aux);
				s[i] = *s_hor;
				gradient->drawMaxGradient(&s[i]);
			}

			int dist;
			for(int i=numseams-1;i>=0;--i)
			{
				dist=0;
				resize(width,height+1);		
				for(int k=i+1;k<numseams-1;++k)
				{
					// miramos cuantos pixels hay que desplazar por los seams a??adidos
					if(s[i].getPixel(0).y > s[k].getPixel(0).y) dist+=1;
				}	
				expandSeamHorizontal(s[i],dist);
			}
			delete s_hor;
			delete s_hor_aux;
			delete [] s;

		}
		////////////////////////////// END EXPAND VERTICAL
		

		delete gradient;
		
	}

	// calcula el gradiente entero
	void getGradient(const Mask& mask ,Gradient * gradient)
	{
		for(int x = 0; x < width; ++x)
		{
			for(int y = 0; y < height; ++y)
			{
				applyMask(mask, x, y, gradient);
			}
		}
	}

	// calcula solo la parte del gradiente donde borramos el seam
	void getPartialGradient(const Mask& mask ,Gradient * gradient, Seam * s)
	{
		// miramos si el seam existe
		if(s->isEmpty()){
			// calculamos el gradiente de 0
			getGradient(mask,gradient);
		}
		else
		{
			// calculamos solo la parte del gradiente donde borramos el seam
			for(int k = 0; k < s->size; ++k)
			{

				for(int i = -(mask.center_x)  ; i <= mask.center_x ; ++i){
					for(int j = -(mask.center_y) ; j <= mask.center_y; ++j)
					{

						applyMask(mask, s->isInsideX(k,i,width) , s->isInsideY(k,j,height), gradient);
												
					}
				}
			}
		}
	}


	// aplicamos la masacara para calcular el gradiente en el pixel X Y
	void applyMask(const Mask & mask,  unsigned int x, unsigned int y, Gradient * gradient){
		Color grad_x;
		Color grad_y;
		// el indice medio de una array 3x1 o 3x3
		int mask_value;
		float energy = 0;
		for(int i = -(mask.center_x)  ; i <= mask.center_x ; i+=2){
			for(int j = -(mask.center_y) ; j <= mask.center_y; j+=2)
			{
				mask_value = mask.getValue(mask.center_x + i, mask.center_y + j);
				grad_x += mask_value *getPixel((x+i > width-1 ||  x+i < 0) ? x : x+i,y);
				grad_y += mask_value *getPixel(x,(y+i > height-1 || y+i < 0 ) ? y : y+i);

			}
		}
		// ponemos en positivo los colores para saber si hay cambio
		grad_x.absColor();
		grad_y.absColor();
		// los clampeamos entre 0 y 1
		grad_x.clampColor();
		grad_y.clampColor();

		// calculamos el valor total del gradiente
		energy = gradient->energyFunction(grad_x.getGreyValue(), grad_y.getGreyValue());

		// ponemos el color del gradiente en el resultado
		gradient->setEnergy( energy, x, y);
		// reseteamos los gradientes
		grad_x.set(0,0,0);
		grad_y.set(0,0,0);

	}
	
	void drawSeam(Seam * s){

		Color red(1.0,0,0);

		for(unsigned int x = 0; x < s->size; ++x){
				setPixel(red, s->getPixel(x).x , s->getPixel(x).y);
		}

	}


	void movePixelRight(unsigned int x,unsigned int y){
		
		for(unsigned int i=x;i<width-1;++i){
			setPixel(getPixel(i+1,y),i,y);
		}
	}

	void movePixelDown(unsigned int x,unsigned int y){

		for(unsigned int j=y;j<height-1;++j){
			setPixel(getPixel(x,j+1),x,j);
		}
	}


	
	void expandPixelRight(unsigned int x,unsigned int y){
		unsigned int i;			
		for(i=width-1;i>x;--i){
			
			setPixel(getPixel(i-1,y),i,y);
		}
		//Para hacer la media de p??xeles al agrandar (sale mejor sin la media las im??genes)
		//setPixel(getMeanVertical(i,y),i,y);
	}

	void expandPixelDown(unsigned int x,unsigned int y){
		unsigned int j;
		for(j=height-1;j>y;--j){
			
			setPixel(getPixel(x,j-1),x,j);
		}
		//Para hacer la media de p??xeles al agrandar (sale mejor sin la media las im??genes)
		//setPixel(getMeanHorizontal(x,j),x,j);
	}

	//Devuelve el color medio de la posici??n x e y
	Color getMeanHorizontal(int x,int y){

		Color c;
		if(x+1>=width) c=(getPixel(x,y)+getPixel(x-1,y))/2;
		else if(x-1<0) c=(getPixel(x,y)+getPixel(x+1,y))/2;
		else c=(getPixel(x,y)+getPixel(x+1,y)+getPixel(x-1,y))/3;
		return c;
	}	

	Color getMeanVertical(int x,int y){

		Color c;
		if(y+1>=height) c=(getPixel(x,y)+getPixel(x,y-1))/2;
		else if(y-1<0) c=(getPixel(x,y)+getPixel(x,y+1))/2;
		else c=(getPixel(x,y)+getPixel(x,y-1)+getPixel(x,y-1))/3;
		return c;
	}	


	void removeSeamHoritzontal(Seam* s){

		for(unsigned int i=0;i<s->size;++i){
			movePixelDown(s->getPixel(i).x,s->getPixel(i).y);
		}
		resize(width,height-1);
	}


	void removeSeamVertical(Seam* s){

		for(unsigned int j=0;j<s->size;++j){
			movePixelRight(s->getPixel(j).x,s->getPixel(j).y);
		}
		resize(width-1,height);
	}
	

	//para duplicar seams para agrandar la imagen
	void expandSeamHorizontal(const Seam& s,unsigned int dist){

		for(unsigned int i=0;i<s.size;++i){
			expandPixelDown(s.getPixel(i).x,s.getPixel(i).y + dist);
		}
	}

	void expandSeamVertical(const Seam& s,unsigned int dist){

		for(unsigned int j=0;j<s.size;++j){
			expandPixelRight(s.getPixel(j).x+dist,s.getPixel(j).y);
		}
	}


	void debugGradient(Gradient * partial , Gradient * full){


		for(unsigned int x = 0; x < partial->width; ++x)
			for(unsigned int y = 0; y < partial->height; ++y)
				if(partial->getEnergy(x,y) != full->getEnergy(x,y)) 
					std::cout << "El gradiente parcial esta mal calculado " << x << " " << y << std::endl;

	}



	//Loads an image from a TGA file
	bool loadTGA(const char* filename)
	{
		unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		unsigned char TGAcompare[12];
		unsigned char header[6];
		unsigned int bytesPerPixel;
		unsigned int imageSize;

		FILE * file = fopen(filename, "rb");
   		if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
			memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
			fread(header, 1, sizeof(header), file) != sizeof(header))
		{
			std::cerr << "File not found: " << filename << std::endl;
			if (file == NULL)
				return NULL;
			else
			{
				fclose(file);
				return NULL;
			}
		}

		TGAInfo* tgainfo = new TGAInfo;
    
		tgainfo->width = header[1] * 256 + header[0];
		tgainfo->height = header[3] * 256 + header[2];
    
		if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
		{
			fclose(file);
			delete tgainfo;
			return NULL;
		}
    
		tgainfo->bpp = header[4];
		bytesPerPixel = tgainfo->bpp / 8;
		imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
		tgainfo->data = new unsigned char[imageSize];
    
		if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
		{
			if (tgainfo->data != NULL)
				delete tgainfo->data;
            
			fclose(file);
			delete tgainfo;
			return false;
		}

		fclose(file);

		//save info in image
		if(pixels)
			delete pixels;

		width = tgainfo->width;
		height = tgainfo->height;
		pixels = new Color[width*height];

		//convert to float all pixels
		for(unsigned int y = 0; y < height; ++y)
			for(unsigned int x = 0; x < width; ++x)
			{
				unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
				this->setPixel(Color( tgainfo->data[pos+2] / 255.0f, tgainfo->data[pos+1] / 255.0f, tgainfo->data[pos] / 255.0f),x,height - y - 1);
			}

		delete tgainfo->data;
		delete tgainfo;

		return true;
	}

	// Saves the image to a TGA file
	bool saveTGA(const char* filename)
	{
		unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		FILE *file = fopen(filename, "wb");
		if ( file == NULL )
		{
			fclose(file);
			return false;
		}

		unsigned short header_short[3];
		header_short[0] = width;
		header_short[1] = height;
		unsigned char* header = (unsigned char*)header_short;
		header[4] = 24;
		header[5] = 0;

		//tgainfo->width = header[1] * 256 + header[0];
		//tgainfo->height = header[3] * 256 + header[2];

		fwrite(TGAheader, 1, sizeof(TGAheader), file);
		fwrite(header, 1, 6, file);

		//convert pixels to unsigned char
		unsigned char* bytes = new unsigned char[width*height*3];
		for(unsigned int y = 0; y < height; ++y)
			for(unsigned int x = 0; x < width; ++x)
			{
				Color c = pixels[(height-y-1)*width+x];
				unsigned int pos = (y*width+x)*3;
				bytes[pos+2] = (unsigned char)clamp(c.r * 255,0,255);
				bytes[pos+1] = (unsigned char)clamp(c.g * 255,0,255);
				bytes[pos] = (unsigned char)clamp(c.b * 255,0,255);
			}

		fwrite(bytes, 1, width*height*3, file);
		fclose(file);
		delete [] bytes;
		return true;
	}
};

#endif
