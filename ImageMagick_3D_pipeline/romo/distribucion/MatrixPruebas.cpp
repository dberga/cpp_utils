
#include "Matrix.h"
#include "Vector.h"

#include <iostream>
#include <cstdlib>

void print(Vector m){
    for(int i = 0; i < m.size(); ++i)
        std::cout << m(i) << " ";
    std::cout << std::endl;   
}

void print(Matrix m){
    for(int i = 0; i < m.rows(); ++i){
        for(int j = 0; j < m.cols(); ++j)
            std::cout << m(i,j) << " ";
        std::cout << std::endl;
    }
}


int main(int argc, char** argv){

	if(argc != 1){
		std::cerr << "Uso: " << argv[0] << std::endl;
		exit(EXIT_FAILURE);
	}
    
    Vector a, b, c, d;
    a = Vector(3, 1);
    b = Vector(3, 1);
    
    Matrix mat;
    mat = Matrix(3,3,0);
    mat(0,0) = 1;
    mat(1,1) = 1;
    mat(2,2) = 1;

    //a(0) = 0;
    b(1) = 0;
        
    std::cout << "a = ";
    print(a);
    std::cout << "b = ";
    print(b);
    
    std::cout << "M = ";
    print(mat);
    
    c = mat*a;
    std::cout << "M·a = ";
    print(c);
    
    std::cout << "a.transformar(M) = ";
    //print(a.transformar(mat));
    
    
    //d = mat*b; 
    std::cout << "M·b = ";
    print(mat*b);
    
    std::cout << "b.transformar(M) = ";
    //print(b.transformar(mat));
    
}
