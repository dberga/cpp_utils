#include "poligono.hpp"




Poligono::Poligono(){visible=true;}

Poligono::Poligono(int l){
	lados=l;
	vertices.resize(lados);
	visible=true;
}



void Poligono::calculaNormal(const Vector &v1, const Vector &v2){
	// calculamos la normal del vertice
	normal= v1 ^ v2;
	normal.normalizar();
}




void Poligono::esVisible(const Vector &n){
	// comprobamos la visiblidad haciendo el dot product
	// aqui se hacce el culling
	if((normal*n)>=0) 
		visible=false;
}


void Poligono::raster(Image &img,std::vector<Vector> & vertarray, std::map<std::string,Arista *> &aristas){

	float x,xs,xe;
	int ys,ye;
	float zs,ze;
	float m;
	// necesario para crear la id para el mapa
	int ids,ide;

	Arista * arista;
	Vector * v;
	bool esSwap;

	ids=vertices.back();
	xs=vertarray[ids -1].x;	
	ys=vertarray[ids -1].y;
	zs=vertarray[ids -1].z;



	for(unsigned i = 0; i< vertices.size();++i){
		esSwap=false;
		ide=vertices[i];


		xe=vertarray[ide -1].x;	
		ye=vertarray[ide -1].y;
		ze=vertarray[ide -1].z;	

		arista = new Arista;
		arista->crear(ids,ide);
		if(ye-ys!=0){

			if(ys>ye){
				esSwap=true;
				std::swap(xe,xs);
				std::swap(ye,ys);
				std::swap(ze,zs);
				std::swap(ide,ids);
			}
			x=xs;
			m=(xe-xs)/(ye-ys);



			for(int y=ys;y<ye;++y){

				//img.setPixel(Color(1.0,0.0,0.0),(int)(0.5+x),y);
				v = new Vector;
				v->x=(int)(x+0.5);
				v->y=y;
				// interpolamos la Z para la arista
				float alpha=(y-ys)/(ye-ys);
				float z=(ze-zs)*alpha+ze;
				v->z=z;
				v->w=1;

				arista->puntos.push_back(v);
				
				x=x+m;
			}

		}else {

			if(xs>xe){
				esSwap=true;
				std::swap(xe,xs);
				std::swap(ye,ys);
				std::swap(ze,zs);
				std::swap(ide,ids);
			}

			for(float x=xs;x<xe;++x){
				//img.setPixel(Color(1.0,0.0,0.0),(int)(0.5+x),ye);

			}

		}
		if(!esSwap){
			xs = xe;
			ys = ye;
			zs = ze;
			ids = ide;
		}
		aristas_id.push_back(arista->id);
		aristas[arista->id]=arista;
	}	

}

bool comparador (const Vector * vec1,const Vector * vec2) { 
	return vec1->x<vec2->x;
}

void Poligono::rasterGouraud(Image &img,Color &c,std::map<std::string,Arista *> &aristas){


	std::vector<std::string>::iterator it;
	for(it=aristas_id.begin();it!=aristas_id.end();++it){
		std::vector< Vector * >::iterator it_vec;
		for(it_vec=aristas[*it]->puntos.begin();it_vec!=aristas[*it]->puntos.end();++it_vec){
			if(aristas_list.find((*it_vec)->y)==aristas_list.end())
				aristas_list[(*it_vec)->y] = std::vector<Vector * >();
			aristas_list[(*it_vec)->y].push_back(*it_vec);
		
		}
	}

	Color caux=c;
	float alpha=0;
	float incrementoZ ;
	float incrementoI ;
	float z_interpolada;

	if(visible){

		// recorremos todas las aristas por orden de Y
		for(std::map<int,std::vector<Vector*> >::iterator it=aristas_list.begin();it!=aristas_list.end();++it){
			//  ordenamos el vectorpor la coordenadas X
			sort(it->second.begin(),it->second.end(), comparador);
			for(unsigned k=0;k<it->second.size()-1;++k){

				// si nos encontramos en el caso que hay que pintar
				if(k%2==0){
					// seteamos los incrementos de Z e Intensis
					incrementoZ = 0;
					incrementoI = 0;
					// calculamos los incrementos para cada cosa
					if(it->second[k+1]->z != it->second[k]->z)
						incrementoZ =  (1 / (it->second[k+1]->x - it->second[k]->x))*(it->second[k+1]->z - it->second[k]->z);
					if(it->second[k+1]->x != it->second[k]->x)
						incrementoI = (1 / (it->second[k+1]->x - it->second[k]->x))* (it->second[k+1]->w - it->second[k]->w);

					// inicializamos la intensidad 0
					alpha = it->second[k]->w - incrementoI;
					// inicializamos la Z 0
					z_interpolada = it->second[k]->z - incrementoZ;
					for(int i=(it->second[k]->x);i<it->second[k+1]->x;++i){
						// calculamos la siguente Z e I apartir de la anterior
						alpha +=incrementoI; 
						z_interpolada +=incrementoZ;
						// miramos el Zbuffer si es necesario escribir en la imagen
						if(zbuffer(img, i,it->first,z_interpolada))
							img.setPixel(caux*alpha,i,it->first);

					}
				}

			}

		}	
		c=caux;
	}
}




void Poligono::rasterPoligono(Image &img,Color &c){

	
	if(visible){

		std::map<int,std::vector<Vector * > >::iterator it;
		for(it=aristas_list.begin();it!=aristas_list.end();++it){

			sort(it->second.begin(),it->second.end(),comparador);
			for(unsigned k=0;k<it->second.size()-1;++k){

				for(int i=it->second[k]->x;i<it->second[k+1]->x;++i){
					//c.set(1.,0.,0.);
					Color caux(1.,0.,0.);		
					if(k%2==0){
						float f=(is+id+ia);																		
						//printf("%f--- %f = %f\n",id,is,is+id);
						caux.set(f*caux.r,f*caux.g,f*caux.b);
						//printf("%f  -- %f--- %f = %f\n",f,c.r,c.g,c.b);
						img.setPixel(caux,i,it->first);
					}
				}

			}

		}	
	}
}


void Poligono::shadingPoligono(const Vector &h,const Vector &l,float ks,float kd,float ka){
	// si es visible calculamos todas las intensidades
	if(visible){

		id=l*normal*kd;
		id=clamp(id,0,1);
		is=pow(normal*h,1)*ks;
		is=clamp(is,0,1);
		ia=ka*1;
	}

}


bool Poligono::zbuffer(Image &img, int x,int y,float z){

	// normalizamos la Z entre 0 y 1
	z = (z/(img.width/2))-1;
	// si el pixel que ya habia tenia mas Z lo actualiozamos
	if(img.getZbuffer(x,y) > z){
		img.setZbuffer(x,y,z);
		return true;
	}
	return false;
}





