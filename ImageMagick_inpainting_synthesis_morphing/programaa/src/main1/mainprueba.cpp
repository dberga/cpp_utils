#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <cmath>
#include <string.h>
#include <string>

#include <Magick++.h>
#include <iostream> 
using namespace std;
using namespace Magick; 
#include <Imagen.h>


		//Esta funcion nos servira para mirar que los parametros de entrada son correctos
		void uso()
		{
  			fprintf(stderr,"Uso: prueba imagen_entrada imagen_mascara lista.txt \n");
  			exit(1);
		};
		//Esta funcion nos servira para indicar que hay que poner iguales dimensiones de mascara e imagen
		void midas()
		{
 			fprintf(stderr,"Las dimensiones de la imagen y la mascara deben ser iguales \n");
 			exit(1);
		};

/**************************************************FUNCION DE SEMEJANZA**********************************************************/
//En esta funcion calculamos la semejanza entre la coordenada un pixel de mascara y la coordenada un pixel cualquiera

	float semejanzadepixeles(int maskposX, int maskposY, int x, int y, Imagen & COLOR, Imagen & MASCARA)
{

float semej;
float colorkernel[3][3];

//en imatge png concreta peta

		//asignamos los vecinos del pixel normal
		colorkernel[0][0]=COLOR(x-1,y-1);  
		colorkernel[0][1]=COLOR(x-1,y);
		colorkernel[0][2]=COLOR(x-1,y+1);
		colorkernel[1][0]=COLOR(x,y-1); 
		colorkernel[1][2]=COLOR(x,y+1);
		colorkernel[2][0]=COLOR(x+1,y-1);
		colorkernel[2][1]=COLOR(x+1,y);
		colorkernel[2][2]=COLOR(x+1,y+1);

		
		//asignamos los vecinos del pixel de mascara
		float v1 = MASCARA(maskposX-1,maskposY-1);
		float v2 = MASCARA(maskposX-1,maskposY);
		float v3 = MASCARA(maskposX-1,maskposY+1);
		float v4 = MASCARA(maskposX,maskposY-1);
		float v6 = MASCARA(maskposX,maskposY+1);
		float v7 = MASCARA(maskposX+1,maskposY-1);
		float v8 = MASCARA(maskposX+1,maskposY);
		float v9 = MASCARA(maskposX+1,maskposY+1);

//miramos que tipo de pixel es (esquina, borde...) y hacemos el cuadrado de las diferencias entre vecinos, y despues sumamos los valores de semejanza
			//ESQUINA SUPERIOR ESQUERRA
			if(v1 == 0 && v2 == 0 && v3 == 0 && v4 == 0 && v6 == 1 && v7 == 0 && v8 == 1 && v9 == 1){
			
			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float dosR = pow(COLOR(maskposX-1,maskposY) - colorkernel[0][1],2);
			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			float quatreR = pow(COLOR(maskposX,maskposY-1)-colorkernel[1][0],2);
			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			semej= (unR + dosR + tresR + quatreR + setR)/5;

			
			}

			//BORDE SUPERIOR
			else if(v1 == 0 && v2 == 0 && v3 == 0 && v4 == 1 && v6 == 1 && v7 == 1 && v8 == 1 && v9 == 1){
				
			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float dosR = pow(COLOR(maskposX-1,maskposY) - colorkernel[0][1],2);
			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			semej= (unR + dosR + tresR)/3;
	
			}

			//ESQUINA SUPERIOR DRETA
			else if(v1 == 0 && v2 == 0 && v3 == 0 && v4 == 1 && v6 == 0 && v7 == 1 && v8 == 1 && v9 == 0){

			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float dosR = pow(COLOR(maskposX-1,maskposY) - colorkernel[0][1],2);
			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			float sisR = pow(COLOR(maskposX,maskposY+1) - colorkernel[1][2],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			semej= (unR + dosR + tresR + sisR + nouR)/5;
			}

			//BORDE ESQUERRA
			else if(v1 == 0 && v2 == 1 && v3 == 1 && v4 == 0 && v6 == 1 && v7 == 0 && v8 == 1 && v9 == 1){

			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float quatreR = pow(COLOR(maskposX,maskposY-1)-colorkernel[1][0],2);
			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			semej= (unR + quatreR + setR)/3;

			}

			//BORDE DRETA
			else if(v1 == 1 && v2 == 1 && v3 == 0 && v4 == 1 && v6 == 0 && v7 == 1 && v8 == 1 && v9 == 0){


			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			float sisR = pow(COLOR(maskposX,maskposY+1) - colorkernel[1][2],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			semej= (tresR + sisR + nouR)/3;
			
			}

			//ESQUINA INFERIOR ESQUERRA
			else if(v1 == 0 && v2 == 1 && v3 == 1 && v4 == 0 && v6 == 1 && v7 == 0 && v8 == 0 && v9 == 0){

			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float quatreR = pow(COLOR(maskposX,maskposY-1)-colorkernel[1][0],2);
			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			float vuitR = pow(COLOR(maskposX+1,maskposY) - colorkernel[2][1],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			semej= (unR + quatreR + setR + vuitR + nouR)/5;

			}

			//BORDE INFERIOR
			else if(v1 == 1 && v2 == 1 && v3 == 1 && v4 == 1 && v6 == 1 && v7 == 0 && v8 == 0 && v9 == 0){
			
			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			float vuitR = pow(COLOR(maskposX+1,maskposY) - colorkernel[2][1],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			semej= (setR + vuitR + nouR)/3;
			
			}
			
			//ESQUINA INFERIOR DRETA
			else if(v1 == 1 && v2 == 1 && v3 == 0 && v4 == 1 && v6 == 0 && v7 == 0 && v8 == 0 && v9 == 0){

			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			float vuitR = pow(COLOR(maskposX+1,maskposY) - colorkernel[2][1],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			float sisR = pow(COLOR(maskposX,maskposY+1) - colorkernel[1][2],2);
			semej= (tresR + sisR + setR + vuitR + nouR)/5;

			}else{
		
			float unR = pow(COLOR(maskposX-1,maskposY-1) - colorkernel[0][0],2);
			float dosR = pow(COLOR(maskposX-1,maskposY) - colorkernel[0][1],2);
			float tresR = pow(COLOR(maskposX-1,maskposY+1) - colorkernel[0][2],2);
			float quatreR = pow(COLOR(maskposX,maskposY-1)-colorkernel[1][0],2);
			float sisR = pow(COLOR(maskposX,maskposY+1) - colorkernel[1][2],2);
			float setR = pow(COLOR(maskposX+1,maskposY-1) - colorkernel[2][0],2);
			float vuitR = pow(COLOR(maskposX+1,maskposY) - colorkernel[2][1],2);
			float nouR = pow(COLOR(maskposX+1,maskposY+1)- colorkernel[2][2],2);
			semej= (unR + dosR + tresR + quatreR+ sisR + setR + vuitR + nouR)/8;

			}
	
	return semej;
}


/****************************************** MAIN ****************************************************/
main(int argc,char **argv)
{
	//verifica que la cantidad de argumentos es correcta y los asignamos
	if(argc<4)
		uso();
	argv++;
	char * entrada_char=*argv++;
	char * mascara_char=*argv++;
	char * lista_char=*argv++;

printf("Comença a restaurar...\n");

  /****************UTILIZANDO LA LISTA DE SEGMENTOS(NO LA USAMOS)*************************/

	//lee la lista de coordenadas de los segmentos
	//int n_segmentos=0;
	//FILE	*fp;
	//fp= fopen(lista_char,"r");   
	//fscanf(fp, "\n");
	//fscanf(fp, "%d\n", &n_segmentos);
	//int * x0=new int[n_segmentos];
	//int * y0=new int[n_segmentos];
	//int * x1=new int[n_segmentos];
	//int * y1=new int[n_segmentos];
	//for(int i=0;i<n_segmentos;i++)
	//	fscanf(fp,"%d %d %d %d\n",&x0[i],&y0[i],&x1[i],&y1[i]);
	//fclose(fp);

  //escribe en pantalla las coordenadas leidas
  //   fprintf(stderr,"%d\n",n_segmentos);
  //for(int i=0;i<n_segmentos;i++)
  //  fprintf(stderr,"%d %d %d %d\n",x0[i],y0[i],x1[i],y1[i]);

  /*------------------------------------------------------------------------------------*/

//Leemos los archivos y los guardamos en variables de tipo imagen (con estructuras de matrices de floats) para RGB
	/*MASCARA*/
	Imagen maskR=lee(mascara_char,0);
	Imagen maskG=lee(mascara_char,1);
	Imagen maskB=lee(mascara_char,2);
	Imagen maskFLOT=maskR;
	/*ORIGINAL*/
	Imagen uR=lee(entrada_char,0);
	Imagen uG=lee(entrada_char,1);
	Imagen uB=lee(entrada_char,2);
	//compara la original con la mascara
	if (compara_dims(maskR, uR)==0)	midas();
	int fil = uR.fils();
	int col = uR.cols();
	//printf("filas:%d   columnas:%d\n",fil,col);
  /*-------------------------------------------------------------------------------------*/

/****************************************** ENCONTRAMOS MATRIZ MASCARA *******************************************/
  // Crear una matriu amb 1 i 0 segons el forat de la mascara.
  FILE * printar; //document per comprovar que matriupixels conte el forat.
  printar=fopen("output.txt", "w");
  //busquem pixels BLANCS!
  int matriupixels[fil][col];
  for (int i=0; i<fil; i++){
	for(int j=0; j<col; j++){

		if((maskR(i,j)<6) && (maskG(i,j)<6.0) && (maskB(i,j)<6.0)){
			matriupixels[i][j] = 1;//forat
			maskFLOT(i,j)=1;
		}else{
			matriupixels[i][j] = 0;//no forat
			maskFLOT(i,j)=0;
		}
		fprintf(printar,"%d",matriupixels[i][j]);
	}
	fprintf(printar,"\n");
  }
  fclose(printar);
/***************************************************** FUNCIO PRINCIPAL ***********************************************************/

	//FUNCIO PRINCIPAL
/* 
Descripcion:

-Recorremos la matriz de mascara, si el valor es 1(agujero) recorremos toda la matriz en busca del pixel correcto a copiar
-Para eso, recorremos la matriz otra vez considerando que el valor sea 0(no agujero) y para cada uno guardamos la semejanza para ROJO,VERDE y AZUL
-Sumamos estas semejanzas y las multiplicamos por la distancia que tiene respecto la coordenada del pixel de mascara cogido, utilizando sqrt((x1-x2)^2+(y1-y2)^2)
-Una vez tengamos ese valor de semejanza considerando la distancia, miramos si ese valor es menor al mejorvalor de semejanza (cuanto menor sea, mejor)
-Entonces si ha dado que es el mejor valor, guarda esa coordenada en mejorsemejaX y mejorsemejaY esperando que acabe de recorrer la matriz
-Una vez acabado el recorrido de pixels normales lo que hace es copiar el pixel que se ha dado de mejor semejanza (que habiamos guardado sus coordenadas), 
despues marcamos que esa coordenada ya la hemos pintado en la matriz de mascara, asignamos el valor inicial de mejorsemejanza y el for de busqueda de pixels de mascara sigue su recorrido


*/
	float distancia;
	int random=0;
	int mejorsemejaX, mejorsemejaY;
	int mejorsemeja1X, mejorsemeja1Y;
	int mejorsemeja2X, mejorsemeja2Y;
	int mejorsemeja3X, mejorsemeja3Y;
	float semejROJO=0, semejAZUL=0, semejVERDE=0, semej=0;
	float smejorCOLOR=100880;
	float smejorCOLOR2=100880;
	float smejorCOLOR3=100880;
	Imagen uTR=uR;
	Imagen uTG=uG;
	Imagen uTB=uB;
	for (int i=1; i<(fil-1); i++){
		for(int j=1; j<(col-1); j++){
		printf("\n pixel i=%d j=%d ",i,j);
			if(matriupixels[i][j]==1){
			///buscar nou pixel:
			
				for(int x=1; x<(fil-1); x++){
					for(int y=1; y<(col-1); y++){
					//printf("con pixel x=%d y=%d \n",x,y);
						if(matriupixels[x][y]==0){
						//que no compare con pixel de mascara
							semejROJO = semejanzadepixeles(i,j,x,y,uR,maskFLOT);
							//if(semejROJO <0) semejROJO = semejROJO * -1;
							//printf("\n tienen semejanza rojo %f", semejROJO);
							semejVERDE = semejanzadepixeles(i,j,x,y,uG,maskFLOT);
							//if(semejVERDE <0) semejVERDE = semejVERDE * -1;
							semejAZUL = semejanzadepixeles(i,j,x,y,uB,maskFLOT);
							//if(semejAZUL <0) semejAZUL = semejAZUL * -1;
							semej = semejROJO + semejVERDE + semejAZUL;
							//printf("\n tienen semejanza total %f", semej);
							
							distancia = sqrt(pow(x-i,2)+pow(y-j,2));							
							//printf("\n distancia %f",distancia);
							
							semej = semej*distancia;
							if(semej < smejorCOLOR){
							smejorCOLOR = semej;
							mejorsemeja1X = x;
							mejorsemeja1Y = y;
								if(smejorCOLOR < smejorCOLOR2){
								smejorCOLOR2 = smejorCOLOR; 
								mejorsemeja2X = x;
								mejorsemeja2Y = y;
									if(smejorCOLOR2 < smejorCOLOR3){
									smejorCOLOR3 = smejorCOLOR2; 
									mejorsemeja3X = x;
									mejorsemeja3Y = y;
									}
								}				

							}
						
						}
					}
				}
			random = rand()%3;
			random = random+1;
			if(random==1){
				mejorsemejaX=mejorsemeja1X;
				mejorsemejaY=mejorsemeja1Y;
			}else if(random==2){
				mejorsemejaX=mejorsemeja2X;
				mejorsemejaY=mejorsemeja2Y;
			}else if(random==3){
				mejorsemejaX=mejorsemeja3X;
				mejorsemejaY=mejorsemeja3Y;

			}
			uTR(i,j) = uTR(mejorsemejaX,mejorsemejaY);
			uTG(i,j) = uTG(mejorsemejaX,mejorsemejaY);
			uTB(i,j) = uTB(mejorsemejaX,mejorsemejaY);
			maskFLOT(mejorsemejaX,mejorsemejaY)=0;
			matriupixels[mejorsemejaX][mejorsemejaY]=0;
			smejorCOLOR=100880;
			smejorCOLOR2=100880;
			smejorCOLOR3=100880;
			}
		}
  	}

/*******************************MASCARA DE SALIDA*********************************/
//nos aseguramos que ha pintado todos los pixeles
  //PRINTAR IMATGE NOVA:

  FILE * printar2; //document per comprovar que matriupixels conte el forat.
  printar2=fopen("output2.txt", "w");
  //busquem pixels BLANCS!
  for (int i=0; i<fil; i++){
	for(int j=0; j<col; j++){
		fprintf(printar2,"%d",matriupixels[i][j]);
	}
	fprintf(printar2,"\n");
  }
  fclose(printar2);


/***********************************FINALIZAMOS CREANDO EL ARCHIVO DE SALIDA********************************************/
  printf("...FINAL\n");
  char salida_completa[]="imagen_restaurada.tif"; //la extension podria ser gif, jpg, bmp, etc.
  escribe(salida_completa,uTR,uTG,uTB); //se escribe una imagen en colores
  return 0;
  
}





