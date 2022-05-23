#include "vector.hpp"


Vector::Vector(){

	for(int i=0;i<4;++i){

		punto[i]=0.0;
	}

}


Vector::Vector(float x,float y,float z){

	this->x=x;
	this->y=y;
	this->z=z;
	this->w=1;
	
}

Vector::Vector(float x,float y,float z,float w){

	this->x=x;
	this->y=y;
	this->z=z;
	this->w=w;
	
}

float Vector::distancia(){
	
	return sqrt(x*x + y*y + z*z);

}


// normalizamos
void Vector::normalizar(){

	float dist=distancia();

	x=x/dist;
	y=y/dist;
	z=z/dist;

}

// dividimos todo por W
void Vector::cambio_coordenadas(){

	x/=w;
	y/=w;
	z/=w;
	w=1;
}


// operador vector por float
Vector operator * (const Vector& vec,float n) { return Vector(vec.x*n, vec.y*n, vec.z*n); }
void operator *= (Vector& vec,float n) { vec = vec * n; }

// operador producto escalar
float operator * (const Vector& vec,const Vector& vec2) { return vec.x*vec2.x + vec.y*vec2.y + vec.z*vec2.z; }
float operator *= (Vector& vec,Vector& vec2) { return vec * vec2; }

// operado para producto vectorial
Vector operator ^ (const Vector& vec1,const Vector& vec2) { 
	return Vector((vec1.y * vec2.z) - (vec1.z * vec2.y),
				(vec1.z * vec2.x) - (vec1.x *vec2.z),
				(vec1.x * vec2.y) - (vec1.y * vec2.x));
}
void operator ^= (Vector& vec1,Vector& vec2) { vec1 = vec1 ^ vec2; }

// operado para resta de vectores
Vector operator - (const Vector& vec1,const Vector& vec2) { 
	return Vector((vec1.x - vec2.x),(vec1.y - vec2.y),(vec1.z - vec2.z));
}
void operator -= (Vector& vec1,Vector& vec2) { vec1 = vec1 - vec2; }

// operado para suma de vectores
Vector operator + (const Vector& vec1,const Vector& vec2) { 
	return Vector((vec1.x + vec2.x),(vec1.y + vec2.y),(vec1.z + vec2.z));
}
void operator += (Vector& vec1,Vector& vec2) { vec1 = vec1 + vec2; }

// operado para suma de vectores con ints
Vector operator + (const Vector& vec1,int n) { 
	return Vector((vec1.x + n),(vec1.y + n),(vec1.z + n));
}
void operator += (Vector& vec1,int n) { vec1 = vec1 + n; }

// operador para ordenar vector
bool operator < (const Vector& vec1,const Vector& vec2) { 
	return vec1.x<vec2.x;
}




