#ifndef POLIGON
#define POLIGON
#include <list>
#include <vector>
#include "Vector.h"
#include "Color.h"

class Arista;

class Vertice;

class Poligon{
    
    public:
    
    std::vector<Vertice*> _vertices;
	std::vector<Arista*> _aristas;
	std::vector<int> _indiceVertices;
	
	// Color _color;

	Vector _normal;
	Vector _screenNormal;
    
    Poligon();
    
    ~Poligon();

    Poligon(int nVertices);
    
    Poligon(int nVertices, Color col);
		
	int computeAristas();
	
	int sortEdgesDecresingY();
	
	void computeNormal(); 
	
	Vector normal();
	
	void computeScreenNormal(); 
	
	Vector screenNormal();
	
	void setColor(Color col);
        	
};


#endif


