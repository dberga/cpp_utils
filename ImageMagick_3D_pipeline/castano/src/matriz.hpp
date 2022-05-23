#ifndef MATRIZ_H
#define MATRIZ_H

#include <iostream>
#include "vector.hpp"
#include <cstring>
#include <stdio.h>

class Matriz{

public:
	
	float matriz[4][4];
	
	Matriz();
	Matriz(float m[4][4]);

	void imprime();
};

Matriz operator * (const Matriz &m,float n);
void operator *= (Matriz &m,float n);
Matriz operator * (const Matriz &m1, const Matriz &m2);
void operator *= (Matriz &m1, Matriz &m2);

Vector operator * (const Matriz &m1, const Vector &vec);
void operator *= (Matriz &m1, Vector &vec);


#endif
