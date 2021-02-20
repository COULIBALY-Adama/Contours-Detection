#!/bin/bash

for par in *
do
	./Contours r $(basename $par) $1
done
exit 0;
