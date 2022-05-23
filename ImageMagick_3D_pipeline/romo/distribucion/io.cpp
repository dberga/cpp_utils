
#include <iostream>
#include "io.h"


void print(Vector& v){
    for(int i = 0; i < v.size(); ++i)
        std::cout << v(i) << " ";
    std::cout << std::endl;   
}

void print(Matrix& m){
    for(int i = 0; i < m.rows(); ++i){
        for(int j = 0; j < m.cols(); ++j)
            std::cout << m(i,j) << " ";
        std::cout << std::endl;
    }
}

void print(Vertice& v){
    print((Vector&)v);
    std::cout << "normal: ";
    print(v._normal);
    std::cout << "Pertenece a " << v._poligons.size() << " poligonos." 
                                                       << std::endl;
}

void print(Arista& a){
    std::cout << "Arista de ";
    print((Vector&)*(a._inicio));
    std::cout << " a ";
    print((Vector&)*(a._fin));

}

void print(Poligon& p){

    std::cout << "POLIGONO: " << std::endl;
    std::cout << "Normal: ";
    print(p._normal);
    std::cout << "Vertices: " << p._vertices.size() << std::endl;
    for(int i=0; i < p._vertices.size(); ++i)
        print(*(p._vertices[i]));
    
    std::cout << "Aristas: " << p._aristas.size() << std::endl;
    for(int i=0; i < p._vertices.size(); ++i)
        print(*(p._aristas[i]));
    
    std::cout << std::endl;
}
