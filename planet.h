// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include<queue>
#include "medianQueue.h"

class Planet
{
public:
	Planet() : planetID(0), numBattles(0) {
		attackWatcher.state = Initial;
		attackWatcher.bestForceDifference = 0;
		ambushWatcher.state = Initial;
		ambushWatcher.bestForceDifference = 0;
	}

	struct Battalion
	{
		int numTroops;
		int forceSensitivity;
		int timeStamp;
		std::size_t generalID;
		int battalionID;
	};

	enum State
	{
		Initial,
		SeenOnce,
		SeenBoth, 
		MaybeBetter
	};

	struct MovieWatcher 
	{
		Battalion bestBattalion;
		Battalion bestOpposingBattalion;
		Battalion bestMaybeBattalion;
		State state;
		int bestForceDifference;
	};

	void setPlanetID(std::size_t ID_in) {
		planetID = ID_in;
	}

	// Comparator to be used in sithBattalion
	class sithComparator {
	public:
		bool operator()(Battalion sithLeft, Battalion sithRight);
	};

	// Comparator to be used in jediBattalion
	class jediComparator {
	public:
		bool operator()(Battalion jediLeft, Battalion jediRight);
	};

	std::size_t getNumBattles() { return numBattles; }

	int getMedianNumTroopsLost() { return troopsLost.getMedian(); }

	// Counts up all surviving troops and sorts into 
	// a vector where index corresponds to general.
	// Side effect, destroys all data in priority queues
	std::vector<int> countSurvivingTroops(std::size_t numGenerals);

	// Given a battalion just deployed to the planet see 
	//if it wants to attack or will be ambushed
	// returns true if battalion fights and survives
	bool matchBattalion(char foceType, Battalion *battalion_in, bool verboseMode, bool medianMode);

	// Updates movie watcher state machines
	void updateMovieWatchers(Battalion* battalion_in, char forceType);

	// Prints output for movie watchers for given planet
	void printMovieWatchers();

	// make two battalions fight
	void haveBattle(Battalion* sith, Battalion* jedi, bool verboseMode, bool medianMode);

	// carry out all battles on planet
	void haveWar(bool verboseMode, bool medianMode);

private:
	std::size_t planetID;
	int numBattles;
	MovieWatcher attackWatcher;
	MovieWatcher ambushWatcher;
	medianQueue troopsLost;
	std::priority_queue<Battalion, std::vector<Battalion>, sithComparator> sithBattalions;
	std::priority_queue<Battalion, std::vector<Battalion>, jediComparator> jediBattalions;
};