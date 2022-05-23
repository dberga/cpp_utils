#ifndef VERTICE_H
#define VERTICE_H

#include "vector.hpp"
#include "image.hpp"

class Vertice{

public:

	float id,is,ia, I;
	Vector normal;	

	Vertice();
	void calculaIntensidadEnVertice(const Vector &h,const Vector &l,float ks,float kd, float ka);
};



#endif
