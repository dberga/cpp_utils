#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>


#include <Magick++.h>
#include <iostream> 
using namespace std; 
using namespace Magick; 
#include <Imagen.h>

#define MAXCONSTANTE 2000
#define H_BORDER 5
#define W_BORDER 5

///////////////////////////////// FUNCIONES //////////////////////////////////

void uso() 
{	//funcion que sale del programa cuando introducimos mal los parametros
  fprintf(stderr,"Uso: prueba imagen_entrada anchosalida altosalida anchobaldosa altobaldosa precision \n");
  exit(1);
};

Imagen  & crea(int ancho, int alto) 
{	//funcion que crea una imagen de tamano concreto, para facilitar reserva de memoria
	Imagen *im = new Imagen(alto,ancho,255.0);
	return(*im);
}

Imagen  & copiarbaldosa( int posI, int posJ, int anchobaldosa, int altobaldosa, Imagen & im1) 
{	//funcion que devuelve una imagen de dimensiones (anchobaldosa x altobaldosa) que corresponde a los pixeles que empiezan en [posI][posJ] de la imagen im1

	Imagen *im = new Imagen(altobaldosa,anchobaldosa,0.0);

	int imax=altobaldosa+posI;
	int jmax=anchobaldosa+posJ;
	int Y, X;

	X=0;
	for(int x=posI; x<imax; x++){
		Y=0;
	
		for(int y=posJ; y<jmax; y++){
	
		//printf("\n Copy [%d,%d] of [%d,%d]",X,Y,(im->fils()-1),(im->cols()-1));
		(*im)(X,Y)=im1(x,y);
		Y++;	
		}
		X++;
	}
	return(*im);
}

void baldosear(int posI, int posJ,Imagen & im1, Imagen & baldosa1){
	//funcion que pega una imagen baldosa1 en la im1 a partir de las coordenadas [posI][posJ]

	int imax=baldosa1.fils()+posI;
	int jmax=baldosa1.cols()+posJ;
	int Y, X;

	X=0;
	for(int x=posI; x<imax; x++){
		Y=0;
	
		for(int y=posJ; y<jmax; y++){
	
		//printf("\n Put [%d,%d] of [%d,%d]",x,y,(imax-1),(jmax-1));
		im1(x,y)=baldosa1(X,Y);	
		Y++;	
		}
		X++;
	}

}


float restacuadrada(Imagen & im1, Imagen & im2){
	//funcion que hace la resta cuadrada (pixel im1 - pixel im2)^2 para todos los pixeles y despues saca la media de esta resta

	int fil1=im1.fils();
	int col1=im1.cols();
	int fil2=im2.fils();
	int col2=im2.cols();
	float error=0;

	if(fil1!=fil2 || col1!=col2)
	{
		fprintf(stderr,"Diferentes dimensiones al restar imagenes \n");
		return 0;
	}

	
	int largo=fil1*col1;

	for(int i=0; i<largo; i++)
		error += pow(im1.datos[i]-im2.datos[i],2);
	
	error = error/largo;

	return error;
}



///////////////////////////////// 1. LEYENDO DATOS //////////////////////////////////

