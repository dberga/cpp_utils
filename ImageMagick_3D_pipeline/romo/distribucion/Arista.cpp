
#include "Arista.h"
#include "Vertice.h"

Arista::Arista(){
    _inicio = NULL;
    _fin = NULL;
}

Arista::Arista(Vertice* a, Vertice* b){
    _inicio = a;
    _fin = b;
}

Vector Arista::VectorAB(){

	return *_fin - *_inicio;
}

void Arista::sortDecresingY(){

    if((*_fin)(1) < (*_inicio)(1)){
        
        Vertice* aux = _inicio;
        _inicio = _fin;
        _fin = aux;
    }
}

