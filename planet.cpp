// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882
#include "planet.h"
#include<iostream>
#include<cassert>

// Comparator to be used in sithBattalion
bool Planet::sithComparator::operator()(Planet::Battalion sithLeft, Planet::Battalion sithRight) {
	if (sithLeft.forceSensitivity == sithRight.forceSensitivity) {
		if (sithLeft.timeStamp == sithRight.timeStamp) {
			return sithLeft.battalionID > sithRight.battalionID;
		}
		else {
			return sithLeft.timeStamp > sithRight.timeStamp;
		}
	}
	else {
		return sithLeft.forceSensitivity < sithRight.forceSensitivity;
	}
}

// Comparator to be used in jediBattalion
bool Planet::jediComparator::operator()(Planet::Battalion jediLeft, Planet::Battalion jediRight) {
	if (jediLeft.forceSensitivity == jediRight.forceSensitivity) {
		if (jediLeft.timeStamp == jediRight.timeStamp) {
			return jediLeft.battalionID > jediRight.battalionID;
		}
		else {
			return jediLeft.timeStamp > jediRight.timeStamp;
		}
	}
	else {
		return jediLeft.forceSensitivity > jediRight.forceSensitivity;
	}
}


// Counts up all surviving troops and sorts into 
// a vector where index corresponds to general.
// Side effect, destroys all data in priority queues
std::vector<int> Planet::countSurvivingTroops(std::size_t numGenerals) {
	std::vector<int> survivingTroopCount;
	survivingTroopCount.resize(numGenerals);

	while (!sithBattalions.empty()) {
		survivingTroopCount[sithBattalions.top().generalID] += sithBattalions.top().numTroops;
		sithBattalions.pop();
	}

	while (!jediBattalions.empty()) {
		survivingTroopCount[jediBattalions.top().generalID] += jediBattalions.top().numTroops;
		jediBattalions.pop();
	}

	return survivingTroopCount;
}

// make two battalions fight
void Planet::haveBattle(Planet::Battalion* sith, Planet::Battalion* jedi, bool verboseMode, bool medianMode) {
	if (verboseMode) {
		std::cout << "General " << sith->generalID << "'s battalion attacked General "
			<< jedi->generalID << "'s battalion on planet " << planetID << ". "
			<< 2 * std::min(sith->numTroops, jedi->numTroops)
			<< " troops were lost." << "\n";
	}

	if (medianMode) {
		troopsLost.push(2 * std::min(sith->numTroops, jedi->numTroops));
	}
	
	int sithNumTroops = sith->numTroops;
	int jediNumTroops = jedi->numTroops;
	sith->numTroops = std::max(0, sithNumTroops - jediNumTroops);
	jedi->numTroops = std::max(0, jediNumTroops - sithNumTroops);
	++numBattles;
}

