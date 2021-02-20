////********************************************************************************************////
////             Programme de détection de contours dans une image         	            		////
////                                                                                            ////
////																							////
////             Author: COULIBALY Adama                  										////
////                                           													////
////																							////
////             Compilation: make      												 		////
////						  																 	////
////				  Description: Ce programme permet de détecter les contours dans une image  ////
////                               en suivant différents algorithmes                            ////
////													 										////
////	Sources: http://docs.opencv.org/doc/tutorials/											////
////********************************************************************************************////

#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
//#include "Traitements.h"

using namespace cv;
using namespace std;

// Fonction pour convertir l'image entree en image niveau de gris
//et pour supprimer les bruit grâce au filtre gaussien
Mat mise_en_forme_image(Mat image_entree){

	 Mat image_sortie;
	// Suppression des bruits avec le filtre gaussien
	 GaussianBlur( image_entree, image_entree, Size(3,3), 0, 0, BORDER_DEFAULT );

	  // Convertion de l'image entree en image de niveau de gris
	  cvtColor( image_entree, image_sortie, COLOR_RGB2GRAY );
	  return image_sortie;
}

//Fonction de détection de contour: Sobel
Mat filtre_sobel(Mat image_entree, int seuil){

     Mat image_en_gris = mise_en_forme_image(image_entree);
	 Mat image_sortie;
	 int scale = 1;
	 int delta = 0;
	 int ddepth = CV_16S;

  // Variables images pour recevoir les images du gradiant suivant X et Y
  Mat gradient_x, gradient_y;
  Mat abs_gradient_x, abs_gradient_y;

  // Le gradiant suivant X
  Sobel( image_en_gris, gradient_x, ddepth, 1, 0, 3, scale, delta, BORDER_DEFAULT );

  // Norme du gradiant suivant X
  convertScaleAbs( gradient_x, abs_gradient_x );

  // Le grandiant suivant Y
  Sobel( image_en_gris, gradient_y, ddepth, 0, 1, 3, scale, delta, BORDER_DEFAULT );

  // Norme du gradiant suivant Y
  convertScaleAbs( gradient_y, abs_gradient_y );

  // Addition des normes des gradiants suivant les axes X et Y
  addWeighted( abs_gradient_x, 0.5, abs_gradient_y, 0.5, 0, image_sortie );

  // Seuillage en fonction de la valeur de seuil entrée
  threshold( image_sortie, image_sortie,seuil,255,THRESH_BINARY );

  return image_sortie;
}

// Fonction de détection de contour: Laplacien
Mat filtre_laplace(Mat image_entree, int seuil){

	Mat image_gris = mise_en_forme_image(image_entree);
	Mat image_sortie;
	int taille_masque = 3;
	int scale = 1;
	int delta = 0;
	int ddepth = CV_16S;

	// Application de la fonction laplacienne sur l'image
	Mat abs_image_sortie;
	Laplacian( image_gris, image_sortie, ddepth, taille_masque, scale, delta, BORDER_DEFAULT );
	convertScaleAbs( image_sortie, abs_image_sortie );

   // Faire le seuillage en fonction de la valeur de seuil entree
   threshold( abs_image_sortie, abs_image_sortie,seuil,255,THRESH_BINARY );

	return abs_image_sortie;
}

//Fonction de détection de contour de Canny
Mat filtre_canny( Mat image_entree, int seuil_inf, int seuil_sup){

	Mat image_sortie, image_en_gris, contour_detecte;
	int taille_masque = 3;

	// Création de matrix de même dimension que l'image originale
	image_sortie.create( image_entree.size(), image_entree.type() );

	// Conversion de l'image en image de niveau de gris
	cvtColor( image_entree, image_en_gris, CV_BGR2GRAY );

	// Reduction du bruit dans l'image
	blur( image_en_gris, contour_detecte, Size(3,3) );

	// Application de fonction de canny
	Canny( contour_detecte, contour_detecte, seuil_inf, seuil_sup, taille_masque );

	// Utilisé la sortie de canny comme mask
	image_sortie = Scalar::all(0);
	image_entree.copyTo( image_sortie, contour_detecte);
	// Faire le seuillage en portant à 255 toutes les valeurs strictement positives
	threshold( image_sortie, image_sortie,0,255,THRESH_BINARY );

	return image_sortie;
}

