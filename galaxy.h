// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include<getopt.h>
#include "planet.h"


class Galaxy
{
public:
	Galaxy() : verboseMode(false), medianMode(false), generalEvalMode(false),
		movieWatcherMode(false), isDLMode(true), time(0), numBattles(0), 
		numBattalions(0), numPlanets(0), numGenerals(0) {}

	struct General {
		int numSithTroopsDeployed;
		int numJediTroopsDeployed;
		int numTroopsSurvived;
	};

	// Parses command line options
	void get_options(int argc, char** argv);

	// Initializes galaxy from cin
	// Returns true if DL mode false if PR mode
	void initializeGalaxy();

	// Gets next deployment of troops, puts them in proper planet
	void getDeployments();

	// log troop deployment for general mode
	void logTroopDeployment(size_t generalID, char forceType, int numTroops);

	// print out data for general mode
	void troopsSurvived();
	

private:
	std::vector<Planet> planets;
	std::vector<General> generals;
	bool verboseMode;
	bool medianMode;
	bool generalEvalMode;
	bool movieWatcherMode;
	bool isDLMode;
	int time;
	int numBattles;
	int numBattalions;
	unsigned int numPlanets;
	unsigned int numGenerals;

	const static int PLANET_NUMBER_INDEX = 10;
	
};