// Updates movie watcher state machines
void Planet::updateMovieWatchers(Planet::Battalion* battalion_in, char forceType) {
	// Update attack watcher
	// interesting if sith comes after a jedi
	if (attackWatcher.state == Planet::State::Initial) { // Initial state
		if (forceType == 'J') { // Jedi Battalion
			attackWatcher.bestBattalion = *battalion_in;
			attackWatcher.state = Planet::State::SeenOnce;
		}
	}
	else if (attackWatcher.state == Planet::State::SeenOnce) { // Seen Once state
		if (forceType == 'J') { // Jedi Battalion
			if (battalion_in->forceSensitivity < attackWatcher.bestBattalion.forceSensitivity) {
				attackWatcher.bestBattalion = *battalion_in;
			}
		}
		else { // Sith Battalion
			if (battalion_in->forceSensitivity >= attackWatcher.bestBattalion.forceSensitivity) {
				attackWatcher.bestOpposingBattalion = *battalion_in;
				attackWatcher.state = Planet::State::SeenBoth;
			}
		}
	}
	else if (attackWatcher.state == Planet::State::SeenBoth) { // Seen Both state
		if (forceType == 'J') { // Jedi Battalion
			if (battalion_in->forceSensitivity < attackWatcher.bestBattalion.forceSensitivity) {
				attackWatcher.bestMaybeBattalion = *battalion_in;
				attackWatcher.state = Planet::State::MaybeBetter;
			}
		}
		else { // Sith Battalion
			if (battalion_in->forceSensitivity > attackWatcher.bestOpposingBattalion.forceSensitivity) {
				attackWatcher.bestOpposingBattalion = *battalion_in;
			}
		}
	}
	else { // Maybe better state
		if (forceType == 'J') {  // Jedi Battalion
			if (battalion_in->forceSensitivity < attackWatcher.bestMaybeBattalion.forceSensitivity) {
				attackWatcher.bestMaybeBattalion = *battalion_in;
			}
		}
		else { // Sith Battalion
			if ((battalion_in->forceSensitivity - attackWatcher.bestMaybeBattalion.forceSensitivity)
			> (attackWatcher.bestOpposingBattalion.forceSensitivity - attackWatcher.bestBattalion.forceSensitivity)) {
				attackWatcher.bestBattalion = attackWatcher.bestMaybeBattalion;
				attackWatcher.bestOpposingBattalion = *battalion_in;
				attackWatcher.state = Planet::State::SeenBoth;
			}
		}
	}

	// Update ambush watcher
	// interesting if jedi comes after a sith
	if (ambushWatcher.state == Planet::State::Initial) { // Initial state
		if (forceType == 'S') { // Sith Battalion
			ambushWatcher.bestBattalion = *battalion_in;
			ambushWatcher.state = Planet::State::SeenOnce;
		}
	}
	else if (ambushWatcher.state == Planet::State::SeenOnce) { // Seen Once state
		if (forceType == 'S') { // Sith Battalion
			if (battalion_in->forceSensitivity > ambushWatcher.bestBattalion.forceSensitivity) {
				ambushWatcher.bestBattalion = *battalion_in;
			}
		}
		else { // Jedi Battalion
			if (battalion_in->forceSensitivity <= ambushWatcher.bestBattalion.forceSensitivity) {
				ambushWatcher.bestOpposingBattalion = *battalion_in;
				ambushWatcher.state = Planet::State::SeenBoth;
			}
		}
	}
	else if (ambushWatcher.state == Planet::State::SeenBoth) { // Seen Both state
		if (forceType == 'S') { // Sith Battalion
			if (battalion_in->forceSensitivity > ambushWatcher.bestBattalion.forceSensitivity) {
				ambushWatcher.bestMaybeBattalion = *battalion_in;
				ambushWatcher.state = Planet::State::MaybeBetter;
			}
		}
		else { // Jedi Battalion
			if (battalion_in->forceSensitivity < ambushWatcher.bestOpposingBattalion.forceSensitivity) {
				ambushWatcher.bestOpposingBattalion = *battalion_in;
			}
		}
	}
	else { // Maybe better state
		if (forceType == 'S') {  // Sith Battalion
			if (battalion_in->forceSensitivity > ambushWatcher.bestMaybeBattalion.forceSensitivity) {
				ambushWatcher.bestMaybeBattalion = *battalion_in;
			}
		}
		else { // Jedi Battalion
			if ((ambushWatcher.bestMaybeBattalion.forceSensitivity - battalion_in->forceSensitivity)
			> (ambushWatcher.bestBattalion.forceSensitivity - ambushWatcher.bestOpposingBattalion.forceSensitivity)) {
				ambushWatcher.bestBattalion = ambushWatcher.bestMaybeBattalion;
				ambushWatcher.bestOpposingBattalion = *battalion_in;
				ambushWatcher.state = Planet::State::SeenBoth;
			}
		}
	}
}

