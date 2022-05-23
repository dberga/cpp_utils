#ifndef OBJETO_H
#define OBJETO_H

#include "vector.hpp"
#include "poligono.hpp"
#include <vector>
#include "matriz.hpp"
#include <stdio.h>
#include <map>
#include "vertice.hpp"

class Objeto{

public:
	

	

	float ks,ka,kd;
	Color c;

	// poligonos del objeto
	std::vector<Poligono> poligonos;
	// vertices del objeto
	std::vector<Vector> vertices;
	// aristas de objeto
	std::map<std::string,Arista *> aristas;
	// vertices
	std::map<int,Vertice> normales_gouroud;

	Objeto();
	
	void calcularNormalesEnVertices(const Vector &h,const Vector &l);
	void rasterObjeto(Image &img);
	void comprobarVisiblidadCaras(const Vector &n);
	void cambiarCoordenadas(const Matriz& m);
	void normalizarHomogeneas();
	void shading(const Vector &luz, const Vector &h);
};

#endif
