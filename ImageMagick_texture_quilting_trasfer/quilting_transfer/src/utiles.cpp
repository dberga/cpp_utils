#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <Imagen.h>


void Imagen::recorta(float M,float m)
{
  // recorta los valores mayores que M y menores que m

  int largo=dim[0]*dim[1];
  for(int i=0;i<largo;i++)
    {
      datos[i]=min(datos[i],M);
      datos[i]=max(datos[i],m);
    }
}

Imagen & Imagen::desplaza(int dim, int paso)
{

  // devuelve una nueva imagen, resultado de desplazar
  // una cantidad _paso_ de pixels en la direccion _dim_

  int fil=this->fils();
  int col=this->cols();
  Imagen * res=new Imagen(fil,col);
  switch(dim)
    {
    case 0:
      for(int i=0;i<fil;i++)
	for(int j=0;j<col;j++)
	  (*res)(i,j)=(*this)(div(i+paso,fil).rem,j);
      break;
    case 1:
      for(int i=0;i<fil;i++)
	for(int j=0;j<col;j++)
	  (*res)(i,j)=(*this)(i,div(j+paso,col).rem);
      break;
    default:
      fprintf(stderr,"\n Error en desplaza \n");
      break;
    }
  return(*res);
}

void imprimeoutput(Imagen & im1){
	//funcion que imprime el valor RGB de una imagen en un archivo output.txt

	FILE * printar;
	printar=fopen("output.txt", "w");

	for(int i=0; i<im1.fils(); i++){
		for(int j=0; j<im1.cols(); j++){
		fprintf(printar, "%f,",im1(i,j));
		}
	fprintf(printar,"\n");
	}
	fclose(printar);
}