void Planet::printMovieWatchers() {
	if (ambushWatcher.state == Planet::State::Initial || ambushWatcher.state == Planet::State::SeenOnce) {
		std::cout << "A movie watcher would enjoy an ambush on planet " << planetID 
			<< " with Sith at time -1 and Jedi at time -1 with a force difference of 0." << "\n";
	}
	else {
		std::cout << "A movie watcher would enjoy an ambush on planet " << planetID
			<< " with Sith at time " << ambushWatcher.bestBattalion.timeStamp << " and Jedi at time "
			<< ambushWatcher.bestOpposingBattalion.timeStamp << " with a force difference of "
			<< ambushWatcher.bestBattalion.forceSensitivity - ambushWatcher.bestOpposingBattalion.forceSensitivity
			<< "." << "\n";
	}

	if (attackWatcher.state == Planet::State::Initial || attackWatcher.state == Planet::State::SeenOnce) {
		std::cout << "A movie watcher would enjoy an attack on planet " << planetID
			<< " with Jedi at time -1 and Sith at time -1 with a force difference of 0." << "\n";
	}
	else {
		std::cout << "A movie watcher would enjoy an attack on planet " << planetID
			<< " with Jedi at time " << attackWatcher.bestBattalion.timeStamp << " and Sith at time "
			<< attackWatcher.bestOpposingBattalion.timeStamp << " with a force difference of "
			<< attackWatcher.bestOpposingBattalion.forceSensitivity - attackWatcher.bestBattalion.forceSensitivity
			<< "." << "\n";
	}
}

// Given a battalion just deployed to the planet see 
// if it wants to attack or will be ambushed
// returns true if battalion fights and survives
bool Planet::matchBattalion(char forceType, Planet::Battalion* battalion_in, bool verboseMode, bool medianMode) {
	if (forceType == 'S') { // Battalion to match is a sith
		// If no jedi, then push into pq
		if (jediBattalions.empty()) {
			sithBattalions.push(*battalion_in);
			return false;
		}

		// If are jedi, compare to top jedi candidate
		if (battalion_in->forceSensitivity >= jediBattalions.top().forceSensitivity) { // attack
			Planet::Battalion jedi = jediBattalions.top();
			jediBattalions.pop(); // take out of pqueue for battle
			haveBattle(battalion_in, &jedi, verboseMode, medianMode);

			if (jedi.numTroops > 0) { // if survive, put back into queue
				jediBattalions.push(jedi);
			}

			// If sith troops survive, add to pqueue
			if (battalion_in->numTroops > 0) {
				sithBattalions.push(*battalion_in);
				return true;
			}
			else {
				return false;
			}			
		}
		else { // sneak and wait
			sithBattalions.push(*battalion_in);
			return false;
		}
	}
	else { // Battalion to match is a jedi
		// If no sith, then push into pq
		if (sithBattalions.empty()) {
			jediBattalions.push(*battalion_in);
			return false;
		}

		// If are sith, compare to top sith candidate
		if (battalion_in->forceSensitivity <= sithBattalions.top().forceSensitivity) { // ambushed
			Planet::Battalion sith = sithBattalions.top();
			sithBattalions.pop(); // take out of pqueue for battle
			haveBattle(&sith, battalion_in, verboseMode, medianMode);

			if (sith.numTroops > 0) { // if survive, put back into queue
				sithBattalions.push(sith);
			}

			// If jedi troops survive, add to pqueue
			if (battalion_in->numTroops > 0) {
				jediBattalions.push(*battalion_in);
				return true;
			}
			else {
				return false;
			}
		}
		else { // sith don't show
			jediBattalions.push(*battalion_in);
			return false;
		}
	}
}

// carry out all battles on planet
void Planet::haveWar(bool verboseMode, bool medianMode) {
	if (jediBattalions.empty() || sithBattalions.empty()) {
		return;
	}

	while (jediBattalions.top().forceSensitivity <= sithBattalions.top().forceSensitivity) {
		Planet::Battalion jedi = jediBattalions.top();
		Planet::Battalion sith = sithBattalions.top();
		jediBattalions.pop(); // take out of pqueue for battle
		sithBattalions.pop();
		haveBattle(&sith, &jedi, verboseMode, medianMode);

		if (jedi.numTroops > 0) { // if survive, put back into queue
			jediBattalions.push(jedi);
		}
		if (sith.numTroops > 0) {
			sithBattalions.push(sith);
		}

		if (jediBattalions.empty() || sithBattalions.empty()) {
			return;
		}
	}
}