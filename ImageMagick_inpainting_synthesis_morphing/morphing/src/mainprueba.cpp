#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include <Magick++.h>
#include <iostream> 
using namespace std; 
using namespace Magick; 
#include <Imagen.h>


void uso()
{
  fprintf(stderr,"Uso: prueba imagen_entrada imagen_salida lista.txt dt T \n");
  exit(1);
};



main(int argc,char **argv)
{
 
  //verifica que la cantidad de argumentos es correcta 
  if(argc<6)
    uso();


  argv++;
  char * entrada_char=*argv++;
  char * salida_char=*argv++;
  char * lista_char=*argv++;
  float dt=atof(*argv++);
  int T=atoi(*argv++);
 
  
  //lee la lista de coordenadas de los segmentos
  int n_segmentos=0;
  FILE	*fp;
  fp= fopen(lista_char,"r");   
  fscanf(fp, "\n");
  fscanf(fp, "%d\n", &n_segmentos);
  int * x0=new int[n_segmentos];
  int * y0=new int[n_segmentos];
  int * x1=new int[n_segmentos];
  int * y1=new int[n_segmentos];
  for(int i=0;i<n_segmentos;i++)
    fscanf(fp,"%d %d %d %d\n",&x0[i],&y0[i],&x1[i],&y1[i]);
  fclose(fp);
  

  //escribe en pantalla las coordenadas leidas
  for(int i=0;i<n_segmentos;i++)
    fprintf(stderr,"%d %d %d %d\n",x0[i],y0[i],x1[i],y1[i]);

 
  //lee el campo R (rojo) de la imagen de entrada
  Imagen u=lee(entrada_char,0);
  //los otros campos los leemos asi:
  Imagen uG=lee(entrada_char,1);
  Imagen uB=lee(entrada_char,2);

  //calcula las dimensiones de la imagen
  int nfil=u.fils();
  int ncol=u.cols();

  //ejemplo: lee el valor del pixel en las coordenadas (fila,columna)=(12,23)
  int fila= 12;
  int columna=23;
  float val=u(fila,columna); //originalmente es un valor entre 0.0 y 255.0

  //ejemplo: modifica el valor del pixel en las coordenadas (fila,columna)=(12,23)
  float nuevo_val=128.0;
  u(fila,columna)=nuevo_val;


    
  //la imagen es modificada con una funcion miembro
  //y se devuelve una nueva imagen
  Imagen & tmp=u.desplaza(0,T);
  escribe("desplazada.tif",tmp);//se escribe una imagen en grises

  //ejemplos de operaciones con imagenes
  tmp*=dt;//multiplica por un flotante
  u+=tmp;//suma otra imagen


  //modificacion con una funcion miembro, pero ahora NO se
  //crea una nueva imagen
  u.recorta(255.0,0.0);
  //es conveniente recortar entre 0.0 y 255.0 inmediatamente antes de grabar
  //el resultado


  //se escribe la imagen de salida, concatenando el nombre
  //proporcionado por el usuario, seguido del numero T, y
  //la extension tif (la extension podria ser gif, jpg, bmp, etc.)
  char indice[5];
  char salida_completa[256];
  sprintf(indice,"%d",T);
  strcpy(salida_completa,salida_char);
  strcat(salida_completa,indice);
  strcat(salida_completa,".tif");
  escribe(salida_completa,u,uG,uB);//se escribe una imagen en colores
  tmp.~Imagen();
  return 0;
  
}
