
#ifndef CAMARA
#define CAMARA

#include "Vector.h"
#include "Matrix.h"

class Camara{
    
public:

    float _azimut;
	float _elevacion;       
    
    Punto _puntoC;
    
	Vector _vectorN;
	Vector _vectorU;
	Vector _vectorV;
	
	Matrix _matrizT;
	Matrix _matrizR;
    
    Matrix _Tview;
    
    Matrix _Tpers1;
    Matrix _Tpers2;
    Matrix _Tpers;
    
    float _f;              
    
    Camara(float azimut, float elevacion, Punto c, Vector vPrima, float h, float d, float f);
    
    Camara(Punto c, Vector n, Vector vPrima, float h, float d, float f);
    
    void printInfo();
    

private:

    void calcularMatrizT(Punto c);
    
    void calcularMatrizR();

    void calcularVectorN(float azimut, float  elevacion);

    void calcularVectorV(Vector vPrima, Vector vectorN);
    
    void calcularTpers1(float h, float d);
    
    void calcularTpers2(float d, float f);
        
         
};

#endif
