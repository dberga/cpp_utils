
#include "Vector.h"
#include "Poligon.h"
#include <iostream>
#include <cstdlib>

void print(Vector m){
    for(int i = 0; i < m.size(); ++i)
        std::cout << m(i) << " ";
    std::cout << std::endl;   
}


int main(int argc, char** argv){

	if(argc != 1){
		std::cerr << "Uso: " << argv[0] << std::endl;
		exit(EXIT_FAILURE);
	}
    Vector a, b, c, d;
    Poligon p;
    
    a = Vector(3, 1);
    b = Vector(3, 1);
    c = Vector(3, -1);
    
    /*
    std::vector<Vector*> verts(3);
    verts[0] = &a;
    verts[1] = &b;
    verts[2] = &c;
    p = Poligon(verts);
    */
	//p._vertices[0] = &a;
    //p._vertices[1] = &c;
    //p._vertices[2] = &b;
    
    //a(0) = 0;
    b(1) = 0;
    c(2) = 0;
    c(1) = 0;
    
    std::cout << "a = ";
    print(a);
    std::cout << "b = ";
    print(b);
    std::cout << "c = ";
    print(c);
    
    //std::cout << "a·b = " << *(p._vertices[0]) * *(p._vertices[2]) << std::endl;
    std::cout << "a·c = " << a*c << std::endl;
    std::cout << "b·c = " << b*c << std::endl;
    std::cout << "a·b = " << a*b << std::endl;
    
    std::cout << "a x b = ";
    print(productoVectorial(a,b));
    
        
    std::cout << "a x c = ";
    print(productoVectorial(a,c));
    
        
    std::cout << "c x b = ";
    print(productoVectorial(c,b));
    
    std::cout << "a - b = ";
    print(a-b);
    
    std::cout << "a·4 = ";
    print(a*4);
    
}
