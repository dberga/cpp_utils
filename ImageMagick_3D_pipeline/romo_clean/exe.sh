#!/bin/bash

cd obj
rm -f prueba
cd ..
cd src
make -k prueba
cd ..
./obj/prueba cubo.txt

