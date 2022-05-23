#include "arista.hpp"


Arista::Arista(){
}
Arista::~Arista(){
	std::vector<Vector*>::iterator it;
	for(it=puntos.begin();it!=puntos.end();++it){
			if(*it) delete *it;		
	}
}

Arista::Arista(int inicio,int final){

	this->inicio=inicio;
	this->final=final;
	id=crear_id(inicio,final);
}

void Arista::crear(int inicio,int final){

	this->inicio=inicio;
	this->final=final;
	this->id=crear_id(inicio,final);
}

// creamos la ID necesaria para le map
std::string Arista::crear_id(int i,int f){

	// creamos una ID con el nummin nummax
	std::ostringstream oss;
	oss << std::min(i,f)<<std::max(i,f);
	return oss.str();
}


void Arista::interpolarIntensidad(std::vector<Vector> & vertices,std::map<int,Vertice> & normales_gouroud){

	// calculamos la distancia total
	distancia=(vertices[inicio-1]-vertices[final-1]).distancia();
	float alpha;
	// la distancia que queremos interpolar
	float distancia2;
	for(unsigned i=0;i<puntos.size();++i){
		// obtenemos la Z y la guardamos para utilizarla despues
		puntos[i]->z=vertices[inicio-1].z;
		distancia2 = (vertices[inicio-1] - *puntos[i]).distancia();
		// calculamos el%
		alpha=distancia2/distancia;
		// interpolamos
		alpha=(normales_gouroud[inicio].I)*(1-alpha)+(normales_gouroud[final].I)*alpha;
		alpha = clamp(alpha,0,1);
		// utilizamos la W para guardar la Intensidad
		puntos[i]->w=alpha;
	}

}