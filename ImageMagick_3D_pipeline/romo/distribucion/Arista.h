#ifndef ARISTA
#define ARISTA

#include "Vector.h"

class Vertice;

class Arista{
    
    public:
    
    Vertice* _inicio;
	Vertice* _fin;
	
	Arista();
    
    Arista(Vertice* a, Vertice* b);
	
	Vector VectorAB();
	
	void sortDecresingY();
};
#endif
