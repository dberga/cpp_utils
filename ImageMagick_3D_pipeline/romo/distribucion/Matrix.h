
#ifndef MATRIX
#define MATRIX

#include <cstdlib>
#include "Vector.h"

class Matrix{
    
    private:

        int _rows;
        int _cols;
        float* _values;    
    
        void init(int rows, int cols);
              
    public:
        
        Matrix();
              
        Matrix(int rows, int cols);

        Matrix(int rows, int cols, float startValue);
        
        // Falla el destructor
        //~Matrix();
        
		inline int rows() const{
			return _rows;
		}

		inline int cols() const{
			return _cols;
		}

		inline float & operator()(int i, int j)
		{
			return _values[i * _cols + j];
		};

		inline Matrix transpose(){
		
			Matrix res(_cols, _rows);
			for(int i = 0; i < _rows; ++i)
				for(int j = 0; j < _cols; ++j)
				    res(j,i) = _values[i * _cols + j];
			return res;
		}
        
        Matrix& operator*=(float scalar);
        
        const Matrix operator*(const Matrix& b);    
        
        const Vector operator*(Vector& b);     
};

void transformar(Vector& v, Matrix& m);

#endif
