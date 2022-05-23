
#include "Vertice.h"


Vertice::Vertice() : Vector(){
	;
}

Vertice::Vertice(int size) : Vector(size){
	;
}

Vertice::Vertice(const Vertice& v) : Vector(v){
	;
}

void Vertice::addPoligon(Poligon* parent){
    
    _poligons.push_back(parent);   
}

Poligon* Vertice::firstPoligon(){
    
    if (_poligons.size() > 0)
    {
        _iter = _poligons.begin();
        return (*_iter);
    }
    return NULL;
}

Poligon* Vertice::nextPoligon(){

    _iter++;
    if (_iter != _poligons.end())
    {
        return (*_iter);
    }
    return NULL;
}

void Vertice::computarNormal(){
	_normal = Vector(3,0);
	for(Poligon* p = firstPoligon(); p != NULL; p = nextPoligon())
		_normal += p->normal();
	
	_normal.normalizar();

}
