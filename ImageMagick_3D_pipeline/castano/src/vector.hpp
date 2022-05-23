#ifndef VECTOR_H
#define VECTOR_H

#include <cmath>

class Vector{

public:
	union {
		struct { float x; float y; float z; float w; };
		float punto[4];
	};
	
	Vector();
	Vector(float x,float y,float z);
	Vector(float x,float y,float z,float w);

	Vector& operator = (const Vector& v)
	{

		x=v.x;
		y=v.y;
		z=v.z;
		w=v.w;
		return *this;
	}

	float distancia();
	void normalizar();
	
	void cambio_coordenadas();

};


float operator * (const Vector& vec,const Vector& vec2);
float operator *= (Vector& vec,Vector& vec2);

Vector operator * (const Vector& vec,float n);
void operator *= (Vector& vec,float n);
Vector operator ^ (const Vector& vec1,const Vector& vec2);
void operator ^= (Vector& vec1,Vector& vec2);
Vector operator - (const Vector& vec1,const Vector& vec2);
void operator -= (Vector& vec1,Vector& vec2);
Vector operator + (const Vector& vec1,const Vector& vec2);
void operator += (Vector& vec1,Vector& vec2);
Vector operator + (const Vector& vec1,int n) ;
void operator += (Vector& vec1,int n);
bool operator < (const Vector& vec1,const Vector& vec2);

#endif
