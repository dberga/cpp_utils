/* boxmuller.h           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
			  this software for any application provided this
			  copyright notice is preserved.

*/

#include <math.h>
#include <stdlib.h>

double vec_ranf();  /*  uniform random in 0...1*/

double box_muller(double m, double s);	/* normal random variate generator */
					/* mean m, standard deviation s */
