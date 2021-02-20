#!/bin/bash

for par in *
do
	./Contours comparaison $1 $(basename $par) 
done
exit 0;
