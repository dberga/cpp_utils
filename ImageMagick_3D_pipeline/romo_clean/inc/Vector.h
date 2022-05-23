
#ifndef VECTOR
#define VECTOR

#include <list>
#include <cstdlib>

//#include "Poligon.h"

// class Matrix;

class Vector{
  
    private:
    
        float* _values;
        int _size;
 
    public:
        
        Vector();
        
        Vector(int size);
        
        Vector(const Vector& v);
        
        Vector(int size, float startValue);
        
        ~Vector();
        
        void resize(int newsize);
        
        inline const int size();
        
        void normalizar();
        
        inline float & operator()(int i);
        
        inline float & operator[](int i);
        
        Vector& operator=(const Vector& b);
		
		Vector& operator+=(const Vector& b);
		
		Vector& operator-=(const Vector& b);
		
        Vector& operator*=(float scalar);
        
        Vector& operator/=(float scalar);
		
		Vector operator+(const Vector& b);
		
		Vector operator-(const Vector& b);
		
		Vector operator*(float scalar);
		
		Vector operator/(float scalar);
		
        float operator*(const Vector& b);
        
};

typedef Vector Punto;

Vector productoVectorial(Vector& a, Vector& b);


inline float & Vector::operator()(int i)
{
    return _values[i];
}

inline float & Vector::operator[](int i)
{
    return _values[i];
}

inline const int Vector:: size(){
    return _size;
}

#endif

