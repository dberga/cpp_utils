
#include "Vector.h"
#include "Object3d.h"
#include "Camara.h"
#include "Raster.h"

#include <string>
#include <iostream>

#define PI 3.1415


int main(void){
	
	Object3d e("cubo.txt");
	Object3d a("piramide2.txt");
	e.printPoligonos();
	
	Matrix translate = Matrix(4,4,0);
    
    for(int i = 0; i < 4; ++i){
        translate(i,i) = 1;   
    }
    translate(0,3) = 1.5;
    
    a.aplicarMatriz(translate);
	
	Punto c(3,0);
	Vector vPrima(3,0);
	vPrima(1) = 1;
	
	float d = 0.5;
	float h = 0.8;
	float f = 100.0;
	
	Punto lookAt(3,0.5);
	lookAt(0) = 1.5;
	int side = 700;
	
	Color incidente;
	Color ambiental;
	
	Vector dirluz(3);
	
	std::cout << std::endl << "direccion luz Lx Ly Lz >>> ";
	std::cin >> dirluz(0) >> dirluz(1) >> dirluz(2);
	
	std::cout << std::endl << "luz incidente r g b [0, 1] >>> ";
	std::cin >> incidente.r >> incidente.g >> incidente.b;
	
	std::cout << std::endl << "luz ambiental r g b [0, 1] >>> ";
	std::cin >> ambiental.r >> ambiental.g >> ambiental.b;
	

	// Luz(Vector l, Color incidente, Color ambiental);
	Luz luz(dirluz, incidente, ambiental);
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
		Object3d obj3(a);
		obj2.proyectar(cam);
		obj3.proyectar(cam);
		Magick::Geometry geom(side, side);
		Magick::Image res(geom, "black");
		rasterizar(res, obj2, side, cam, luz, zbuffer);
		rasterizar(res, obj3, side, cam, luz, zbuffer);
		res.display();
		vPrima = cam._vectorV;
		std::cout << std::endl << "cx cy cz >>> ";
	}

}
