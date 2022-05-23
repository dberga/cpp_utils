
#include "Poligon.h"
#include "Arista.h"
#include "Vector.h"
#include "Vertice.h"

#include <iostream>

#include <algorithm>
#include "io.h"

Poligon::Poligon(){
    
    _vertices = std::vector<Vertice*>(0);
    _aristas = std::vector<Arista*>(0);
    _normal = Vector(0);
    _screenNormal = Vector(0);
}

Poligon::Poligon(int nVertices){
    
    _vertices = std::vector<Vertice*>(nVertices);
    _aristas = std::vector<Arista*>(nVertices);
    _indiceVertices = std::vector<int>(nVertices);
    _normal = Vector(0);
    _screenNormal = Vector(0);
}

/*
Poligon::Poligon(int nVertices, Color col){
    
    _vertices = std::vector<Vertice*>(nVertices);
    _aristas = std::vector<Arista*>(nVertices);
    _normal = Vector(0);
    _screenNormal = Vector(0);
    _color = col;
}
*/

Poligon::~Poligon(){
	
	for(int i = 0; i < _aristas.size(); ++i)
		delete _aristas[i];
}

int Poligon::computeAristas(){


    if(_vertices.size() >= 3){
        
        int last = _vertices.size() - 1;
    	for(int i = 0; i < last; ++i){
    		_aristas[i] = new Arista(_vertices[i], _vertices[i + 1]);
		}
     	
     	_aristas[last] = new Arista(_vertices[last], _vertices[0]);
     	
     	return _aristas.size();
    }	
    return -1;
}

void Poligon::computeNormal(){

    Vector a = _aristas[0]->VectorAB();
    Vector b = _aristas[1]->VectorAB();

    _normal = productoVectorial(a, b);
    _normal.normalizar();

}

Vector Poligon::normal(){
    if(_normal.size() == 0)
        computeNormal();
    return _normal;
}

void Poligon::computeScreenNormal(){

    Vector a = _aristas[0]->VectorAB();
    Vector b = _aristas[1]->VectorAB();

    _screenNormal = productoVectorial(a, b);
    _screenNormal.normalizar();


} 
	
Vector Poligon::screenNormal(){

    if(_screenNormal.size() == 0)
        computeScreenNormal();
    return _screenNormal;


}

bool minY(Arista* a, Arista* b){
    return (*(a->_inicio))(1) < (*(b->_inicio))(1);
}

int Poligon::sortEdgesDecresingY(){
    for(int i = 0; i < _aristas.size(); ++i)
        _aristas[i]->sortDecresingY();
    
    sort(_aristas.begin(), _aristas.end(), minY);
}



