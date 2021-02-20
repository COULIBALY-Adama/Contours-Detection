#!/bin/bash

for par in *
do
	./Contours l $(basename $par) $1
done
exit 0;
