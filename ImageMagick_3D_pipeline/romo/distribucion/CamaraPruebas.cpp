
#include "Camara.h"
#include "Vector.h"

int main(void){
    
    Punto c(3,0);
    c(2) = -3;
    Vector vPrima(3,0);
    
    vPrima(1) = 1;
    
    float azimut = 0;
    float elevacion = 0;
    float h = (float)0.5;
    float d = (float) 0.5;
    float f = 10;
    
    Camara cam(azimut, elevacion, c, vPrima, h, d, f);

}
