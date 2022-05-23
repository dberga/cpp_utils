
#include <cstdlib>
#include "Matrix.h"
//#include "Vertice.h"
#include <iostream>
#include "io.h"
    
void Matrix::init(int rows, int cols){
    _rows = rows;
    _cols = cols;
    _values = new float[_rows * _cols];
}
      

Matrix:: Matrix(){
    _rows = _cols = 0;
    _values = NULL;
}
      
Matrix::Matrix(int rows, int cols){
    init(rows, cols);
}

Matrix::Matrix(int rows, int cols, float startValue){            

    init(rows, cols);
    for(int i = 0; i < _rows; ++i)
        for(int j = 0; j < _cols; ++j)
            _values[i * _cols + j] = startValue;    
}
/*
Matrix::~Matrix(){
	if(_values != NULL)
    	delete _values;
}
*/
Matrix& Matrix::operator*=(float scalar)
{
  for(int i=0;i< _rows; i++)
    for(int j = 0; j < _cols; ++j)
        _values[i * _cols + j] *= scalar;
	
	return *this;
}

const Matrix Matrix::operator*(const Matrix& b)
{
    if(_cols != b._rows){
        return Matrix(); // lanzar excepcion !
    }
    int n = _cols;
    Matrix c(_rows, b._cols);
    float cij;
    
    for(int i = 0; i < c.rows(); ++i){
        for(int j = 0; j < c.cols(); ++j){
            cij = 0;
            for(int r = 0; r < n; ++r){
                cij += _values[i * _cols + r] * b._values[r * b._cols + j];
            }
            c(i,j) = cij; 
        }
    }
    
    return c;
    
}

const Vector Matrix::operator*(Vector& b)
{
    
    if(_cols != b.size()){
        return Vector(); // lanzar excepcion !
        std::cout << "Peta" << std::endl;
    }
    int n = _cols;
    Vector c(b.size());
    float cij;
    
    for(int i = 0; i < c.size(); ++i){
        cij = 0;
        for(int r = 0; r < n; ++r){
            cij += _values[i * _cols + r] * b(r);
        }
        c(i) = cij; 
    }
    
    return c;
    
} 

void transformar(Vector& v, Matrix& m){
     
    v = m * v;    
    
}  

/*
void transformar(Vector& v, Matrix& m){
    
    if(m.cols() == v.size()){
        
        int n = m.cols();
        Vector aux = v;
        float vi;
        
        for(int i = 0; i < v.size(); ++i){
            vi = 0;
            for(int r = 0; r < n; ++r){
                vi += m(i,r) * aux(r);
            }
            v(i) = vi; 
        }
        print(v);
    }
    
}   

*/
