#ifndef ESCENA_H
#define ESCENA_H

#include "objeto.hpp"
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include "vector.hpp"
#include <fstream>
#include "camara.hpp"
#include "matriz.hpp"


class Escena{

public:
	
	// Todos los objetos de la escena
	typedef std::vector<Objeto> Objetos;
	Objetos objetos;	

	int tamano;
	Camara camara;
	Vector luz;
	Vector h;	

	Escena();
	Escena(const char *fileobject, const char *fileinfo, Image & img);
	
	void leerObjetos(const char *fileobject);
	void leerInfo(const char *fileinfo);	
	void generarCamara();

	void culling();
	void shading();
	void raster(Image & img);

	void convertirCoordenadas();

};

#endif
