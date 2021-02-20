////********************************************************************************************////
////             Programme de détection de contours dans une image         	                    ////
////                                                                                            ////
////																							////
////             Author: COULIBALY Adama                                                        ////
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

#include <opencv2/opencv.hpp>
#include <math.h>
#include <string.h>

using namespace cv;
using namespace std;

// Fonction de détection de contour de sobel
Mat filtre_sobel(Mat image_entree, int seuil);

// Fonction de détection de contour de laplace
Mat filtre_laplace(Mat image_entree, int seuil);

// Fonction de détection de contour de canny
Mat filtre_canny( Mat image_entree, int seuil_inf, int seuil_sup);

// Fonction de détection de contour de roberts
Mat filtre_roberts(Mat image_entree,int seuil);

// Fonction pour comparer deux images (référence et calculee)
int contoursCorrects(Mat image_reference,Mat image_calculee);

// Foncton pour calculer le nombre de pixel contour dans une image
int pixelContour(Mat image, char type_image);
