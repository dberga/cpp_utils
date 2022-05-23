
#include <cmath>
#include "Camara.h"
#include <iostream>
#include "io.h"

     
Camara::Camara(float azimut, float elevacion, Punto c, Vector vPrima, float h, float d, float f){
    _f = f;
    _azimut = azimut;
    _elevacion  = elevacion;
    _puntoC = c;
    
    _vectorN = Vector(3,0);
    _vectorU = Vector(3,0);
    _vectorV = Vector(3,0);
    
    _matrizT = Matrix(4,4,0);
	_matrizR = Matrix(4,4,0);
    
    _Tview = Matrix(4,4,0);
    
    _Tpers = Matrix(4,4,0);
    
    calcularVectorN(azimut, elevacion);
    
    calcularVectorV(vPrima, _vectorN);
    
    _vectorU = productoVectorial(_vectorN, _vectorV);
    
    calcularMatrizT(_puntoC);
    
    calcularMatrizR();
    
    _Tview = _matrizR * _matrizT;
    
    calcularTpers1(h,d);
    calcularTpers2(d,f);
    
    _Tpers = _Tpers2 * _Tpers1;
    
 
    
    
}

Camara::Camara(Punto c, Vector n, Vector vPrima, float h, float d, float f){
	_f = f;
	_puntoC = c;
	_vectorN = n;
	
	_vectorN.normalizar();
    
    _vectorU = Vector(3,0);
    _vectorV = Vector(3,0);
    
    _matrizT = Matrix(4,4,0);
	_matrizR = Matrix(4,4,0);
    
    _Tview = Matrix(4,4,0);
    
    _Tpers = Matrix(4,4,0);
    
    calcularVectorV(vPrima, _vectorN);
    
    _vectorV.normalizar();
    
    _vectorU = productoVectorial(_vectorN, _vectorV);
    
    _vectorU.normalizar();
    
    calcularMatrizT(_puntoC);
    
    calcularMatrizR();
    
    _Tview = _matrizR * _matrizT;
    
    calcularTpers1(h,d);
    calcularTpers2(d,f);
    
    _Tpers = _Tpers2 * _Tpers1;
	

}

void Camara::printInfo(){

   std::cout << "Camara : vector N" << std::endl;
    print(_vectorN);
    
    std::cout << "Camara : vector V" << std::endl;
    print(_vectorV);
    
    std::cout << "Camara - vector U" << std::endl;
    print(_vectorU);
    
    std::cout << "Camara - MATRIZ R" << std::endl;
    print(_matrizR);
    
    std::cout << "Camara - MATRIZ T" << std::endl;
    print(_matrizT);
    
    std::cout << "Camara - MATRIZ Tview" << std::endl;
    print(_Tview);
    
    std::cout << "Camara - MATRIZ Tpers1" << std::endl;
    print(_Tpers1);
    
    std::cout << "Camara - MATRIZ Tpers2" << std::endl;
    print(_Tpers2);
    
    std::cout << "Camara - MATRIZ Tpers = Tpers2 * Tpers1 =" << std::endl;
    print(_Tpers);


}

/* h - apotema del plano imagen
   d - distancia del plano imagen (corte cercano)
   f - Far plane - distancia del plano de corte lejano
*/
void Camara:: calcularTpers1(float h, float d){
    
    _Tpers1 = Matrix(4,4,0); 
     
    _Tpers1(0,0) = d/h;
    _Tpers1(1,1) = d/h;
    _Tpers1(2,2) = 1;
    _Tpers1(3,3) = 1; 

}

void Camara:: calcularTpers2(float d, float f){
    
    _Tpers2 = Matrix(4,4,0);

    
    _Tpers2(0,0) = _Tpers2(1,1) = _Tpers2(3,1) = _Tpers2(3,2) = 1;
    _Tpers2(2,2) = f / (f - d);
    _Tpers2(2,3) = -(d*f) / (f - d);
        

}


void Camara:: calcularMatrizT(Punto c){

   for(int i = 0; i < 4; ++i)
        _matrizT(i,i) = 1;

   for(int i = 0; i < 3; ++i)
        _matrizT(i,3) = -c[i];		
}

void Camara:: calcularMatrizR(){
    
    for(int i = 0; i < 3; ++i)
        _matrizR(0,i) = _vectorU(i);
    
    for(int i = 0; i < 3; ++i)
        _matrizR(1,i) = _vectorV(i);
    
    for(int i = 0; i < 3; ++i)
        _matrizR(2,i) = _vectorN(i);
    
    _matrizR(3,3) = 1;
    
    
}

void Camara:: calcularVectorN(float  azimut, float  elevacion){            
     _vectorN(0) = float(sin(elevacion)) * float(cos(azimut));
     _vectorN(1) = float(sin(elevacion)) * float(sin(azimut));
     _vectorN(2) = float(cos(elevacion));   
}

void Camara:: calcularVectorV(Vector vPrima, Vector vectorN){
	
	Vector aux = vectorN;
	float f = vPrima * vectorN;
	aux *= f;
    _vectorV = vPrima - aux;
} 


