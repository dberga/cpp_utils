
#ifndef OBJECT3D
#define OBJECT3D

#include <vector>
#include "Vertice.h"
#include "Poligon.h"
#include "Camara.h"
#include "Material.h"

class Object3d{
    
    public:    
        
        std::vector<Vertice> _vertices;
        
        std::vector<Poligon> _poligonos;
        
        std::list<Poligon*> _visibles;
        
        Material* _material;
        
        // lISTA ARISTAS

    
    	Object3d(char* fileName);
    	
    	Object3d(const Object3d& orig);
    	
    	// Object3d(Object3d );
    	
    	void printVertices();
    	
    	// Multiplica los vertices por una matriz 
    	void aplicarMatriz(Matrix& m);
    	
    	// Transformacion a espacio de pantalla
    	void proyectar(Camara& c);
    	
    	//void cullAndClip(); 
    	
    	void printPoligonos();

};

#endif
