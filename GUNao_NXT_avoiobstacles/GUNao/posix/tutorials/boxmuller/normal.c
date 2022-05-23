#include <stdio.h>
#include "boxmuller.h"

int main ()
{ 	int i;
	for (i=0; i< 1000; i++) printf ("%12.8f\n",box_muller(0.0,1.0));
}
