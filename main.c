// Urse Andrei - 312CB

#include "functions.h"

int main()
{
	// initialize system
	System *sys = initSystem();

	// operations
	executeOperations(sys);

	// free system
	destroySystem(sys);

	return 0;
}
