
#include "Color.h"
#include <vector>
#include "Vector.h"
#include "BoundingBox.h"
#include <iostream>
#include "io.h"

class EdgeListElement{

public:

	int y;
	int x;
	
	float z;
	
	Vector normal;
	
	EdgeListElement(int x, int y, float z, Vector n){
	    this->x = x;
	    this->y = y;
	    this->normal = n;
	    this->z = z;
    }
};

class ScanLine{

public:

    EdgeListElement* start;
    EdgeListElement* end;
    
    ScanLine(){
        start = NULL;
        end = NULL;
    }
};

class EdgeList{

	public:
	int y_offset;
	std::vector<ScanLine> lines;
	bool isOK;

	EdgeList(){
		;
	}

	EdgeList(int lines, int y_offset){
		this->y_offset = y_offset;
		this->lines = std::vector<ScanLine>(lines);
	}
	
	EdgeList(Poligon& p, BoundingBox& bounds, int side){

        p.sortEdgesDecresingY();  	
		
		this->y_offset = (int) bounds.minY;
		
		lines = std::vector<ScanLine>((int)bounds.maxY - this->y_offset);
					
	    for(int a = 0; a < p._aristas.size(); ++a){
		        
	        float xs = (*(p._aristas[a]->_inicio))(0);
	        float ys = (*(p._aristas[a]->_inicio))(1);
	     	        
            float xe = (*(p._aristas[a]->_fin))(0);
            float ye = (*(p._aristas[a]->_fin))(1); 
            
            float zs = (*(p._aristas[a]->_inicio))(2);
            float ze = (*(p._aristas[a]->_fin))(2);
            
            float divs = ye - ys;
            
            float deltaz = (ze - zs) / divs;
            float z = zs;
            
            Vector startNormal = p._aristas[a]->_inicio->_normal;
            Vector endNormal = p._aristas[a]->_fin->_normal;
            
            Vector deltaNorm = (endNormal - startNormal) / divs;
	        Vector norm = startNormal;
	                 
            float x = xs;
            float m = (xe - xs) / (ye - ys);
	                        
            for(int y = ys; y < (int)ye; ++y){
                if(lines[y - y_offset].start == NULL){
                    lines[y - y_offset].start = new EdgeListElement((int)x, y + y_offset, z,  norm);
                    
                }
                else{
                	if( (int)x < lines[y -y_offset].start->x){
                		EdgeListElement* aux = lines[y -y_offset].start;
                		lines[y -y_offset].start = new EdgeListElement((int)x, y + y_offset, z,  norm);
            			lines[y -y_offset].end = aux;
            		 }
            		else{

                    	lines[y - y_offset].end = new EdgeListElement((int)x, y + y_offset, z,  norm);
             		}
                 }
                 x += m; 
                 z += deltaz;
                 norm += deltaNorm;
            }    
		}
	}		

};

// Cull
bool cullVisible(Poligon& p, Vector n){
	
	std::cout << "CULLING" << std::endl; 
	
	print(p._normal);
	std::cout << " * ";
	print(n);
	std::cout << " = " << p.normal() * n << std::endl;
	if(p.normal() * n > 0)
		return true;
	
	return false;
}

bool cullVisible(Poligon& p){
	
	Vector n(3,0);
	n(2) = -1;
	if(p.screenNormal() * n > 0)
		return true;
	
	return false;
}

bool insideScreenSpace(BoundingBox& bounds, float min, float max, float near , float far){

	if(bounds.minY > max or bounds.maxY < min or bounds.minX > max or 
				bounds.maxX < min or bounds.minZ > far or bounds.maxZ < near)
		return false;
	
	return true;
}