main(int argc,char **argv)
{


 
  //verifica que la cantidad de argumentos es correcta 
  if(argc<6)
    uso();


  argv++;
  char * entrada_char=*argv++;
  int anchosalida=atoi(*argv++);
  int altosalida=atoi(*argv++);
  int anchobaldosa=atoi(*argv++);
  int altobaldosa=atoi(*argv++);
  float precision =atof(*argv++);
  
	if(precision >= 1){
		printf("\nLa precision ha de ser entre 0 y 1\n");	
		exit(1);
	}

  Imagen uR=lee(entrada_char,0);
  Imagen uG=lee(entrada_char,1);
  Imagen uB=lee(entrada_char,2);

  int nfil=uR.fils(); 
  int ncol=uR.cols();

  int nfilblock = altosalida/altobaldosa;
  int ncolblock = anchosalida/anchobaldosa;
  int totalblock = nfilblock*ncolblock;
  int nfilquilt = nfilblock*altobaldosa; //asi normalizamos divisibilidad de pixeles
  int ncolquilt = ncolblock*anchobaldosa; //asi normalizamos divisibilidad de pixeles
  int bordehorizontal = H_BORDER;
  int bordevertical = W_BORDER;

//////////////////////// 2. COMPROBANDO DATOS /////////////////////////

  if(anchobaldosa > ncol || altobaldosa > nfil)
       printf("\n Has puesto mal el tamano de la baldosa, es demasiado grande, intenta poner valores divisibles\n");

  if(anchosalida < ncol || altosalida < nfil)
       printf("\n Has puesto mal el tamano de la salida, es demasiado pequeno, intenta poner valores multiples\n");

  printf("\n***DATOS (ancho x alto)***\nTextura:%dx%d\nBaldosa:%dx%d\nTexturaQuilted:%dx%d\nMatriz de baldosas:%dx%d\n", 
	  ncol,nfil,anchobaldosa,altobaldosa,ncolquilt,nfilquilt,ncolblock,nfilblock);

  printf("**************************\n");


//////////////////////////// 3. INICIALIZANDO VALORES ///////////////////////////////

	//Imagen de salida
  Imagen salidaR=crea(ncolquilt,nfilquilt);
  Imagen salidaG=crea(ncolquilt,nfilquilt);
  Imagen salidaB=crea(ncolquilt,nfilquilt);


int irandom=0;
int jrandom=0;
int count=0;
int contarintento=0;

int randfilmax=(nfil-altobaldosa)-bordehorizontal; 
int randcolmax=(ncol-anchobaldosa)-bordevertical;
int i=0,j=0;
float errorencontrado;
float errorencontrado1;
float errorencontrado2;
float error;
float bounderror;
int hboundedborder[altobaldosa];
int wboundedborder[anchobaldosa];

int bordeauxiliar;

Imagen borde1R;
Imagen borde1G;
Imagen borde1B;
Imagen borde2R;
Imagen borde2G;
Imagen borde2B;
Imagen borde3R;
Imagen borde3G;
Imagen borde3B;
Imagen borde4R;
Imagen borde4G;
Imagen borde4B;

Imagen bordeaR;
Imagen bordeaG;
Imagen bordeaB;

Imagen bordebR;
Imagen bordebG;
Imagen bordebB;

//////////////////////////// 4. ALGORITMO ///////////////////////////////

////4.1. baldosear la primera baldosa random

count++;
srand ( time(NULL) );
irandom=rand()%randfilmax;
jrandom=rand()%randcolmax;
printf("\nirandom=%d,jrandom=%d",irandom,jrandom);
printf("\nCopiando baldosa...");

	//copiamos la baldosa de la posicion random
Imagen baldosaR=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uR);
Imagen baldosaG=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uG);
Imagen baldosaB=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uB);

printf("\nPoniendo baldosa %d \n",count);
escribe("baldosa.png",baldosaR,baldosaG,baldosaB);
	
	//enganchamos la baldosa en la imagen de salida
baldosear(i,j,salidaR,baldosaR);
baldosear(i,j,salidaG,baldosaG);
baldosear(i,j,salidaB,baldosaB);
escribe("quilted.png",salidaR,salidaG,salidaB);




////4.2. calculamos valor aproximado de tolerancia
 // (cogemos el error entre la primera baldosa y el error con varias baldosas random, lo dividimos por diez y despues lo reducimos) 

	//creamos dos imagenes de borde
float toleranciafinal=0;
float tolerancia=0;
	//el primero es el borde derecho de la primera baldosa puesta
 borde1R=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uR);
 borde1G=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uG);
 borde1B=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uB);