// Fonction de détection de contour de Roberts
Mat filtre_roberts(Mat image_entree,int seuil){

  int hauteur = image_entree.rows, largeur = image_entree.cols;
  Mat image_sortie;
  IplImage* image_traitee;
  IplImage image = image_entree;
  int gradient_x,gradient_y;

 // utiliser une image  pour la modification de l'image entree
  image_traitee = cvCreateImage(cvSize(largeur, hauteur), IPL_DEPTH_8U, 1 );

  for(int i=0;i<hauteur-1;i++)
  {
   	for(int j=0;j<largeur-1;j++)
      {
	 		CvScalar s;
			s = cvGet2D (&image, i,j);
			double x1 = s.val[0];

			s = cvGet2D (&image, i+1,j+1);
			double x2 = s.val[0];
			//calcul du gradient suivant X
			gradient_x = abs(x1 - x2);

			s = cvGet2D (&image, i+1,j);
			double y1 = s.val[0];

			s = cvGet2D (&image,i,j+1);
			double y2 = s.val[0];
			//calcul du gradient suivant Y
			gradient_y = abs(y1 - y2);

			s.val[0] = gradient_x+gradient_y;
			cvSet2D(image_traitee,i,j,s);
		}
   }
   // Seuillage du résultat
	for(int i=0; i < hauteur-1;i++)
  	{
   	for(int j=0; j< largeur-1;j++)
 		{
			CvScalar s;
			s = cvGet2D (image_traitee, i,j);
			double dx1 = s.val[0];
			if (dx1 > seuil ) {
				s.val[0] = 255;
			} else {
				s.val[0] = 0;
			}
			cvSet2D(image_traitee,i,j,s);
		}
	}
	image_sortie = image_traitee;
	return image_sortie;
}

// Fonction de comparaison d'une image de référence à une image de contour calculee
int contoursCorrects(Mat image_reference,Mat image_calculee){
    //Ajout de bordures aux images entrées
	copyMakeBorder( image_reference, image_reference, 2, 2, 2, 2, BORDER_CONSTANT, 100 );
	copyMakeBorder( image_calculee, image_calculee, 2, 2, 2, 2, BORDER_CONSTANT, 100 );
	CvScalar s,s1, s2;
	IplImage img_ref = image_reference;
	IplImage img_cal = image_calculee;

	int nombreBonPixel = 0;
	double dx1;
	double dx2;

	//Comparaison des images
	for(int i = 2; i < image_calculee.rows-2; i++){

		for(int j = 2 ; j < image_calculee.cols-2 ; j++) {

			s1 = cvGet2D (&img_cal, i,j);
			s2 = cvGet2D (&img_ref, i,j);

			dx1 = s1.val[0];
			dx2 = s2.val[0];


				if ((dx1 == 255) && (dx2 == 0)) {

					nombreBonPixel++;
					s.val[0] = 100;
					cvSet2D(&img_cal,i,j,s);
					cvSet2D(&img_ref,i,j,s);
				}
				else { // Regarder un voisinage de 5x5 pour trouver le bon pixel


					for(int k = i-2; k < i+3; k++){
						for(int l = j-2; l < j+3; l++){

							s2 = cvGet2D (&img_ref, k,l);
							dx2 = s2.val[0];

							if ((dx1 == 255) && (dx2 == 0)) {

								nombreBonPixel++;
								s.val[0] = 100;
								cvSet2D(&img_cal, k, l,s);
								cvSet2D(&img_ref, k, l,s);

							}
						}
					}
				}
			}
		}
		return nombreBonPixel;
}

// Fonction retournant le nombre de pixel contour dans une image
int pixelContour(Mat image_entree, char type_image) {

	CvScalar s;
	IplImage img_in = image_entree;
	double dx;

	int nombrePixelContour = 0; // Nombre de pixel contour détecté

	for(int i=0;i<image_entree.rows;i++)
  	{
		for(int j=0;j<image_entree.cols;j++)
		{
			s = cvGet2D (&img_in, i,j);
			dx = s.val[0];

			if (type_image == 'r')
			{
				if (dx == 0)
				{
					nombrePixelContour++;
				}
			}
			else
			{
				  if (dx == 255) nombrePixelContour++;
			}
	 	}
	}
	return nombrePixelContour;
}
