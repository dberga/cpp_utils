#include <iostream>
#include "image.hpp"
#include <time.h>

int main(int argc,char **argv)
{
	std::cout << "Image processing\n********************" << std::endl;
	if(argc < 2)
	{
		std::cerr << "Parameter missing, usage:\n  app image.tga " << std::endl;
		return 1;
	}

	const char* input_image = argv[1];

	Image img;
	if( img.loadTGA(input_image) == false )
	{
		std::cerr << "Error: image not found: " << input_image << std::endl;
		return 1;
	}

	std::cout << "Image info:\n + Width: " << img.width << "\n + Height: " << img.height << std::endl;


	int newwidth,newheight;
	
	
	std::cout<<std::endl<<"Introduce nuevo tamaño para el ancho de la imagen "<<std::endl;
	std::cin>>newwidth;
	

	//capamos para que no se incremente más del 50% la imagen de ancho
	while(newwidth>(img.width*0.5)+img.width){
		
		std::cout<<std::endl<<"No puedes redimensionar más del 50% la imagen!!! "<<std::endl;
		std::cout<<std::endl<<"Introduce nuevo tamaño para el ancho de la imagen "<<std::endl;
		std::cin>>newwidth;
	}

	std::cout<<"Introduce nuevo tamaño para el alto de la imagen "<<std::endl;
	std::cin>>newheight;

	//capamos para que no se incremente más del 50% la imagen de alto
	while(newheight>(img.height*0.5)+img.height){
		
		std::cout<<std::endl<<"No puedes redimensionar más del 50% la imagen!!! "<<std::endl;
		std::cout<<std::endl<<"Introduce nuevo tamaño para el alto de la imagen "<<std::endl;
		std::cin>>newheight;
	}

	clock_t startTime = clock();

	img.resizeCarving(newwidth,newheight);
	
	clock_t endTime = clock();
	clock_t clockTicksTaken = endTime - startTime;
	double timeInSeconds = clockTicksTaken / (double) CLOCKS_PER_SEC;
	std::cout<<"tarda "<< timeInSeconds <<" segundos\n";


	img.saveTGA("image_resized.tga");



	std::cout << "Done" << std::endl;
	return 0;
}
