
#include "Vector.h"
#include "Object3d.h"
#include "Camara.h"
#include "Raster.h"

#include <string>
#include <iostream>

#define PI 3.1415


int main(int argc, char** argv){

	if(argc != 2){
		std::cerr << "Uso: " << argv[0] << " entrada.txt" << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Object3d e(argv[1]);
	
	e.printPoligonos();
	Punto c(3,0);
	Vector vPrima(3,0);
	
	// c(0) = 1;
	
	//c(0) = 3;
	//c(1) = -2;
	//c(2) = 3;
	
	c(1) = -2;
	c(0) = -5;
	c(2) = -5;
	
	Vector n = Punto(3,0.5) - c;
	
	vPrima(2) = 1;
	
	Camara cam(c, n, vPrima, 0.8, 0.7, 20.0);
	
	cam.printInfo();
	
	Object3d obj2(e);
	
	obj2.proyectar(cam);

	e.proyectar(cam);
	int side = 500;
		// Luz(Vector l, Color incidente, Color ambiental);
	Luz luz(Vector(3,0.5), Color(0.6, 0.6, 0.6), Color(0.2,0.2,0.2));
	
	float** zbuffer = (float**) malloc(sizeof(float*) * side);
		for(int i = 0; i < side; ++i){
	zbuffer[i] = (float*) malloc(sizeof(float) * side);
	for(int j = 0; j < side; ++j)
		zbuffer[i][j] = cam._f;
		}
	
	Magick::Geometry geom(side, side);
	Magick::Image res(geom, "black");
	rasterizar(res, e, side, cam, luz, zbuffer);
	
	res.display();
	
	//char a;
	//std::cin >> a;
	//res.display();
	
	res.write("test.bmp");
	
	std::cout << "MAXRGB = " << MAXRGB << std::endl;
	

}

