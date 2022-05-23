
#include "Object3d.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char **argv){

	if(argc != 2){
		std::cerr << "Uso: " << argv[0] << " entrada.txt" << std::endl;
		exit(EXIT_FAILURE);
	}
	Object3d e(argv[1]);
	// e.printVertices();
	e.printPoligonos();
	
	Punto c(4,0);
	Vector vPrima(3,0);
	c(0) = 1;
	vPrima(1) = 1;
	
	Camara cam(0.0, 0.0, c, vPrima, 1.0, 0.2, 5.0);

    std::cout << "Proyectando objeto..." << std::endl;
	e.proyectar(cam);
	std::cout << "Nuevo objeto:" << std::endl;

    
	// e.printVertices();
	e.printPoligonos();	
	   	
}

	
