
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "kalman.h"




double KALMAN_FILTER:: kfilter(double latestvalue, double previousvalue)
{				       
	double average;
	average = (latestvalue + previousvalue)/2;
	

	return( average );
}

double KALMAN_FILTER:: kfilter(double latestvalue, double previousvalues[] )
{				       
	double average;
	double previousvalue = 0;
	for (int position = 0; position < (sizeof(previousvalues)/sizeof(double)); position++){
		previousvalue += previousvalues[position];
	}
	previousvalue = previousvalue / (sizeof(previousvalues)/sizeof(double));
	average = (previousvalue + latestvalue)/2;
	
	return( average );
}






