#!/bin/bash

cd obj
rm -f prueba
cd ..
cd src
make -k prueba
cd ..
#ejecucion entrada anchosalida altosalida anchobaldosa altobaldosa precision
./obj/prueba fruta.png 418 360 60 60 0.72


