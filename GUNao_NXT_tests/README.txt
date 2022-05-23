DAVID BERGA GARRETA
NIA: 146716

ROBOTICS
EJECUCION DE PROGRAMAS DE LEGO NXT con nxtpp5 para linux


/////////////Como ejecutar los programas /////////////

-------- tarea 1 -> el robot da vueltas cuando es apretado y se muestra las revoluciones por pantalla (solo es para test)

sudo ./plainTest


-------- tarea 2 -> el robot se mueve y esquiva objetos

sudo ./avoidObstacles


-------- tarea 3 -> el robot mantiene la distancia con un objeto que detecta su sonar. cuando se aprieta uno de sus botones, mantiene la distancia con un nuevo objeto

sudo ./distanceFeedControl


-------- tarea 4 -> el robot mantiene la distancia con un objeto que detecta su sonar. cuando se aprieta uno de sus botones, mantiene la distancia con un nuevo objeto (usando threads, como es concurrente, se supone que es mas efectivo que la tarea 3)

sudo ./distanceThreadControl



/////////////Videos de demostracion  /////////////

video 1 -> Esquiva obstaculos (tarea 2) -> 
https://vimeo.com/51934654
video 2 -> Intenta mantener distancia (tarea 3-4) -> 
https://vimeo.com/51935635


 /////////////Como instalar nxtpp0-5 para linux (tutorial de clase)  /////////////

1. Tener instalado el compilador c++ con "sudo apt-get install build-essential"
2. Tener instalado libusb con "sudo apt-get install libusb-dev"
3. Descargar y extraer nxtpp0-5 de http://sourceforge.net/projects/nxtpp/?source=directory
4. Aplicar el comando "sudo make" dentro de nxtpp0-5 para que se haga el build del programa


/////////////Como compilar los programas /////////////

1. Meter cada programa en una carpeta dentro de "examples/linux/ "
2. Aplicar el comando "make clean" si hay ejecutable
3. Compilar el Makefile con "sudo make"





