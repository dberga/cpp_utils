
#include "Vertice.h"
#include <cstdlib>
#include <cmath>
#include <iostream>

Vector::Vector(){
    _size = 0;
    _values = NULL;
}

Vector::Vector(const Vector& v){
	
	_size = v._size;
	_values = new float[_size];
	
    for(int i =0; i < _size; ++i)
    	_values[i] = v._values[i];
}

Vector::Vector(int size){
    //_parents = list<Poligon*>();
    _size = size;
    _values = new float[size];
    
}

Vector::~Vector(){
	if(_values != NULL)
		delete _values;
}

Vector::Vector(int size, float startValue){

    //_parents = list<Poligon*>();
    _size = size;
    _values = new float[size];
    for(int i =0; i < _size; ++i)
        _values[i] = startValue;
}

void Vector::resize(int newsize){
    _values = (float*) realloc(_values, newsize);
    _size = newsize;
}

void Vector::normalizar(){
	
	float modulo = 0;
	for(int i = 0; i < _size; ++i)
		modulo += (_values[i] * _values[i]);
	
	modulo = sqrt(modulo);
	
	for(int i = 0; i < _size; ++i)
		_values[i] /= modulo;

}


Vector& Vector::operator=(const Vector& b){
	if(this == &b)
		return *this;
		
	delete _values;
	_size = b._size;
	_values = new float[_size];
	for(int i = 0; i < _size; ++i)
		_values[i] = b._values[i];
	
	return *this;
}

Vector& Vector::operator-=(const Vector& b)
{
	if(_size == b._size){
		for(int i=0;i< _size; i++)
			_values[i] -= b._values[i];
	}
	return *this;
}

Vector& Vector::operator+=(const Vector& b)
{
	if(_size == b._size){
		for(int i=0;i< _size; i++)
			_values[i] += b._values[i];
   }
   return *this;
}

Vector& Vector::operator*=(float scalar)
{
	for(int i=0;i< _size; i++)
		_values[i] *= scalar;
	return *this;
}

Vector& Vector::operator/=(float scalar)
{
	for(int i=0;i< _size; i++)
		_values[i] /= scalar;
	return *this;
}

Vector Vector::operator-(const Vector& b){
	if(_size == b._size){
		Vector res = *(this);
		res-= b; 
		return res;
	}
	return *this;	
}

Vector Vector::operator+(const Vector& b){
	if(_size == b._size){
		Vector res = *(this);
		res+= b; 
		return res;
	}
	return *this;	
}

Vector Vector::operator*(float scalar){
	
	Vector res = *(this);
	res *= scalar;
	return res; 
    
}

Vector Vector::operator/(float scalar){
	
	Vector res = *(this);
	res /= scalar;
	return res; 
    
}

// producto escalar entre Vectors
float Vector::operator*(const Vector& b) 
{
    if(_size != b._size){
        return 0; // lanzar excepcion !
        std::cout << "Excepcion de dot product" << std::endl;
    }
    
    float res = 0;
    for(int i = 0 ; i < _size; ++i)
        res += _values[i] * b._values[i];
    
    return res;
}

Vector productoVectorial(Vector& a, Vector& b){
    
    Vector vertice(3);
    vertice(0) = a(1)*b(2) - a(2)*b(1);
    vertice(1) = a(2)*b(0) - a(0)*b(2);
    vertice(2) = a(0)*b(1) - a(1)*b(0);
    
    return vertice;
} 

