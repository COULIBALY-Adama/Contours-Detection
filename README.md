# Programme de détection de contours dans une image

## Description: Ce programme permet de détecter les contours dans une image en suivant différents algorithmes

Compilation: 
	- Pour compiler le programme
	$ make
	- Pour detecter le contour
	$ ./Contours type_algorithme nom_image seuil1 seuil2
	NB: La valeur "seuil2" n'est necessaire que pour l'algorithme de Canny.

Les valeurs identifiant le type d'algorithme sont:
	. s: algorithme de Sobel;
	. l: algorithme de Laplace;
	. c: algorithme de Canny;
	. r: algorithme de Roberts.

e.g: $ ./Contours r image.pgm 30

Pour appliquer un meme algorithme a une serie d'image,
	$ sh fichier_shell seuil1 seuil2
	$ sh Contours_canny.sh 60 100
	
	NB: La valeur "seuil2" n'est necessaire que pour l'algorithme de Canny.


