
#include "Poligon.h"

class BoundingBox{
	
	public:
	
	float minX, minY, minZ;
	float maxX, maxY, maxZ;
	
	BoundingBox(Poligon& p){
	
		minX = (*(p._vertices[0]))(0);
		maxX = (*(p._vertices[0]))(0);
	
		minY = (*(p._vertices[0]))(1);
		maxY = (*(p._vertices[0]))(1);
	
		minZ = (*(p._vertices[0]))(2);
		maxZ = (*(p._vertices[0]))(2);

		for(int i = 1; i < p._vertices.size(); ++i){
		
			if((*(p._vertices[i]))(0) < minX)
				minX = (*(p._vertices[i]))(0);
		
			else if((*(p._vertices[i]))(0) > maxX)
				maxX = (*(p._vertices[i]))(0);
			
			if((*(p._vertices[i]))(1) < minY)
				minY = (*(p._vertices[i]))(1);
		
			else if((*(p._vertices[i]))(1) > maxY)
				maxY = (*(p._vertices[i]))(1);
		
			if((*(p._vertices[i]))(2) < minZ)
				minZ = (*(p._vertices[i]))(2);
		
			else if((*(p._vertices[i]))(2) > maxZ)
				maxZ = (*(p._vertices[i]))(2);
		}	
	}
};
