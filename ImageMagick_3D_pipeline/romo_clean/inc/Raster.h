
#ifndef RASTA
#define RASTA

#include "Object3d.h"
#include "Magick++.h"
#include "Material.h"

void rasterizar(Magick::Image& res, Object3d& screenSpace, int side, Camara& cam, Luz luz, float** zbuffer);

#endif

