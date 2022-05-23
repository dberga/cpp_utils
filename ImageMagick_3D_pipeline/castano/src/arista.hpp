#ifndef ARISTA_H
#define ARISTA_H

#include "vector.hpp"
#include "matriz.hpp"
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <map>
#include "vertice.hpp"
#include "image.hpp"

class Arista{

public:
	
	int inicio,final;
	std::string id;
	float distancia;
	std::vector<Vector * > puntos;

	void crear(int inicio,int final);
	std::string crear_id(int i,int f);
	Arista();
	~Arista();
	Arista(int inicio,int final);
	void interpolarIntensidad(std::vector<Vector> & vertices,std::map<int,Vertice> & normales_gouroud);

};

#endif
