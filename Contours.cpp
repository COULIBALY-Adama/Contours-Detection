////********************************************************************************************////
////             Programme de détection de contours dans une image         	            		////
////                                                                                            ////
////																							////
////             Author: COULIBALY Adama                  										////
////                                           													////
////																							////
////             Compilation: make      												 		////
////																							////
////				  Description: Ce programme permet de détecter les contours dans une image  ////
////                               en suivant différents algorithmes                            ////
////													 										////
////	Sources: http://docs.opencv.org/doc/tutorials/											////
////********************************************************************************************////

// Importation des bibliothèques nécessaires
#include "Traitements.h"
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv) {
	// Comparaison des images calculées aux images de référence
	if (strcasecmp(argv[1], "comparaison") == 0) {

		Mat image_ref = imread(argv[2]);
		Mat image_cal = imread(argv[3]);

		if ((!image_ref.data) || (!image_cal.data)) {
			cout << "Impossible de charger l'une des images\n" << endl;
			return -1;
		}

		int faux_positifs = 0;
		int faux_negatifs = 0;

		int contours_reference = pixelContour(image_ref, 'r');
		int contours_detecte = pixelContour(image_cal, 'c');

		int contours_corrects = contoursCorrects(image_ref, image_cal);
		faux_positifs = contours_detecte - contours_corrects;
		faux_negatifs = contours_reference - contours_corrects;

		double P = (double) contours_corrects
				/ (contours_corrects + faux_positifs + faux_negatifs);
		double tfp = (double) faux_positifs
				/ (contours_corrects + faux_positifs + faux_negatifs);
		double tfn = (double) faux_negatifs
				/ (contours_corrects + faux_positifs + faux_negatifs);

		cout << "Comparaison des images " << argv[2] << " et " << argv[3]
				<< endl;
		cout << "Contours détectés :" << contours_detecte << endl;
		cout << "Contours references :" << contours_reference << endl;
		cout << "Contours corrects :" << contours_corrects << endl;
		cout << "Faux positifs :" << faux_positifs << endl;
		cout << "Faux negatifs :" << faux_negatifs << endl;
		cout << "Performance :" << P << endl;
		cout << "Taux de faux positifs :" << tfp << endl;
		cout << "Taux de faux negatifs :" << tfn << endl;
	} else {

		Mat image_entree;
		Mat image_sortie;
		int seuil = atoi(argv[3]);

		// Chargement de l'image
		image_entree = imread(argv[2]);

		if (!image_entree.data) {
			return -1;
		}

		char* type_filtre = argv[1];

		switch (type_filtre[0]) {
		// application filtre de Sobel
		case 's': {
			image_sortie = filtre_sobel(image_entree, seuil);

			string nom_image_sobel = "images_sobel/filtre_sobel-"
					+ (string) argv[2] + "-seuil=" + (string) argv[3] + ".png";
			if (!imwrite(nom_image_sobel, image_sortie))
				cout << "Erreur lors de l'enregistrement" << endl;
		}
			break;
			// application filtre de Laplace
		case 'l': {
			image_sortie = filtre_laplace(image_entree, seuil);

			string nom_image_laplace = "images_laplace/filtre_laplace-"
					+ (string) argv[2] + "-seuil=" + (string) argv[3] + ".png";
			if (!imwrite(nom_image_laplace, image_sortie))
				cout << "Erreur lors de l'enregistrement" << endl;
		}
			break;
			// application filtre de Canny
		case 'c': {
			int seuil1 = atoi(argv[3]);

			int seuil2 = atoi(argv[4]);

			image_sortie = filtre_canny(image_entree, seuil1, seuil2);

			string nom_image_canny = "images_canny/filtre_canny-"
					+ (string) argv[2] + "-seuil1=" + (string) argv[3]
					+ "-seuil2=" + (string) argv[4] + ".png";

			if (!imwrite(nom_image_canny, image_sortie))
				cout << "Erreur lors de l'enregistrement" << endl;
		}
			break;
			// application filtre de Roberts
		case 'r': {
			image_sortie = filtre_roberts(image_entree, seuil);

			string nom_image_roberts = "images_roberts/filtre_roberts-"
					+ (string) argv[2] + "-seuil=" + (string) argv[3] + ".png";

			if (!imwrite(nom_image_roberts, image_sortie))
				cout << "Erreur lors de l'enregistrement" << endl;
		}
			break;

		default:
			cout << "L'option choisie ne correspond à aucun filtre " << endl;

		}
	}

	return 0;
}
