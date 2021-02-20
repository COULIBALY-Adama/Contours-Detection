#!/bin/bash

for par in *
do
	./Contours s $(basename $par) $1
done
exit 0;
