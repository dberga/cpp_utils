#include "matriz.hpp"


Matriz::Matriz(){
	memset(matriz,0.0,4*4*sizeof(float));
}


Matriz::Matriz(float m[4][4]){

	memcpy(matriz,m,4*4*sizeof(float));
}
// funcion para imprimir
void Matriz::imprime(){

	for(int i=0;i<4;i++) {
		for(int j=0;j<4;j++){
			printf("%f    ",matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
}


// operador para multiplicar matriz con float
Matriz operator * (const Matriz &m,float n) { 

	Matriz m2;
	for(int i=0;i<4;++i){
		for(int j=0;j<4;++j){
			m2.matriz[i][j]=m.matriz[i][j]*n;		
		}
	}
	return m2;
}
void operator *= (Matriz &m,float n) { m =m * n; }


// operador para multiplicar matriz con matriz
Matriz operator * (const Matriz &m1, const Matriz &m2) { 

	Matriz res;
	for(int i=0;i<4;++i)
	{
		for(int j=0;j<4;++j)
		{
			for(int k=0;k<4;++k){				
				res.matriz[i][j]+=m1.matriz[i][k]*m2.matriz[k][j];
			}			
		}
	}	
	
	return res;
}

void operator *= (Matriz &m1, Matriz &m2) { m1 =m1 * m2; }

// operador para multiplicar matriz con vector
Vector operator * (const Matriz &m1, const Vector &vec){

	Vector aux;

	aux.x= vec.x * m1.matriz[0][0] + vec.y * m1.matriz[0][1] + vec.z * m1.matriz[0][2] + 1 * m1.matriz[0][3];
	aux.y= vec.x * m1.matriz[1][0] + vec.y * m1.matriz[1][1] + vec.z * m1.matriz[1][2] + 1 * m1.matriz[1][3];
	aux.z= vec.x * m1.matriz[2][0] + vec.y * m1.matriz[2][1] + vec.z * m1.matriz[2][2] + 1 * m1.matriz[2][3];
	aux.w= vec.x * m1.matriz[3][0] + vec.y * m1.matriz[3][1] + vec.z * m1.matriz[3][2] + 1 * m1.matriz[3][3];	

	return aux;

}

void operator *= (Matriz &m1, Vector &vec) {vec=m1*vec;}


