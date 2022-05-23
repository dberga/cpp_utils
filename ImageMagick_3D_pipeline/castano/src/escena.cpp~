#include "escena.hpp"


Escena::Escena(){}

Escena::Escena(const char *object, const char *info, Image & img){

	//camara y luces
	tamano=512;
	leerObjetos(object);
	leerInfo(info);
	generarCamara();
	culling();
	convertirCoordenadas();
	shading();
	raster(img);
}

void Escena::leerObjetos(const char *fileobject){

	// numeros de vertices y poligonos
	unsigned n_vert,n_pol;
	// abriamos el fichero 
	std::ifstream file(fileobject);

	while(file>>n_vert){
		Objeto objeto;

		//Leemos el número de vértices y de poligonos del fichero txt

		file>>n_pol;

		objeto.poligonos.resize(n_pol);

		//Creamos los arrays del tamaño indicaod en el .txt para los vértices y los polígonos
		objeto.vertices.resize(n_vert);
		objeto.poligonos.resize(n_pol);

		//Guardamos las coordenadas de los vertices	
		for(unsigned i=0;i<n_vert;i++)
		{
			file>>objeto.vertices[i].x>>objeto.vertices[i].y>>objeto.vertices[i].z;
		}

		//Imprimos los vértices leídos en el .txt
		printf("\n************************************************************************\n\n");
		printf("Los %d vertices del fichero .txt son:\n\n",n_vert);

		for (unsigned i=0;i<n_vert;i++)
		{			
			printf("Vértice número %d: ( %f , %f , %f )\n\n",i+1,objeto.vertices[i].x,objeto.vertices[i].y,objeto.vertices[i].z);			
		}

		//Guardamos cada polígono con los vértices que determina el .txt

		//variable para saber el vértice que es
		unsigned num_vertice;

		for(unsigned i=0;i<n_pol;i++)
		{
			//leemos el número de lados que tiene el polígono del .txt
			file>>objeto.poligonos[i].lados;
			objeto.poligonos[i].vertices.resize(objeto.poligonos[i].lados);
			for(int j=0;j<objeto.poligonos[i].lados;j++)
			{
				//leemos del .txt el número de vértice y lo añadimos al vector de 'poligonos'
				file>>num_vertice;
				// almacenamos cada vertice en el poligono correpondiente
				objeto.poligonos[i].vertices[j]=num_vertice;

			}
		}

		//Imprimimos los polígonos y sus vértices
		printf("\n************************************************************************\n\n");
		printf("Los %d poligonos del fichero .txt son:\n",n_pol);

		// printamos los poligonos
		for(unsigned i=0;i<n_pol;i++)
		{
			printf("\nPoligono numero %d con los %lu siguientes lados:\n",i+1,objeto.poligonos[i].vertices.size());		
			for(unsigned j=0;j<objeto.poligonos[i].vertices.size();j++)
			{
				printf("Vertice numero %d\n",objeto.poligonos[i].vertices[j]);
			}
		} 

		//anyadimos el objeto en el vector de objetos de escena
		objetos.push_back(objeto);
	}
}

void Escena::leerInfo(const char *fileinfo){


	std::ifstream datos(fileinfo);
	// para ir leyendo las string del fichero
	std::string papelera;
	// leemos el target de la camara	
	datos >> camara.n.x >> camara.n.y >> camara.n.z ;
	datos >> papelera;

	//leemos posicion de camara	
	for(int i=0;i<3;++i){
		datos>>camara.cam.punto[i];
	}
	datos >> papelera;
	printf("\nCamara: ( %f , %f , %f )\n",camara.cam.x,camara.cam.y,camara.cam.z);


	//leemos vprima 
	for(int i=0;i<3;++i){
		datos>>camara.vprima.punto[i];
	}	
	datos >> papelera;
	printf("\nV prima: ( %f , %f , %f )\n",camara.vprima.x,camara.vprima.y,camara.vprima.z);

	//leemos d,h,f
	datos>>camara.d;
	datos >> papelera;
	datos>>camara.h;
	datos >> papelera;
	datos>>camara.f;
	datos >> papelera;
	printf("\nValores frustrum:\n ( %f , %f , %f )\n\n",camara.d,camara.h,camara.f);


	// leemos la luz
	datos>>luz.x>>luz.y>>luz.z;
	datos >> papelera;

	// leemos el target de la luz
	Vector aux;
	datos>>aux.x>>aux.y>>aux.z;
	datos >> papelera;

	// calculamos el vector director de la luz
	luz=luz-aux;
	// normalizamos el vector
	luz.normalizar();
	printf("Vector luz:\n ( %f , %f , %f )\n\n",luz.x,luz.y,luz.z);

	for(int i=0;i<objetos.size();++i){
		datos >> objetos[i].c.r;
		datos >> objetos[i].c.g;
		datos >> objetos[i].c.b;
	}

	datos >> papelera;

}


void Escena::generarCamara(){
	//calculamos vectores u,v,n de la camara
	camara.inicializar();		
}


void Escena::culling(){
	// para cada objeto comprobamos la visiblidad de sus poligonos
	for(unsigned i=0;i<objetos.size();++i)
		objetos[i].comprobarVisiblidadCaras(camara.n);
}

void Escena::convertirCoordenadas(){

	//cambio de coordenadas a view
	printf("Cambio a view\n");
	for(unsigned i=0;i<objetos.size();++i){
		// mutiplicamos por la matriz view
		objetos[i].cambiarCoordenadas(camara.view);
	}

	//cambio de coordenadas a perspectiva
	printf("\nCambio a perspectiva\n");
	for(unsigned i=0;i<objetos.size();++i){	
		// multiplicamos todas las coordenadas por la matriz de perspectiva
		objetos[i].cambiarCoordenadas(camara.pers);	
		// dividimos por la W
		objetos[i].normalizarHomogeneas();

	}

	//pasamos a display space
	printf("\nCambio a imagen\n");
	for(unsigned i=0;i<objetos.size();++i){
		for(unsigned j=0;j<objetos[i].vertices.size();++j){
			// cada vertice lo normalizamos al tamaño de la imagen
			objetos[i].vertices[j]=((objetos[i].vertices[j]+1.0)*(tamano/2));

			printf("\nCambio a imagen: ( %f , %f , %f , %f )",
				objetos[i].vertices[j].x,
				objetos[i].vertices[j].y,
				objetos[i].vertices[j].z,
				objetos[i].vertices[j].w);
		}
		printf("\n");
	}

}

void Escena::shading(){

	// Calculamos el vector H 
	h=(luz+camara.n)*0.5;
	h.normalizar();

	// realizamos shading a todos los objetos
	for(unsigned i=0; i< objetos.size();++i){				
			objetos[i].shading(h,luz);
			// calculamos las normales en los vertices necesario para gouraud
			objetos[i].calcularNormalesEnVertices(h,luz);
	}

}

void Escena::raster(Image &img){

	// para cada objeto hacemos raster
	for(unsigned i=0;i<objetos.size();++i){
		objetos[i].rasterObjeto(img);
	}
}
