#include <iostream>
#include <cassert>

#include "physics.h"

// e: Expected value
// o: Actual value
double deg_error (double e, double o) {
	return (o - e) / e;
}
int main () {
	phys::Plane plane(200, 200), plane2(200, 200, phys::Point2D(10, 10));
	plane.makeParticle(0, 0);
	plane.moveParticle(0);
	assert(plane.y(0) == 0.0 && plane.x(0) == 0.0);
	plane.makeParticle(0, 0, phys::PVector(10, 30));
	plane.moveParticle(1);
	assert(deg_error(5.0, plane.y(1)) < 0.001);
	return 0;
}
