HOME = ..

TARGETS = $(APP) 

OBJECTS = $(OBJAPP) 



###############################
# esto es general

CPP = g++ 


SRC= $(HOME)/src
OBJ = $(HOME)/obj
bindir=$(HOME)


CPPFLAGS = -std=c++0x 




##########################################
# APP

APP = app
OBJAPP = main.o 

installapp: $(APP)
$(bindir)/app: $(OBJ)/app
	-rm $(bindir)/app
	cp -p $(OBJ)/app $(bindir)/app

app: $(OBJ)/app

$(OBJ)/app: $(addprefix $(OBJ)/, $(OBJAPP))
	$(CPP) $(CPPFLAGS) -o $(addprefix $(HOME)/,$(APP)) $(addprefix $(OBJ)/, $(OBJAPP))


#########################################
# reglas generales


$(OBJ)/%.o : %.cpp
	$(CPP) -c $(CPPFLAGS)  $< -o $@


.PHONY: app

all: directorios $(TARGETS)

clean:
	-rm $(TARGETS)
	-rm $(OBJ)/*.o

cleanall: clean

build: clean all

install: $(subst $(OBJ),$(bindir),$(TARGETS))

directorios:
	@if [ ! -d $(bindir) ]; then mkdir $(bindir); fi
	@if [ ! -d $(OBJ) ]; then mkdir $(OBJ); fi



