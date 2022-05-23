
#ifndef COLOR
#define COLOR

const float MAXRGB = 1;

class Color{

	public:

	float r;
	float g;
	float b;
	
	Color(){}
	
	Color(float r, float g, float b){
		this->r = r; this->g = g; this->b = b;
	}
};

#endif
