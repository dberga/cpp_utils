
#include "Vector.h"
#include "Object3d.h"
#include "Camara.h"
#include "Raster.h"

#include <string>
#include <iostream>

#define PI 3.1415


int main(int argc, char** argv){

	if(argc != 2){
		std::cerr << "Uso: " << argv[0] << " objeto1.txt objeto2.txt ..." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	Object3d e(argv[1]);
	
	e.printPoligonos();
	
	Punto c(3,0);
	Vector vPrima(3,0);
	vPrima(1) = 1;
	
	float d = 0.4;
	float h = 0.5;
	float f = 100.0;
	
	Punto lookAt(3,0.5);
	int side = 700;
	
	Color incidente;
	Color ambiental;
	
	std::cout << std::endl << "luz incidente r g b [0, 1] >>> ";
	std::cin >> incidente.r >> incidente.g >> incidente.b;
	
	std::cout << std::endl << "luz ambiental r g b [0, 1] >>> ";
	std::cin >> ambiental.r >> ambiental.g >> ambiental.b;
	// Luz(Vector l, Color incidente, Color ambiental);
	Luz luz(Vector(3, 0.5), incidente, ambiental);
	std::cout << std::endl << "cx cy cz >>> ";
	while(std::cin >> c(0) >> c(1) >> c(2)){
	
	
		
		Vector n = lookAt - c;
		Camara cam = Camara(c, n, vPrima, h, d, f);
		cam.printInfo();
		
	    // creamos el zbuffer y lo inicializamos a f (far plane)
		float** zbuffer = (float**) malloc(sizeof(float*) * side);
		for(int i = 0; i < side; ++i){
			zbuffer[i] = (float*) malloc(sizeof(float) * side);
	 		for(int j = 0; j < side; ++j)
	 			zbuffer[i][j] = cam._f;
		}
		
		Object3d obj2(e);
		obj2.proyectar(cam);
		Magick::Geometry geom(side, side);
		Magick::Image res(geom, "black");
		rasterizar(res, obj2, side, cam, luz, zbuffer);
		res.display();
		vPrima = cam._vectorV;
		std::cout << std::endl << "cx cy cz >>> ";
	}

}

