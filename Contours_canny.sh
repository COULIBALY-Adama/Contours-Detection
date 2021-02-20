#!/bin/bash

for par in *
do
	./Contours c $(basename $par) $1 $2
done
exit 0;
