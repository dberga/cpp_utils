

#include"image.hpp"

float clamp(float x, float a, float b) { return x < a ? a : (x > b ? b : x); }




	Color::Color() { r = g = b = 0.0; z=100;}
	Color::Color(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }
	void Color::set(float r, float g, float b) { this->r = r; this->g = g; this->b = b; }


Color operator * (const Color& c,float v) { return Color(c.r*v, c.g*v, c.b*v); }
void operator *= (Color& c,float v) { c = c * v; }
Color operator / (const Color& c,float v) { return Color(c.r/v, c.g/v, c.b/v); }
void operator /= (Color& c,float v) { c = c / v; }
Color operator * (float v, const Color& c) { return Color(c.r*v, c.g*v, c.b*v); }
Color operator + (const Color& a, const Color& b) { return Color(a.r+b.r, a.g+b.g, a.b+b.b); }
void operator += (Color& a,const Color& b) { a = a + b; }
Color operator - (const Color& a, const Color& b) { return Color(a.r-b.r, a.g-b.g, a.b-b.b); }
void operator -= (Color& a,const Color& b) { a = a - b; }



	/* CONSTRUCTORS */
	Image::Image() {
		width = 0; height = 0;
		pixels = NULL;
		zBuffer = NULL;
	}

	Image::Image(unsigned int width, unsigned int height)
	{
		this->width = width;
		this->height = height;
		pixels = new Color[width*height];
		zBuffer = new float[width*height];
		memset(pixels, 0, width * height * sizeof(Color));

		//inicializamos el zBuffer a 10
		std::fill(zBuffer, zBuffer+width * height, 10);
	}

	//copy constructor
	Image::Image(const Image& c) {
		if(pixels) delete pixels;
		pixels = NULL;
		if(zBuffer) delete zBuffer;
		zBuffer = NULL;

		width = c.width;
		height = c.height;
		if(c.pixels)
		{
			pixels = new Color[width*height];
			memcpy(pixels, c.pixels, width*height*sizeof(Color));
		}
		// en caso de copia, copiamos el zbuffer
		if(c.zBuffer)
		{
			zBuffer = new float[width*height];
			memcpy(zBuffer, c.zBuffer, width*height*sizeof(float));
		}
	}



	Image::~Image()
	{
		if(pixels) delete pixels;
		if(zBuffer) delete zBuffer;
	}

	//get the pixel at position x,y
	Color Image::getPixel(unsigned int x, unsigned int y) const 
	{
		return pixels[ y * width + x ];
	}

	//get the pixel at position x,y
	float Image::getZbuffer(unsigned int x, unsigned int y) const 
	{
		if(x<width && y< height){
			return zBuffer[ y * width + x ];
		}
	}
	//set the pixel at position x,y with value C
	void Image::setZbuffer( unsigned int x, unsigned int y , const float z)
	{
		if(x<width && y< height){
			zBuffer[ y * width + x ] = z;
		}
	}

	//set the pixel at position x,y with value C
	void Image::setPixel(const Color& c, unsigned int x, unsigned int y)
	{
		if(x<width && y< height){
			pixels[ y * width + x ] = c;
		}
	}

	//change image size (the old one will remain in the top-left corner)
	void Image::resize(unsigned int width, unsigned int height)
	{
		Color* new_pixels = new Color[width*height];
		unsigned int min_width = this->width > width ? width : this->width;
		unsigned int min_height = this->height > height ? height : this->height;

		for(unsigned int x = 0; x < min_width; ++x)
			for(unsigned int y = 0; y < min_height; ++y)
				new_pixels[ y * width + x ] = getPixel(x,y);

		delete pixels;
		this->width = width;
		this->height = height;
		pixels = new_pixels;
	}


	//Loads an image from a TGA file
	bool Image::loadTGA(const char* filename)
	{
		unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};
		unsigned char TGAcompare[12];
		unsigned char header[6];
		unsigned int bytesPerPixel;
		unsigned int imageSize;

		FILE * file = fopen(filename, "rb");
   		if ( file == NULL || fread(TGAcompare, 1, sizeof(TGAcompare), file) != sizeof(TGAcompare) ||
			memcmp(TGAheader, TGAcompare, sizeof(TGAheader)) != 0 ||
			fread(header, 1, sizeof(header), file) != sizeof(header))
		{
			std::cerr << "File not found: " << filename << std::endl;
			if (file == NULL)
				return NULL;
			else
			{
				fclose(file);
				return NULL;
			}
		}

		TGAInfo* tgainfo = new TGAInfo;
    
		tgainfo->width = header[1] * 256 + header[0];
		tgainfo->height = header[3] * 256 + header[2];
    
		if (tgainfo->width <= 0 || tgainfo->height <= 0 || (header[4] != 24 && header[4] != 32))
		{
			fclose(file);
			delete tgainfo;
			return NULL;
		}
    
		tgainfo->bpp = header[4];
		bytesPerPixel = tgainfo->bpp / 8;
		imageSize = tgainfo->width * tgainfo->height * bytesPerPixel;
    
		tgainfo->data = new unsigned char[imageSize];
    
		if (tgainfo->data == NULL || fread(tgainfo->data, 1, imageSize, file) != imageSize)
		{
			if (tgainfo->data != NULL)
				delete tgainfo->data;
            
			fclose(file);
			delete tgainfo;
			return false;
		}

		fclose(file);

		//save info in image
		if(pixels)
			delete pixels;

		width = tgainfo->width;
		height = tgainfo->height;
		pixels = new Color[width*height];

		//convert to float all pixels
		for(unsigned int y = 0; y < height; ++y)
			for(unsigned int x = 0; x < width; ++x)
			{
				unsigned int pos = y * width * bytesPerPixel + x * bytesPerPixel;
				this->setPixel(Color( tgainfo->data[pos+2] / 255.0f, tgainfo->data[pos+1] / 255.0f, tgainfo->data[pos] / 255.0f),x,height - y - 1);
			}

		delete tgainfo->data;
		delete tgainfo;

		return true;
	}

	// Saves the image to a TGA file
	bool Image::saveTGA(const char* filename)
	{
		unsigned char TGAheader[12] = {0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0};

		FILE *file = fopen(filename, "wb");
		if ( file == NULL )
		{
			fclose(file);
			return false;
		}

		unsigned short header_short[3];
		header_short[0] = width;
		header_short[1] = height;
		unsigned char* header = (unsigned char*)header_short;
		header[4] = 24;
		header[5] = 0;

		//tgainfo->width = header[1] * 256 + header[0];
		//tgainfo->height = header[3] * 256 + header[2];

		fwrite(TGAheader, 1, sizeof(TGAheader), file);
		fwrite(header, 1, 6, file);

		//convert pixels to unsigned char
		unsigned char* bytes = new unsigned char[width*height*3];
		for(unsigned int y = 0; y < height; ++y)
			for(unsigned int x = 0; x < width; ++x)
			{
				Color c = pixels[(height-y-1)*width+x];
				unsigned int pos = (y*width+x)*3;
				bytes[pos+2] = (unsigned char)clamp(c.r * 255,0,255);
				bytes[pos+1] = (unsigned char)clamp(c.g * 255,0,255);
				bytes[pos] = (unsigned char)clamp(c.b * 255,0,255);
			}

		fwrite(bytes, 1, width*height*3, file);
		fclose(file);
		return true;
	}



