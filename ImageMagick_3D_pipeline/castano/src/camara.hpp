#ifndef CAMARA_H
#define CAMARA_H

#include "vector.hpp"
#include "matriz.hpp"
#include <stdio.h>

class Camara{

public:
	
	Vector cam;
	Vector u,v,n,vprima;
	Matriz view;
	Matriz pers;
	float d,h,f;

	Camara();
	Camara(float x,float y,float z);
	Camara(float x,float y,float z,float w);

	Camara& operator = (const Camara& v)
	{

		cam.x=v.cam.x;
		cam.y=v.cam.y;
		cam.z=v.cam.z;
		cam.w=v.cam.w;
	}

	void inicializar();
	void calcula_N();
	void calcula_V();
	void calcula_U();


	Matriz calcula_T();
	Matriz calcula_R();
	void calcula_View();

	Matriz calcula_Tpers1();
	Matriz calcula_Tpers2();
	void calcula_Tpers();

};

#endif
