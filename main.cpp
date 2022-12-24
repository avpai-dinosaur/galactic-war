// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include<iostream>
#include<string>
#include "galaxy.h"

using namespace std;

int main(int argc, char** argv) {
	// Turn off synchronized I\O
	std::ios_base::sync_with_stdio(false);

	Galaxy galaxy; // create galaxy object
	galaxy.get_options(argc, argv); // parse command line options
	galaxy.initializeGalaxy(); // initialize galaxy
	galaxy.getDeployments();

	return 0;
}