for(int t=0; t<10; t++){ 	//probamos el error 10 veces
				
	irandom=rand()%randfilmax;
	jrandom=rand()%randcolmax;
		//el segundo es el borde izquierdo de la baldosa random encontrada 
	bordeauxiliar = jrandom+anchobaldosa-bordevertical-1;
	 borde2R=copiarbaldosa(irandom,bordeauxiliar,bordevertical,altobaldosa,salidaR);
	 borde2G=copiarbaldosa(irandom,bordeauxiliar,bordevertical,altobaldosa,salidaG);
	 borde2B=copiarbaldosa(irandom,bordeauxiliar,bordevertical,altobaldosa,salidaB);

	tolerancia += restacuadrada(borde1R,borde2R);
	tolerancia += restacuadrada(borde1G,borde2G);
	tolerancia += restacuadrada(borde1B,borde2B);
	tolerancia = sqrt(tolerancia);

	toleranciafinal += tolerancia;
}

precision = ((precision)/10)+0.9;
toleranciafinal = toleranciafinal*(1-precision);

printf("\nTolerancia = %f\n",toleranciafinal);

////4.3. algoritmo de poner baldosas

i=0;                                                //i es el primer pixel de fila la baldosa

for(int bi=0; bi<nfilblock; bi++){                  //bi es la fila de baldosas correspondiente

	j=0;                                        //j es el primer pixel de columna de la baldosa
	
	for(int bj=0; bj<ncolblock ; bj++){         //bj es la columna de baldosas correspondiente

		count++;
		error=1000000;
		bounderror=1000000;	
		
		if (bi==0 && bj==0) { //prescindir de la primera baldosa
			bj=1; 
			j=anchobaldosa;
		}

		printf("\n\nPoniendo baldosa %d de %d",count,totalblock);
		printf("\nBuscando baldosa dentro de tolerancia");
		

			if(bi==0) {                                                //baldosa de tipo 1: poner en parte de arriba
				
				//ahora buscamos una coordenada random y restamos los bordes. escojemos la que tenga error menor a la tolerancia 
				while (error>toleranciafinal && contarintento<MAXCONSTANTE){ 

					contarintento++;

					irandom=rand()%randfilmax;
					jrandom=rand()%randcolmax;

					//creamos dos imagenes de borde
					errorencontrado=0;

						//el primero es el borde derecho de la baldosa anerior puesta
					bordeauxiliar = j-bordevertical-1;
					 borde1R=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaR);
					 borde1G=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaG);
					 borde1B=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaB);
						//el segundo es el borde izquierdo de la baldosa random encontrada
					 borde2R=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uR);
					 borde2G=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uG);
					 borde2B=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uB);

					errorencontrado += restacuadrada(borde1R,borde2R);
					errorencontrado += restacuadrada(borde1G,borde2G);
					errorencontrado += restacuadrada(borde1B,borde2B);
				
					errorencontrado = sqrt(errorencontrado);
					error=errorencontrado;
				}

				//ahora copiamos el borde de la primera baldosa
				bordeaR=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaR);
				bordeaG=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaG);
				bordeaB=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaB);

				//ahora calculamos el cambio de limite de esa posicion de baldosa y escojemos la coordenada con error minimo
				for(int ibounded=0; ibounded < altobaldosa ; ibounded++){
					
					bounderror = 1000000;
					
					for(int jbounded=0; jbounded < bordevertical; jbounded++){
						
						//printf("\n bordea %d,%d",ibounded,jbounded);
						errorencontrado = 0;
						errorencontrado += pow(bordeaR(ibounded,jbounded) - borde2R(ibounded,jbounded),2);
						errorencontrado += pow(bordeaG(ibounded,jbounded) - borde2G(ibounded,jbounded),2);
						errorencontrado += pow(bordeaB(ibounded,jbounded) - borde2B(ibounded,jbounded),2);
						errorencontrado = sqrt(errorencontrado);

						if(errorencontrado < bounderror){
						bounderror = errorencontrado;
						hboundedborder[ibounded] = jbounded;
						//printf("hit in [%d]=%d", ibounded,hboundedborder[ibounded]);
						}
					}
				}
				
				//entonces pegamos la parte derecha del borde de la otra baldosa a partir del corte de limite que habiamos encontrado
				for(int ibounded=0; ibounded < altobaldosa ; ibounded++){				
					for(int jbounded=hboundedborder[ibounded]; jbounded < bordevertical; jbounded++){
						//printf("\n junta %d,%d",ibounded,jbounded);
						bordeaR(ibounded,jbounded)=borde2R(ibounded,jbounded);
						bordeaG(ibounded,jbounded)=borde2G(ibounded,jbounded);
						bordeaB(ibounded,jbounded)=borde2B(ibounded,jbounded);
					}
				}
				escribe("sampleboundedborde1.png",bordeaR,bordeaG,bordeaB);
				
				bordebR=bordeaR;
				bordebG=bordeaG;
				bordebB=bordeaB;
				

			}else if(bj==0) {                                          //baldosa de tipo 2: poner en parte de izquierda

				while (error>toleranciafinal && contarintento<MAXCONSTANTE){ 

					contarintento++;
				
					irandom=rand()%randfilmax;
					jrandom=rand()%randcolmax;
					//creamos dos imagenes de borde
					errorencontrado=0;

						//el primero es el borde de abajo de la baldosa que hay arriba
					bordeauxiliar = i-bordehorizontal-1;
					 borde3R=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaR);
					 borde3G=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaG);
					 borde3B=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaB);
						//el segundo es el borde de arriba de la baldosa random encontrada
					 borde4R=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uR);
					 borde4G=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uG);
					 borde4B=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uB);

					errorencontrado += restacuadrada(borde3R,borde4R);
					errorencontrado += restacuadrada(borde3G,borde4G);
					errorencontrado += restacuadrada(borde3B,borde4B);
				
					errorencontrado = sqrt(errorencontrado);
					error=errorencontrado;	
				}


				//ahora copiamos el borde de la primera baldosa
				bordebR=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaR);
				bordebG=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaG);
				bordebB=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaB);				

				//ahora calculamos el cambio de limite de esa posicion de baldosa y escojemos la coordenada con error minimo
				for(int jbounded=0; jbounded < anchobaldosa ; jbounded++){
					
					bounderror = 1000000;
					
					for(int ibounded=0; ibounded < bordehorizontal; ibounded++){
						
						//printf("\n bordea %d,%d",ibounded,jbounded);
						errorencontrado = 0;
						errorencontrado += pow(bordebR(ibounded,jbounded) - borde4R(ibounded,jbounded),2);
						errorencontrado += pow(bordebG(ibounded,jbounded) - borde4G(ibounded,jbounded),2);
						errorencontrado += pow(bordebB(ibounded,jbounded) - borde4B(ibounded,jbounded),2);
						errorencontrado = sqrt(errorencontrado);

						if(errorencontrado < bounderror){
						bounderror = errorencontrado;
						wboundedborder[jbounded] = ibounded;
						//printf("hit in [%d]=%d", ibounded,hboundedborder[ibounded]);
						}
					}
				}
				
				//entonces pegamos la parte derecha del borde de la otra baldosa a partir del corte de limite que habiamos encontrado
				for(int jbounded=0; jbounded < anchobaldosa ; jbounded++){				
					for(int ibounded=wboundedborder[jbounded]; ibounded < bordehorizontal; ibounded++){
						//printf("\n junta %d,%d",ibounded,jbounded);
						bordebR(ibounded,jbounded)=borde4R(ibounded,jbounded);
						bordebG(ibounded,jbounded)=borde4G(ibounded,jbounded);
						bordebB(ibounded,jbounded)=borde4B(ibounded,jbounded);
					}
				}
				escribe("sampleboundedborde2.png",bordeaR,bordeaG,bordeaB);
				
				bordeaR=bordebR;
				bordeaG=bordebG;
				bordeaB=bordebB;
				
				
			
			}else {                                                    //baldosa de tipo 3: poner en parte del medio

				while (error>toleranciafinal && contarintento<MAXCONSTANTE){ 

					contarintento++;
				
					irandom=rand()%randfilmax;
					jrandom=rand()%randcolmax;
					//creamos cuatro imagenes de borde
					errorencontrado1=0;
					errorencontrado2=0;
					errorencontrado=0;

						//el primero es el borde derecho de la baldosa anerior puesta
					bordeauxiliar = j-bordevertical-1;
					 borde1R=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaR);
					 borde1G=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaG);
					 borde1B=copiarbaldosa(i,bordeauxiliar,bordevertical,altobaldosa,salidaB);
						//el segundo es el borde izquierdo de la baldosa random encontrada
					 borde2R=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uR);
					 borde2G=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uG);
					 borde2B=copiarbaldosa(irandom,jrandom,bordevertical,altobaldosa,uB);
						//el tercero es el borde de abajo de la baldosa que hay arriba
					bordeauxiliar = i-bordehorizontal-1;
					Imagen borde3R=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaR);
					Imagen borde3G=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaG);
					Imagen borde3B=copiarbaldosa(bordeauxiliar,j,anchobaldosa,bordehorizontal,salidaB);
						//el cuarto es el borde de arriba de la baldosa random encontrada
					 borde4R=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uR);
					 borde4G=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uG);
					 borde4B=copiarbaldosa(irandom,jrandom,anchobaldosa,bordehorizontal,uB);
				
					errorencontrado1 += restacuadrada(borde1R,borde2R);
					errorencontrado1 += restacuadrada(borde1G,borde2G);
					errorencontrado1 += restacuadrada(borde1B,borde2B);
					errorencontrado1 = sqrt(errorencontrado1);
					errorencontrado2 += restacuadrada(borde3R,borde4R);
					errorencontrado2 += restacuadrada(borde3G,borde4G);
					errorencontrado2 += restacuadrada(borde3B,borde4B);
					errorencontrado2 = sqrt(errorencontrado2);
				
					errorencontrado=(errorencontrado1+errorencontrado2)/2;
					error=errorencontrado;
				}
			
				//ahora copiamos el borde de la primera baldosa
				bordeaR=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaR);
				bordeaG=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaG);
				bordeaB=copiarbaldosa(i,j,bordevertical,altobaldosa,salidaB);

				//ahora calculamos el cambio de limite de esa posicion de baldosa y escojemos la coordenada con error minimo
				for(int ibounded=0; ibounded < altobaldosa ; ibounded++){
					
					bounderror = 1000000;
					
					for(int jbounded=0; jbounded < bordevertical; jbounded++){
						
						//printf("\n bordea %d,%d",ibounded,jbounded);
						errorencontrado = 0;
						errorencontrado += pow(bordeaR(ibounded,jbounded) - borde2R(ibounded,jbounded),2);
						errorencontrado += pow(bordeaG(ibounded,jbounded) - borde2G(ibounded,jbounded),2);
						errorencontrado += pow(bordeaB(ibounded,jbounded) - borde2B(ibounded,jbounded),2);
						errorencontrado = sqrt(errorencontrado);

						if(errorencontrado < bounderror){
						bounderror = errorencontrado;
						hboundedborder[ibounded] = jbounded;
						//printf("hit in [%d]=%d", ibounded,hboundedborder[ibounded]);
						}
					}
				}
				
				//entonces pegamos la parte derecha del borde de la otra baldosa a partir del corte de limite que habiamos encontrado
				for(int ibounded=0; ibounded < altobaldosa ; ibounded++){				
					for(int jbounded=hboundedborder[ibounded]; jbounded < bordevertical; jbounded++){
						//printf("\n junta %d,%d",ibounded,jbounded);
						bordeaR(ibounded,jbounded)=borde2R(ibounded,jbounded);
						bordeaG(ibounded,jbounded)=borde2G(ibounded,jbounded);
						bordeaB(ibounded,jbounded)=borde2B(ibounded,jbounded);
					}
				}
				escribe("sampleboundedborde1.png",bordeaR,bordeaG,bordeaB);

				
				//ahora copiamos el borde de la primera baldosa
				bordebR=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaR);
				bordebG=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaG);
				bordebB=copiarbaldosa(i,j,anchobaldosa,bordehorizontal,salidaB);				

				//ahora calculamos el cambio de limite de esa posicion de baldosa y escojemos la coordenada con error minimo
				for(int jbounded=0; jbounded < anchobaldosa ; jbounded++){
					
					bounderror = 1000000;
					
					for(int ibounded=0; ibounded < bordehorizontal; ibounded++){
						
						//printf("\n bordea %d,%d",ibounded,jbounded);
						errorencontrado = 0;
						errorencontrado += pow(bordebR(ibounded,jbounded) - borde4R(ibounded,jbounded),2);
						errorencontrado += pow(bordebG(ibounded,jbounded) - borde4G(ibounded,jbounded),2);
						errorencontrado += pow(bordebB(ibounded,jbounded) - borde4B(ibounded,jbounded),2);
						errorencontrado = sqrt(errorencontrado);

						if(errorencontrado < bounderror){
						bounderror = errorencontrado;
						wboundedborder[jbounded] = ibounded;
						//printf("hit in [%d]=%d", ibounded,hboundedborder[ibounded]);
						}
					}
				}
				
				//entonces pegamos la parte derecha del borde de la otra baldosa a partir del corte de limite que habiamos encontrado
				for(int jbounded=0; jbounded < anchobaldosa ; jbounded++){				
					for(int ibounded=wboundedborder[jbounded]; ibounded < bordehorizontal; ibounded++){
						//printf("\n junta %d,%d",ibounded,jbounded);
						bordebR(ibounded,jbounded)=borde4R(ibounded,jbounded);
						bordebG(ibounded,jbounded)=borde4G(ibounded,jbounded);
						bordebB(ibounded,jbounded)=borde4B(ibounded,jbounded);
					}
				}
				escribe("sampleboundedborde2.png",bordeaR,bordeaG,bordeaB);
				
			}
		

		

		printf("\nIntentos de busqueda: %d",contarintento);
		if (contarintento==MAXCONSTANTE) printf(" - MAL! Intenta disminuir la precision");
		else if(contarintento==1) printf(" - MAL! Intenta aumentar la precision");
		contarintento=0;

		printf("\nirandom=%d,jrandom=%d",irandom,jrandom);
		printf("\nError encontrado=%f",error);
		printf("\nError despues del corte=%f",bounderror);		
			//copiamos la baldosa de la coordenada encontrada
		 if(bj<ncolblock-1 && bi<nfilblock-1){
		 baldosaR=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uR);
		 baldosaG=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uG);
		 baldosaB=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa+bordehorizontal,uB);
		}else if(bi<nfilblock-1){
		baldosaR=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa+bordehorizontal,uR);
         	baldosaG=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa+bordehorizontal,uG);
		baldosaB=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa+bordehorizontal,uB);
		}else if(bj<ncolblock-1){
		baldosaR=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa,uR);
         	baldosaG=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa,uG);
		baldosaB=copiarbaldosa(irandom,jrandom,anchobaldosa+bordevertical,altobaldosa,uB);
		}else{
		baldosaR=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa,uR);
         	baldosaG=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa,uG);
		baldosaB=copiarbaldosa(irandom,jrandom,anchobaldosa,altobaldosa,uB);
		}
		printf("\nCopiando baldosa...");
		escribe("baldosa.png",baldosaR,baldosaG,baldosaB);

			//la pegamos en la imagen de salida
		baldosear(i,j,salidaR,baldosaR);
		baldosear(i,j,salidaG,baldosaG);
		baldosear(i,j,salidaB,baldosaB);

		//ahora pegamos ese borde hibrido a la imagen
				baldosear(i,j,salidaR,bordeaR);
				baldosear(i,j,salidaG,bordeaG);
				baldosear(i,j,salidaB,bordeaB);

		//ahora pegamos ese borde hibrido a la imagen
				baldosear(i,j,salidaR,bordebR);
				baldosear(i,j,salidaG,bordebG);
				baldosear(i,j,salidaB,bordebB);

		escribe("quilted.png",salidaR,salidaG,salidaB);
		
	j=j+anchobaldosa;
	}
	i=i+altobaldosa;
}

/////////////////////////////ESCRIBIENDO SALIDA/////////////////////////////////

  printf("\nHecho!\n\n");
  //tmp.~Imagen();

  return 0;
  
}

