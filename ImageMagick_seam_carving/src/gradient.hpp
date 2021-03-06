#ifndef GRADIENT_H
#define GRADIENT_H


#include "seam.hpp"

class Gradient
{

public:

	float* energyArray;
	unsigned int width,height;

	/* Constructors */
	Gradient():	
				width(0),
				height(0)
	{
	}
	
	Gradient(unsigned int width, unsigned int height) : width(width), height(height){

		energyArray = new float[width * height];
		memset(energyArray, 0, width * height * sizeof(float));
	}

	~Gradient(){

		if(energyArray) delete energyArray;

	}

	void resize(const unsigned int width, const unsigned int height)
	{

		float* new_energyArray = new float[width*height];

		for(unsigned int x = 0; x < width; ++x)
			for(unsigned int y = 0; y < height; ++y)
				new_energyArray[ y * width + x ] = getEnergy(x,y);

		delete energyArray;
		this->width = width;
		this->height = height;
		energyArray = new_energyArray;
	}

	void setSize(unsigned int x, unsigned int y){

		width = x; 
		height = y;
	}

	void setEnergy(float e, unsigned int x, unsigned int y){

		energyArray[ y * width + x ] = e;
	}

	float getEnergy(unsigned int x, unsigned int y){

		return energyArray[ y * width + x ];
	}

	// recorremos toda la primera fila y obtenemos un cada Seam, 
	// nos quedamos con el seam de energia minima
	void getMinSeamVertical(Seam * s_min, Seam * s_aux){

		Pixel* paux;
		s_min->setEnergy(FLT_MAX);
		for(unsigned int x = 0; x < width; ++x){
			getSeamVertical( x , s_aux);
			if(s_aux->getEnergy() < s_min->getEnergy()){
				paux = s_min->pixels;
				s_min->pixels = s_aux->pixels;
				s_aux->pixels = paux;
				s_min->energy = s_aux->energy; 
			}
		}

	}

	// recorremos toda la primera columna y obtenemos un cada Seam, 
	// nos quedamos con el seam de energia minima
	void getMinSeamHorizontal(Seam * s_min, Seam * s_aux){

		Pixel* paux;
		s_min->setEnergy(FLT_MAX);
		for(unsigned int y = 0;y < height; ++y){
			getSeamHortizontal( y ,s_aux);
			if(s_aux->getEnergy() < s_min->getEnergy()){
				paux = s_min->pixels;
				s_min->pixels = s_aux->pixels;
				s_aux->pixels = paux;
				s_min->energy = s_aux->energy; 
			}
		}

	}

	// obtenemos un seam empezando en la posicion 0,x
	void  getSeamVertical(unsigned int x,  Seam * s_aux) 
	{

		s_aux->energy = 0.0;
		for (unsigned int y = 0; y < height-1; ++y)
		{
			s_aux->setEnergy(s_aux->getEnergy() + getEnergy(x,y));
			s_aux->setNewPixel(x,y);
			x += getMinValueTopDown(x,y);
		}
		s_aux->setEnergy(s_aux->getEnergy() + getEnergy(x,height-1));
		s_aux->setNewPixel(x,height-1);

	}

	// obtenemos un seam empezando en la posicion 0,y
	void  getSeamHortizontal(unsigned int y,  Seam * s_aux) 
	{
		s_aux->energy = 0.0;
		for (unsigned int x = 0; x < width-1; ++x)
		{
			s_aux->setEnergy(s_aux->getEnergy() + getEnergy(x,y));
			s_aux->setNewPixel(x,y);
			y += getMinValueLeftRight(x,y);
		}
		s_aux->setEnergy(s_aux->getEnergy() + getEnergy(width-1,y));
		s_aux->setNewPixel(width-1,y);
		
	}

	// obtenemos el valor de la coordenada X donde la energia
	// es menor en la siguiente fila
	int getMinValueTopDown(unsigned int x, unsigned int y){

		float left = (x == 0)? FLT_MAX : getEnergy(x-1,y+1);
		float min = getEnergy(x,y+1);
		float right = (x == width -1)? FLT_MAX : getEnergy(x+ 1,y+1);
		int index = 0;
    	if (min > left) 
    		{
    			min = left; 
				index = -1;
			}
    	if (min > right) 
    		{ 
    			index = 1;
    		}
    	return index;


	}

	int getMinValueLeftRight(unsigned int x, unsigned int y){

		float up = (y == 0)? FLT_MAX : getEnergy(x+1,y-1);
		float min = getEnergy(x+1,y);
		float down = (y == height-1)? FLT_MAX : getEnergy(x+ 1,y+1);
		int index = 0;
    	if (min > down) 
    		{
    			min = down; 
				index = 1;
			}
    	if (min > up) 
    		{ 
    			index = -1;
    		}
    	return index;

	}


	// funcion para calcular energia suma
	float energyFunction(float gx, float gy){

		return gx + gy;

	}

	void drawMaxGradient(Seam * s){

		for(unsigned int x = 0; x < s->size; ++x){
				setEnergy(1.0, s->getPixel(x).x , s->getPixel(x).y);
		}

	}

	void movePixelRight(int x,int y){
		
		for(unsigned int i=x;i<width-1;++i){
			setEnergy(getEnergy(i+1,y),i,y);
		}
	}

	void movePixelDown(int x,int y){
		
		for(unsigned int j=y;j<height-1;++j){
			setEnergy(getEnergy(x,j+1),x,j);
		}

	}


	void removeSeamHoritzontal(Seam* s){

		for(unsigned int i=0;i<s->size;++i){
			movePixelDown(s->getPixel(i).x,s->getPixel(i).y);
		}
		resize(width,height-1);
	}


	void removeSeamVertical(Seam* s){

		for(int j=0;j<s->size;++j){
			movePixelRight(s->getPixel(j).x,s->getPixel(j).y);
		}
		resize(width-1,height);
	}
};


#endif
