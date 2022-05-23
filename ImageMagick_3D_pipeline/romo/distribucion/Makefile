
incdir2 = /usr/include/ImageMagick

INCLUDES = -I$(incdir2)

CPPFLAGS =  

LIBS = -DHAVE_CONFIG_H -D_REENTRANT -D_FILE_OFFSET_BITS=64 -I/usr/include/ImageMagick -I/X11 -L/usr/lib -L/usr/X11R6/lib -L../lib/ -lMagick++ -ltiff -ljpeg -lz -lpthread -lm -ldl 

OBJS = Vector.o Vertice.o Matrix.o Arista.o Poligon.o io.o Object3d.o Camara.o Raster.o
EXEC = Render cli demo

all: $(EXEC)

	
Render: $(OBJS) Render.cpp
	g++ $(OBJS) $(INCLUDES) $(LIBS) Render.cpp -o Render
	
cli: $(OBJS) cli.cpp
	g++ $(OBJS) $(INCLUDES) $(LIBS) cli.cpp -o cli

demo: $(OBJS) demo.cpp
	g++ $(OBJS) $(INCLUDES) $(LIBS) demo.cpp -o demo

%.o : %.cpp
	g++ -c $(INCLUDES) $< -o $@
	
clean:
	rm $(OBJS) $(EXEC) 
