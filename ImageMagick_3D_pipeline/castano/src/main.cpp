#include <iostream>
#include "vector.hpp"
#include "image.hpp"
#include "escena.hpp"


int main(int argc,char **argv)
{
	std::cout << "Image processing\n********************" << std::endl;
	if(argc < 3)
	{
		std::cerr << "Parameter missing, usage:\n  app fichero.txt fichero2.txt" << std::endl;
		return 1;
	}

	const char* objetos = argv[1];
	const char* datos = argv[2];
	const char* outputimage = argv[3];

	Image img(512,512);
	Escena escena(objetos,datos,img);

	img.saveTGA(outputimage);
	std::cout << "Done" << std::endl;
	return 0;
}
