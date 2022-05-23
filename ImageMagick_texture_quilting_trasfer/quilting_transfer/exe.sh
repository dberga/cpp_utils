#!/bin/bash

cd obj
rm -f prueba
cd ..
cd src
make -k prueba
cd ..
#ejecucion imagen.png anchosalida altosalida anchobaldosa altobaldosa precision
./obj/prueba arena.png 418 360 60 60 0.754


