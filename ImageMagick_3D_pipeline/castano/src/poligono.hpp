#ifndef POLIGONO_H
#define POLIGONO_H

#include "vector.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include "image.hpp"
#include <algorithm>
#include "vector.hpp"
#include "arista.hpp"
#include <map>

class Poligono{

public:
	

	bool visible;
	int lados;
	std::vector<int> vertices;
	std::vector<std::string> aristas_id;
	Vector normal;

	float is,id,ia;

	std::map<int,std::vector< Vector * > > aristas_list;

	Poligono();
	Poligono(int l);


	void shadingPoligono(const Vector &h,const Vector &l,float ks,float kd,float ka);
	
	void raster(Image &img,std::vector<Vector> & vertarray, std::map<std::string,Arista *> &aristas);
	void rasterPoligono(Image &img,Color &c);
	void rasterGouraud(Image &img,Color &c,std::map<std::string,Arista *> &aristas);


	bool zbuffer(Image &img, int x,int y,float z);

	void addVertice(int numvert);
	void calculaNormal(const Vector &v1, const Vector &v2);	
	void esVisible(const Vector &n);

};

#endif
