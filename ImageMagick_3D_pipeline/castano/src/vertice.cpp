#include "vertice.hpp"


Vertice::Vertice(){

}

void Vertice::calculaIntensidadEnVertice(const Vector &h,const Vector &l,float ks,float kd ,float ka){
	// calculamos la intensiadada en el vertice con la formula del liro
	// el parametro n lo ponemos a 1
	id=l*normal*kd;
	is=pow(normal*h,1)*ks;
	id = clamp(id,0,1);
	is = clamp(is,0,1);
	ia = 1 * ka; // suponekos una intensidad de 1
	I = ia + is + id; // calculo de la intensidad total
}


