/* boxmuller.c           Implements the Polar form of the Box-Muller
                         Transformation

                      (c) Copyright 1994, Everett F. Carter Jr.
                          Permission is granted by the author to use
			  this software for any application provided this
			  copyright notice is preserved.

*/

#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "boxmuller.h"


double vec_ranf()  /*  uniform random in 0...1*/
	{
		long int v_random = random();
		double d_v_random = v_random*1.0;
		double max_random = RAND_MAX*1.0;
		double r_value =d_v_random/max_random;
		if (r_value > 1.0) return 1.0;
		else
		  {     if (r_value < 0.0) return 0.0;
			else return r_value;
		  }
	};



double box_muller(double m, double s)	/* normal random variate generator */
{				        /* mean m, standard deviation s */
	double x1, x2, w, y1;
	static double y2;
	static int use_last = 0;

	if (use_last)		        /* use value from previous call */
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * vec_ranf() - 1.0;
			x2 = 2.0 * vec_ranf() - 1.0;
			w = x1 * x1 + x2 * x2;
		        //fprintf (stderr,"vec_ran=%f  \n",vec_ranf() );
		        //fprintf (stderr,"x1=%f  : x2 = %f   w= %f\n",x1,x2,w);

		} while ( (w >= 1.0) || (w<= 0.0) );

		w = sqrt( (-2.0 * log( w ) ) / w );
		y1 = x1 * w;
		y2 = x2 * w;
		use_last = 1;
	}

	return( m + y1 * s );
}



