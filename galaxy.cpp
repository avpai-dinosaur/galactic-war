// Project identifier: AD48FB4835AF347EB0CA8009E24C3B13F8519882

#include<iostream>
#include<sstream>
#include<string>
#include "galaxy.h"
#include "P2random.h"

// Parses command line options
void Galaxy::get_options(int argc, char** argv) {
    int option_index = 0, option = 0;

    // Don't display getopt error messages about options
    opterr = false;

    // use getopt to find command line options
    struct option longOpts[] = {
        {"verbose", no_argument, nullptr, 'v'},
        {"median", no_argument, nullptr, 'm'},
        {"general-eval", no_argument, nullptr, 'g'},
        {"watcher", no_argument, nullptr, 'w'},
        { nullptr, 0, nullptr, '\0' } };

    while ((option = getopt_long(argc, argv, "vmgw", longOpts, &option_index)) != -1) {
        switch (option) {
        case 'v':
            verboseMode = true;
            break;

        case 'm':
            medianMode = true;
            break;

        case 'g':
            generalEvalMode = true;
            break;

        case 'w':
            movieWatcherMode = true;
            break;

        default:
            std::cerr << "Unknown command line option" << std::endl;
            exit(1);
        }
    }
}

// Initializes galaxy from cin
// Returns true if DL mode false if PR mode
void Galaxy::initializeGalaxy() {
    // general string to store input junk
    std::string commentJunk;
    
    // Skip first comment line
    getline(std::cin, commentJunk);

    // Get mode
    char mode; // Stores first letter of mode 
    std::cin >> commentJunk >> mode;
    getline(std::cin, commentJunk); // Finish line

    // Get number of generals
    std::cin >> commentJunk >> numGenerals;
    getline(std::cin, commentJunk);
    generals.resize(numGenerals);
    for (size_t i = 0; i < numGenerals; ++i) {
        generals[i] = { 0, 0, 0 };
    }

    // Get number of planets
    std::cin >> commentJunk >> numPlanets;
    getline(std::cin, commentJunk);
    planets.resize(numPlanets);
    for (size_t i = 0; i < planets.size(); ++i) {
        planets[i].setPlanetID(i);
    }

    // Print startup message
    std::cout << "Deploying troops..." << "\n";

    if (mode == 'D') { // DL mode
        isDLMode = true;
    }
    else { // PR mode
        isDLMode = false;
    }
}


void Galaxy::logTroopDeployment(size_t generalID, char forceType, int numTroops) {
    if (forceType == 'S') {
        generals[generalID].numSithTroopsDeployed += numTroops;
    }
    else {
        generals[generalID].numJediTroopsDeployed += numTroops;
    }
}

// print out data for general mode
void Galaxy::troopsSurvived() {
    for (size_t i = 0; i < planets.size(); ++i) {
        std::vector<int> survivingTroops;
        survivingTroops = planets[i].countSurvivingTroops(numGenerals);

        for (size_t i = 0; i < survivingTroops.size(); ++i) {
            generals[i].numTroopsSurvived += survivingTroops[i];
        }
    }
    
    for (size_t i = 0; i < generals.size(); ++i) {
        std::cout << "General " << i << " deployed " << generals[i].numJediTroopsDeployed
            << " Jedi troops and " << generals[i].numSithTroopsDeployed
            << " Sith troops, and " << generals[i].numTroopsSurvived << "/"
            << generals[i].numJediTroopsDeployed + generals[i].numSithTroopsDeployed
            << " troops survived." << "\n";
    }
}


// Gets next deployment of troops, puts them in proper planet
void Galaxy::getDeployments() {
    std::stringstream ss;
    // inputMode is the "PR" or "DL" from line 2
    if (!isDLMode) {
        std::string commentJunk;

        int seed;
        std::cin >> commentJunk >> seed;
        getline(std::cin, commentJunk);

        int num_deploys;
        std::cin >> commentJunk >> num_deploys;
        getline(std::cin, commentJunk);

        int rate;
        std::cin >> commentJunk >> rate;
        getline(std::cin, commentJunk);

        P2random::PR_init(ss, seed, numGenerals, numPlanets, num_deploys, rate);
    }
    
    // Create a reference variable that is ALWAYS used for reading input.
    // If PR mode is on, refer to the stringstream. Otherwise, refer to cin.
    // This is a place where the ternary operator must be used: an equivalent
    // if/else is impossible because reference variables must be initialized
    // when they are created.
    std::istream& inputStream = isDLMode ? std::cin : ss;

    //Read an entire deployment
    int time_in;
    char junk;
    char forceType;
    std::size_t generalID;
    std::size_t planetNumber;
    int forceSensitivity;
    int numTroops;
    while (inputStream >> time_in >> forceType >> junk >> junk >> junk >> junk 
        >> generalID >> junk >> planetNumber >> junk >> forceSensitivity >> junk >> numTroops) {

        // DL input error checking
        if (planetNumber >= planets.size()) {
            std::cerr << "Invalid planet ID" << "\n";
            exit(1);
        }

        if (generalID >= generals.size()) {
            std::cerr << "Invalid general ID" << "\n";
            exit(1);
        }

        if (forceSensitivity < 1) {
            std::cerr << "Invalid force sensitivity level" << "\n";
            exit(1);
        }

        if (numTroops < 1) {
            std::cerr << "Invalid number of troops" << "\n";
            exit(1);
        }

        if (time_in < time) {
            std::cerr << "Invalid decreasing timestamp" << "\n";
            exit(1);
        }

        ++numBattalions;

        logTroopDeployment(generalID, forceType, numTroops);
        
        // enter if current line read in is in different tick
        if (time_in != time) {
            for (size_t i = 0; i < planets.size(); ++i) {
                planets[i].haveWar(verboseMode, medianMode);
            }

            if (medianMode) {
                for (size_t i = 0; i < planets.size(); ++i) {
                    if (planets[i].getMedianNumTroopsLost() == 0) {
                        continue;
                    }

                    std::cout << "Median troops lost on planet " << i
                        << " at time " << time << " is "
                        << planets[i].getMedianNumTroopsLost() << "." << "\n";
                }
            }

            time = time_in;
        }
        
        Planet::Battalion battalion_in = { numTroops, forceSensitivity, time_in, generalID, numBattalions };
        if (movieWatcherMode) {
            planets[planetNumber].updateMovieWatchers(&battalion_in, forceType);
        }

        if (planets[planetNumber].matchBattalion(forceType, &battalion_in, verboseMode, medianMode)) {
            planets[planetNumber].haveWar(verboseMode, medianMode);
        }
    }

    // Exit while loop and carry out all final battles
    std::size_t totalBattles = 0;
    for (size_t i = 0; i < planets.size(); ++i) {
        planets[i].haveWar(verboseMode, medianMode);
        totalBattles += planets[i].getNumBattles();
    }

    if (medianMode) {
        for (size_t i = 0; i < planets.size(); ++i) {
            if (planets[i].getMedianNumTroopsLost() == 0) {
                continue;
            }

            std::cout << "Median troops lost on planet " << i
                << " at time " << time << " is "
                << planets[i].getMedianNumTroopsLost() << "." << "\n";
        }
    }


    std::cout << "---End of Day---" << "\n";
    std::cout << "Battles: " << totalBattles << "\n";

    if (generalEvalMode) {
        std::cout << "---General Evaluation---" << "\n";
        troopsSurvived();
    }

    if (movieWatcherMode) {
        std::cout << "---Movie Watcher---" << "\n";
        for (size_t i = 0; i < planets.size(); ++i) {
            planets[i].printMovieWatchers();
        }
    }

}
