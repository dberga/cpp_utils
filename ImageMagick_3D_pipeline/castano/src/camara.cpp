#include "camara.hpp"


Camara::Camara(){

	for(int i=0;i<4;++i){

		cam.punto[i]=0.0;
	}

}


Camara::Camara(float x,float y,float z){

	this->cam.x=x;
	this->cam.y=y;
	this->cam.z=z;
	this->cam.w=1;
	
}

Camara::Camara(float x,float y,float z,float w){

	this->cam.x=x;
	this->cam.y=y;
	this->cam.z=z;
	this->cam.w=w;
	
}


//Operaciones de camara

void Camara::calcula_N(){	

	n = n - cam;
	n.normalizar();
}

void Camara::calcula_V(){

	v = vprima - (n * (vprima*n) );
	if(v.distancia() == 0)
		printf("Indeterminacion en la camara\n");
	v.normalizar();
}

void Camara::calcula_U(){

	u=n^v;
	u = u*-1; // pasamos el eje a un sistema de mano derecha
	u.normalizar();
}



//Operaciones View
//Operaciones de matrices

Matriz Camara::calcula_R()
{
	
	Matriz r;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if(i==3 && j==3){r.matriz[i][j]=1;}
			else if(j==3||i==3){r.matriz[i][j]=0;}
			else if(i==0){r.matriz[i][j]=u.punto[j];}
			else if(i==1){r.matriz[i][j]=v.punto[j];}
			else if(i==2){r.matriz[i][j]=n.punto[j];}
		}
	}
	return Matriz(r);
}

Matriz Camara::calcula_T()
{
	Matriz t;

	for(int i=0;i<4;i++)
	{
			t.matriz[i][i]=1;
	}
	t.matriz[0][3]=cam.x*(-1);
	t.matriz[1][3]=cam.y*(-1);
	t.matriz[2][3]=cam.z*(-1);


	return t;

}

void Camara::calcula_View(){

	Matriz r,t;
	
	r=calcula_R();
	t=calcula_T();
	view=r*t;	

}


//OPERACIONES DE Tpers
Matriz Camara::calcula_Tpers1()
{
	Matriz m;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if((i==2 && j==2)||(i==3 && j==3)){m.matriz[i][j]=1;}
			else if(i==0 &&j==0){m.matriz[i][j]=d/h;}
			else if(i==1 &&j==1){m.matriz[i][j]=d/h;}
			else {m.matriz[i][j]=0;}
		}
	}
	printf("Tpers1:\n");
	m.imprime();
	return m;
}

Matriz Camara::calcula_Tpers2()
{
	Matriz p;	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<4;j++)
		{
			if((i==0 && j==0)||(i==1 && j==1)||(i==3 && j==2)){p.matriz[i][j]=1;}
			else if(i==2 &&j==2){p.matriz[i][j]=f/(f-d);}
			else if(i==2 &&j==3){p.matriz[i][j]=((-1)*d*f)/(f-d);}
		}
	}

	printf("Tpers2:\n");
	p.imprime();

	return p;
}

void Camara::calcula_Tpers(){
	pers=calcula_Tpers2()*calcula_Tpers1();
}


void Camara::inicializar(){

	calcula_N();
	printf("\nN: ( %f , %f , %f )\n",n.x,n.y,n.z);
	calcula_V();
	printf("\nV: ( %f , %f , %f )\n",v.x,v.y,v.z);
	calcula_U();
	printf("\nU: ( %f , %f , %f )\n",u.x,u.y,u.z);
	
	//calculamos la matriz de perspectiva de la camara
	printf("\n************************************************************************\n\n");

	Matriz t,r;
	r=calcula_R();
	printf("\nMatriz R:\n\n");
	r.imprime();
	
	t=calcula_T();
	printf("\nMatriz T:\n\n");
	t.imprime();
	
	calcula_View();
	printf("\nMatriz View = RT\n\n");
	view.imprime();


	calcula_Tpers();
	pers.imprime();
}


