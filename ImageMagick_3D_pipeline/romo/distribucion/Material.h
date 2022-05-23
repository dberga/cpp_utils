
#ifndef SHADING
#define SHADING

#include "Color.h"
#include "Vector.h"
#include <iostream>
#include "io.h"

class Material{

	public:
	
	Color ambiental;
	Color difuse;
	Color specular;
	
	Material(const Material& orig){
		
		this->ambiental = orig.ambiental;
		this->difuse = orig.difuse;
		this->specular = orig.specular;
		
	}
	
	Material(Color ambiental, Color difuse, Color specular){
		
		this->ambiental = ambiental;
		this->difuse = difuse;
		this->specular = specular;
	}
	
	inline float max(float a, float b){
		return (a>b)? a:b;
	}
	inline float saturate(float f){

		//return (f + 1) / 2;
		return max(0.0, f);
	}

	
	Color shade(Vector& normal, Vector& l, Vector& v, Color Iambiental, Color Iincidente){
		
		float r, g, b;
		normal.normalizar();
		l.normalizar();
		v.normalizar();
		

		
		Vector h = (l + v)/2;
		
		r = Iambiental.r * ambiental.r + Iincidente.r * (saturate(l * normal) * difuse.r + 
														saturate(normal*h) * specular.r); 
		g = Iambiental.g * ambiental.g + Iincidente.g * (saturate(l * normal) * difuse.g + 
														saturate(normal*h) * specular.g);
		b = Iambiental.b * ambiental.b + Iincidente.b * (saturate(l * normal) * difuse.b +
														 saturate(normal*h) * specular.b);
		
		if(r > 1.0) r = 1.0;
		if(g > 1.0) g = 1.0;
		if(b > 1.0) b = 1.0;
		
		//std::cout << "shading rgb: " << r << " " << g << " " << b << std::endl;
		return Color(r, g, b);
	}

};

class Luz{
	
	public:
	
	Vector _l; //Vector desde el infinito
	Color _incidente;
	Color _ambiental;
	
	Luz(Vector l, Color incidente, Color ambiental){
		
		_l =l;
		_incidente = incidente;
		_ambiental = ambiental;
	
	}
	
};

	
#endif
