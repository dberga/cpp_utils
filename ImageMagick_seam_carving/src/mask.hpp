#ifndef MASK_H
#define MASK_H

// clase para las mascaras de lo gradientes
class Mask
{
public:
	int* array;
	unsigned int width;
	unsigned int height;
	int center_x;
	int center_y;

	Mask() {
		array = NULL;
		chooseMask();
	}

	~Mask() {
		if(array) delete array;
	}

	Mask(int * array , unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;
		this->array = new int[width*height];
		memcpy(this->array, array, height*width *sizeof(int));
		center_x = width/2;
		center_y = height/2;
	}

	//get the pixel at position y
	int getValue(unsigned int x, unsigned int y) const 
	{
		// se usa el mismo convenio que los pixeles
		return array[ y * width + x];
	}

	void setMask(int * array , unsigned int width, unsigned int height){


		this->width = width;
		this->height = height;
		this->array = new int[width*height];
		memcpy(this->array, array, height*width *sizeof(int));
		center_x = width/2;
		center_y = height/2;

	}

	void chooseMask(){

		// diferentes mascaras para calcular gradientes
		int mask_sobel[] = { 	-1,  0,  1 ,
				   				-2,  0,  2 , 
				   				-1,  0,  1  };

		int mask_prewitt[] = { 	-1,  0,  1 ,
				   				-1,  0,  1 , 
				   				-1,  0,  1  };


		int mask_1d[] = { 	-1,  0,  1 };

		char opcion;

		std::cout << "Qué máscara quieres??\n 1)Sobel\n 2)Prewitt \n 3)1D (-1 0 1)" << std::endl;
		
		//ignoramos 1000 caracteres hasta salto de línea
		std::cin.ignore(1000,'\n');
		// para poder leer el enter
		opcion = std::cin.get();

		
		switch(opcion){
			case '1':
				std::cout << "1)Sobel\n" << std::endl;
				setMask(mask_sobel ,3 ,3);
				break;
			case '2':
				std::cout << "2)Prewitt" << std::endl;
				setMask(mask_prewitt ,3 ,3);
				break;
			case '3':
				std::cout << "3)1D (-1 0 1)" << std::endl;
				setMask(mask_1d , 3 ,1);
				break;
			default:
				std::cout << "using (default)1D (-1 0 1)" << std::endl;
				setMask(mask_1d , 3 ,1);
		}
	}

};

#endif
