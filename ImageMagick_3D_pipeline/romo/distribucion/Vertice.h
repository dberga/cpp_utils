
#ifndef VERTICE
#define VERTICE

#include <list>
#include "Vector.h"
#include "Poligon.h"

class Vertice : public Punto{

    private:
        std::list<Poligon*>::iterator _iter;
        
        
    public:
    
    	Vector _normal;
    
        std::list<Poligon*> _poligons;
        
        Vertice();
        
        Vertice(int size);
        
        Vertice(const Vertice& v);
        
        void addPoligon(Poligon* parent);
        
        void computarNormal();
        
        Poligon* firstPoligon();
        
        Poligon* nextPoligon(); 
        
};
    
#endif

