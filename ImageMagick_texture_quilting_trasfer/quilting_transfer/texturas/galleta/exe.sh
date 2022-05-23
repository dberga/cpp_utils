#!/bin/bash

cd obj
rm -f prueba
cd ..
cd src
make -k prueba
cd ..
#ejecucion entrada anchosalida altosalida anchobaldosa altobaldosa precision
./obj/prueba galleta.png 418 360 96 96 0.68


