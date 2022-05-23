#ifndef SEAM_H
#define SEAM_H

#include "image.hpp"

class Pixel
{
public:
	unsigned int x; 
	unsigned int y; 
	Pixel() : x(0) , y(0) { }
	Pixel(unsigned int x, unsigned int y) : x(0), y(0){  }
	void setPixel(unsigned int x, unsigned int y){ this->x = x ; this->y = y; }
};


// Definimos << para outputear pixel
std::ostream& operator<<(std::ostream &strm, const Pixel &a) { return strm << "Pixel(x=" << a.x << "y=" << a.y << ")"; }

class Seam
{
public:
	Pixel* pixels;
	float energy;
	unsigned int size;

	/* CONSTRUCTORS */
	Seam() : energy(0.0),
			pixels(NULL),
			size(0)
	{

	}

	Seam(float e) : energy(e),
					pixels(NULL),
					size(0)
	{
		
	}
	Seam(unsigned int size): energy(0),
							size(size)
	{
		pixels = new Pixel[size];
		memset(pixels, 0, size * sizeof(Pixel));
		for (unsigned int i = 0; i < size; ++i)
		{
			Pixel p;
			pixels[i] = p; 
		}
	}

	//copy constructor
	Seam(const Seam& c) {
		if(pixels) delete pixels;
		pixels = NULL;

		energy = c.energy;
		size = c.size;
		if(c.pixels)
		{
			pixels = new Pixel[size];
			memcpy(pixels, c.pixels, size*sizeof(Pixel));
		}
	}
	//assign operator
	Seam& operator = (const Seam& c)
	{
		if(pixels) delete pixels;
		pixels = NULL;

		energy = c.energy;
		size = c.size;
		if(c.pixels)
		{
			pixels = new Pixel[size];
			memcpy(pixels, c.pixels, size*sizeof(Pixel));
		}
		return *this;
	}


	~Seam()
	{
		if(pixels) delete pixels;
	}


	//get the pixel at position y
	Pixel getPixel(const unsigned int y) const 
	{
		return pixels[ y ];
	}

	
	virtual void  setNewPixel(const unsigned int x,const unsigned int y){}


	void setEnergy(float e)
	{
		this->energy = e;
	}
	float getEnergy()
	{
		return energy;
	}
	void setSize(const unsigned int size)
	{

		if(pixels) delete pixels;
		pixels = NULL;
		this->size = size;
		pixels = new Pixel[size];
		for (unsigned int i = 0; i < size; ++i)
		{
			Pixel p;
			pixels[i] = p; 
		}
	}

	bool isEmpty(){

		return (pixels[size-1].x == 0 && pixels[size-1].y == 0);
	}
	
	//Para mirar si se sale por la izquierda o por la derecha
	int isInsideX(const int i,const int k, const int max){
		//si estas en el borde cogemos el de arriba
		if(pixels[ i ].x == max)
			return pixels[ i ].x-1;

		//si el vecino al sumar k esta fuera de los bordes devolvemos la posición inicial
		if(pixels[ i ].x + k >= max || pixels[ i ].x + k  < 0)
		 	return pixels[ i ].x;

		//sino devolvemos el vecino con un offset k
		return pixels[ i ].x + k ;
	}	
		

	//Para mirar si se sale por arriba o por abajo
	int isInsideY(const int i,const int k, const int max){
		//si estas en el borde cogemos el de arriba
		if(pixels[ i ].y == max)
			return pixels[ i ].y-1;

		//si el vecino al sumar k esta fuera de los bordes devolvemos la posición inicial
		if(pixels[ i ].y + k >= max || pixels[ i ].y + k  < 0)
		 	return pixels[ i ].y;

		//sino devolvemos el vecino con un offset k
		return pixels[ i ].y + k ;
	}

};


class HorizontalSeam : public Seam
{
public:
	HorizontalSeam(unsigned int size) : Seam(size) {}
	HorizontalSeam() : Seam() {}
	//set the pixel at position x with value p
	virtual void setNewPixel(const unsigned int x,const unsigned int y)
	{

		pixels[ x ].setPixel(x,y);
	}
};


class VerticalSeam : public Seam
{
public:
	VerticalSeam(unsigned int size) : Seam(size) {}
	VerticalSeam() : Seam() {}
	//set the pixel at position y with value p
	virtual void setNewPixel(const unsigned int x,const unsigned int y)
	{
		pixels[ y ].setPixel(x,y);
	}	
};




#endif
