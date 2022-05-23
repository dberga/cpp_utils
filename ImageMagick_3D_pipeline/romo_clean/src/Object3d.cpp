
#include <iostream>
#include <fstream>




#include <Object3d.h>
#include <io.h>
#include <Matrix.h>

Object3d::Object3d(char* fileName)
{

// FALTA SABER SI LA NUMERACION DE LOS VERTICES VA DE 0 A N O DE 1 A N+1
    std::ifstream f(fileName);
    int nVertices;
    int nPoligonos;

    f >> nVertices;
    _vertices = std::vector<Vertice>(nVertices);


    f >> nPoligonos;
    _poligonos = std::vector<Poligon>(nPoligonos);
	
    
    for (int i = 0; i < nVertices; i++){
        _vertices[i] = Vertice(4);
        f >> _vertices[i](0);
        f >> _vertices[i](1);
        f >> _vertices[i](2);
        _vertices[i](3) = 1;
    }
    for(int i = 0; i < nVertices; ++i)
    	print(_vertices[i]);

    for (int j = 0; j < nPoligonos; j++){
    
        int numVertices;
        f >> numVertices;
        _poligonos[j] = Poligon(numVertices);
        
        for(int k = 0; k < numVertices; ++k){
            int idVertice;
            f >> idVertice;
            _vertices[idVertice].addPoligon(&_poligonos[j]);
            _poligonos[j]._vertices[k] = &_vertices[idVertice];
            _poligonos[j]._indiceVertices[k] = idVertice;
        }
        _poligonos[j].computeAristas();
        _poligonos[j].computeNormal();
        
    }
    
    for (int i = 0; i < nVertices; i++)
        _vertices[i].computarNormal();
    
    
    // Leemos el material
    Color ambiental;
    Color difuso;
    Color specular;
    
    ambiental.r=0.5;
    ambiental.g=0.5;
    ambiental.b=0.5;
    
    difuso.r=0.5;
    difuso.g=0.5;
    difuso.b=0.5;
    
    specular.r=0.5;
    specular.g=0.5;
    specular.b=0.5;
    
    _material = new Material(ambiental, difuso, specular);
    
    f.close();
}

Object3d::Object3d(const Object3d& orig){
	
	_vertices = std::vector<Vertice>(orig._vertices.size());
	_poligonos = std::vector<Poligon>(orig._poligonos.size());
	
	for(int v = 0; v < _vertices.size(); ++v)
		_vertices[v] = Vertice(orig._vertices[v]);
	
	for(int p = 0; p < _poligonos.size(); ++p){
		
		_poligonos[p] = Poligon(orig._poligonos[p]._vertices.size());
		_poligonos[p]._indiceVertices = orig._poligonos[p]._indiceVertices;
		
		for(int v = 0; v < _poligonos[p]._indiceVertices.size(); ++v){
			_vertices[_poligonos[p]._indiceVertices[v]].addPoligon(&_poligonos[p]);
			_poligonos[p]._vertices[v] = &_vertices[_poligonos[p]._indiceVertices[v]];	
		}
		_poligonos[p].computeAristas();
		_poligonos[p].computeNormal();
	} 
	
	for (int i = 0; i < _vertices.size(); i++)
        _vertices[i].computarNormal();
    
    _material = new Material(*(orig._material));

}

void Object3d::printPoligonos(){
    
    for(int i = 0; i < _poligonos.size(); ++i)
        print(_poligonos[i]);   
    
}

void Object3d::printVertices(){

    for(int i=0; i < _vertices.size(); ++i){
        std::cout << _vertices[i](0) << _vertices[i](1) << _vertices[i](2) 
                                                    << _vertices[i](3) << std::endl;
    }
}

void Object3d::proyectar(Camara& c){
    
    Matrix all = c._Tpers * c._Tview;
    print(all);

    for(int i = 0; i < _vertices.size(); ++i){
		_vertices[i](3) = 1;
	    transformar(_vertices[i], all);
    }
        
}

void Object3d::aplicarMatriz(Matrix& m){
    for(int i = 0; i < _vertices.size(); ++i){
    	_vertices[i](3) = 1;
        transformar(_vertices[i], m);
    }
}
/*
void Object3d::print(Poligon& p){
    for(int i =0; i < p._vertices.size(); ++i){
            std::cout << p._vertices[i](0) << p._vertices[i](1) << p._vertices[i](2) 
                                                    << p._vertices[i](3) << std::endl;
    }
        
} 
*/ 

