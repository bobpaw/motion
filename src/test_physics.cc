#include <iostream>
#include <cassert>

#include "physics2.h"


struct MyParticleSystem: phys::ParticleSystem {

};


// e: Expected value
// o: Actual value
double deg_error (double e, double o) {
	return (o - e) / e;
}
int main () {
	phys::ParticleSystem screen(200, 200, 10);
	screen.add_particle({100, 100}, {10, 10});
	screen.move_particle(0, {10, 10});
	screen.move_particle(0, {-10, -10});
	screen.update(sf::milliseconds(1));
	screen.add_particle({50, 50}, {10, 10});
	screen.update(sf::milliseconds(5));
	std::cout << "Done" << std::endl;
	return 0;
}
