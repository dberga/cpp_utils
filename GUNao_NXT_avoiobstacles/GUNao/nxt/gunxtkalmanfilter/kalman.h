

#include <math.h>
#include <stdlib.h>

class KALMAN_FILTER{ 

public:

static double kfilter(double latestvalue, double previousvalue);
					
static double kfilter(double latestvalue, double previousvalues[]);

};
