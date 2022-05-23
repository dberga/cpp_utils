#!/bin/bash

cd obj
rm -f prueba
cd ..
cd src
make -k prueba
cd ..
./obj/prueba AT_poster.png mascara.png lista.txt

