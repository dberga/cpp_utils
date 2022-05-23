#include "objeto.hpp"


Objeto::Objeto(){

	ks=0.1;
	ka=0.1;
	kd=0.8;
}



void Objeto::comprobarVisiblidadCaras(const Vector &n){

	Vector a,b;
	int vert1,vert2,vert3;
	printf("\n************************************************************************\n\n");
	printf("Culling\n\n");		

	std::vector<Poligono>::iterator it;
	for(it=poligonos.begin();it!=poligonos.end();++it){
		// obtenemos los indices que hay que acceder al array de vertices
		vert1=it->vertices[0] -1;
		vert2=it->vertices[1] -1;
		vert3=it->vertices[2] -1;
		// creamos los dos vectores mediante la resta
		a=(vertices[vert2]-vertices[vert1]);
		b=(vertices[vert3]-vertices[vert1]);

		// calculamos la normal del poligono J
		it->calculaNormal(a,b);

		// comparamos el vector de la camara con la normal del pol J
		it->esVisible(n);
		if(it->visible==false)printf("Eliminando cara\n");
	}	
	printf("\n************************************************************************\n\n");
}

void Objeto::cambiarCoordenadas(const Matriz& m){

	// metodo para cambiar de coordenadas todos los vertices
	// dada una matriz de cambio de coordenadas
	for(unsigned j=0;j<vertices.size();++j){
		vertices[j]=m*vertices[j];
		printf("\nCambio a coordenadas: ( %f , %f , %f , %f )",	vertices[j].x,vertices[j].y,vertices[j].z,vertices[j].w);
	}
	printf("\n");

}

void Objeto::normalizarHomogeneas(){

	printf("\nCambio a normalizadas\n");
	for(unsigned j=0;j<vertices.size();++j)	{			
		vertices[j]*=1/vertices[j].w;	
		printf("\nCambio a normalizadas: ( %f , %f , %f , %f )", vertices[j].x,vertices[j].y,vertices[j].z,vertices[j].w);
	}
	printf("\n");
}

void Objeto::rasterObjeto(Image &img){


	for(unsigned i=0;i<poligonos.size();++i){
		poligonos[i].raster(img,vertices,aristas);
		// para flat shadding
		//poligonos[i].raster_poligono(img,c);
	}	



	for(std::map<std::string,Arista *>::iterator it=aristas.begin();it!=aristas.end();++it){
		// interpolamos la inensidad por toda la arista
		it->second->interpolarIntensidad(vertices,normales_gouroud);
	}

	for(unsigned i=0;i<poligonos.size();++i){
		// rasterizamos interpolando
		poligonos[i].rasterGouraud(img,c,aristas);
	}	



}


void Objeto::calcularNormalesEnVertices(const Vector &h,const Vector &l){	

	// metodo para sumar todas los normales en los vertices adyacientes

	for(std::vector<Poligono>::iterator it=poligonos.begin();it!=poligonos.end();++it){
		for(unsigned j=0;j<it->vertices.size();++j){
			if(normales_gouroud.find(it->vertices[j])==normales_gouroud.end())
				normales_gouroud[it->vertices[j]].normal=it->normal;
			//else normales_gouroud[it->vertices[j]].normal+=it->normal; // sin media
			else normales_gouroud[it->vertices[j]].normal= (it->normal + normales_gouroud[it->vertices[j]].normal)*0.5; // con media
		}	
	}

	// una vez tenemos las normales calculadas las normalizamos
	for(std::map<int,Vertice>::iterator it=normales_gouroud.begin();it!=normales_gouroud.end();++it){
		it->second.normal.normalizar();	
		// calculamos la intensidad en cada vertice
		it->second.calculaIntensidadEnVertice(h,l,ks,kd,ka);
		//printf("%f -- %f\n",it->second.is,it->second.id);
	}




}

void Objeto::shading(const Vector &luz, const Vector &h){
	for(std::vector<Poligono>::iterator it=poligonos.begin(); it != poligonos.end();++it){	
		it->shadingPoligono(h,luz, ks, kd, ka);

	}

}

