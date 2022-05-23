
#include "Raster.h"
#include "Color.h"
#include "Arista.h"
#include "io.h"
#include <iostream>
#include <vector>

#include "EdgeList.h"

void rasterizar(Magick::Image& res, Object3d& screenSpace, int side, Camara& cam, Luz luz, float** zbuffer){
    
    // transformamos la escena a las dimensiones de pantalla
    Matrix translate = Matrix(4,4,0);
    Matrix scale = Matrix(4,4,0);
    for(int i = 0; i < 4; ++i){
        scale(i,i) = side/2;
        translate(i,i) = 1;   
    }
    scale(2,2) = 1; // asi no escalamos la componente z
    
    // solo trasladamos las componentes x, y 
    translate(0,3) = 1;
    translate(1,3) = 1;
    

    
	screenSpace.aplicarMatriz(translate);
	screenSpace.aplicarMatriz(scale);
	
 
   	

	std::vector<EdgeList*> listas(screenSpace._poligonos.size());

	// Hacemos culling i cliping i construimos una edge list para cada poligono visible
	for(int i = 0; i < screenSpace._poligonos.size(); ++i){
	    
	    if(cullVisible(screenSpace._poligonos[i])){
	    	BoundingBox bounds = BoundingBox(screenSpace._poligonos[i]);
	    	if(insideScreenSpace(bounds, 0, side - 1, 0, cam._f)){

	    		listas[i] = new EdgeList(screenSpace._poligonos[i], bounds, side);
			}
			else
				listas[i] = NULL;
		}
		else
			listas[i] = NULL;
	}

	// Vector de linea de vista es constante
	Vector v = cam._vectorN;
	v *= -1;

	for(int i = 0; i < listas.size(); ++i){
		
		if(listas[i]){
	
			for(int j = 0; j < listas[i]->lines.size(); ++j){
			
		    	if(j + listas[i]->y_offset >= 0 and j + listas[i]->y_offset < side){ 
		    	// para poder rasterizar poligonos parcialmente fuera del frustum
		    	
					int startx = listas[i]->lines[j].start->x;
					int lastx = listas[i]->lines[j].end->x;
					
					// variables for z interpolation
					float startz = listas[i]->lines[j].start->z;
					float endz = listas[i]->lines[j].end->z;        	
					float deltaz = (endz - startz) / (lastx - startx); 
					float z = startz;
					
					Vector startNormal = listas[i]->lines[j].start->normal;
			    	Vector endNormal = listas[i]->lines[j].end->normal;
					
					Vector deltaNorm = (endNormal - startNormal) / (lastx - startx);
					Vector norm = startNormal;
					
					for(int k = startx; k <= lastx; ++k){
						if(zbuffer[k][j + listas[i]->y_offset] > z and k >= 0 and k < side and z > 0){
						
							Color intensidad = screenSpace._material->shade(norm, luz._l, v, 
													luz._ambiental, luz._incidente);
							Magick::Quantum red = intensidad.r * MaxRGB;
	   
							Magick::Quantum green = intensidad.g * MaxRGB; 
			
							Magick::Quantum blue = intensidad.b * MaxRGB;
							Magick::Color c(red, green, blue);
							res.pixelColor(k, j + listas[i]->y_offset, c);
							zbuffer[k][j + listas[i]->y_offset] = z;
						}
						z += deltaz;
						norm += deltaNorm;
					}
				}
	    	}
	    }
	    
    }
    //return res;
}

