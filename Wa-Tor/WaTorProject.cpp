#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>
#include <omp.h>
/**
 * @StudentName: Sarah Martin
 * @studentID C00257967
 * @date November 30th, 2023
 */
 
 
/*! \fn Wa-Tor Project 
    \brief An Implementation of Wa-Tor Simulation 

The Wa-Tor project is a simulation of a predator/prey ecosystem, featuring fish and sharks, with a focus on their interactions and behaviors within an environment. This implementation uses C++11 features to create a parallelized simulation using OpenMP.

The main components of the simulation include fish, sharks, and an ocean grid. Fish move randomly within the grid, breed when a specified breeding time is reached, and are subject to predation by sharks. Sharks, on the other hand, hunt and consume fish, breed when their energy level allows it, and can starve if their energy level runs out.

*/

/*! @brief Value representing an empty cell */
const int EMPTY = 0;

/*! @brief Initial fish population. */
const int NumFish = 2;

/*! @brief Initial shark population. */
const int NumShark = 10;

/*! @brief Time for a fish to breed. */
const int FishBreed = 20;

/*! @brief Time for a shark to breed. */
const int SharkBreed = 2;

/*! @brief Time for a shark to starve. */
const int sharkStarveTime = 10;



int world[100][100];
int fishBreedingTime[100][100];


// Function to move a shark considering breeding and starvation
void moveSharkWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim);




/** @brief Initialize the world with fish based on a given percentage.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 * @param fishPercentage Percentage of cells to be initialized with fish.
 */
void initializeWorld(int xdim, int ydim, int fishPercentage) {

    std::srand(static_cast<unsigned>(std::time(0)));

    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            if (std::rand() % 100 < fishPercentage) {
                world[i][k] = NumFish;
            } else {
                world[i][k] = EMPTY;
            }
            fishBreedingTime[i][k] = 0;
        }
    }
}

/** @brief Move a fish randomly in an empty direction.
 * @param x Current x-coordinate of the fish.
 * @param y Current y-coordinate of the fish.
 * @param[out] newX New x-coordinate after the move.
 * @param[out] newY New y-coordinate after the move.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 */
void moveFish(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<int> possibleMoves;

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim && world[newX][newY] == EMPTY) {
            possibleMoves.push_back(i);
        }
    }

    if (!possibleMoves.empty()) {
        int randomIndex = std::rand() % possibleMoves.size();
        newX = x + directions[possibleMoves[randomIndex]][0];
        newY = y + directions[possibleMoves[randomIndex]][1];
    } else {
        newX = x;
        newY = y;
    }
}

/** @brief Move a fish considering breeding and escaping from sharks.
 * @param x Current x-coordinate of the fish.
 * @param y Current y-coordinate of the fish.
 * @param[out] newX New x-coordinate after the move.
 * @param[out] newY New y-coordinate after the move.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 */
void moveFishWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<int> possibleMoves;
    std::vector<int> escapeMoves; // vector for escape moves

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim) {
            if (world[newX][newY] == EMPTY) {
                possibleMoves.push_back(i);
            } else if (world[newX][newY] == NumShark) {
                // If a shark is nearby, consider it as an escape move
                escapeMoves.push_back(i);
            }
        }
    }

    if (!possibleMoves.empty()) {
        int randomIndex = std::rand() % possibleMoves.size();
        newX = x + directions[possibleMoves[randomIndex]][0];
        newY = y + directions[possibleMoves[randomIndex]][1];

        if (fishBreedingTime[x][y] >= FishBreed) {
            int breedX, breedY;
            moveFish(x, y, breedX, breedY, xdim, ydim);
            world[breedX][breedY] = NumFish;
            fishBreedingTime[x][y] = 0;
            fishBreedingTime[breedX][breedY] = 0;
        } else {
            fishBreedingTime[x][y]++;
        }
    } else if (!escapeMoves.empty()) {
        // If there are escape moves, move randomly to escape from sharks
        int randomIndex = std::rand() % escapeMoves.size();
        newX = x + directions[escapeMoves[randomIndex]][0];
        newY = y + directions[escapeMoves[randomIndex]][1];
    } else {
        newX = x;
        newY = y;
    }
}



/** @brief Move a shark based on its behavior (hunt fish and breed).
 * @param x Current x-coordinate of the shark.
 * @param y Current y-coordinate of the shark.
 * @param[out] newX New x-coordinate after the move.
 * @param[out] newY New y-coordinate after the move.
 * @param[out]  Energy level of the shark.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 */
void moveShark(int x, int y, int& newX, int& newY, int& energy, int xdim, int ydim) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<std::pair<int, int>> fishPositions;

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim) {
            if (world[newX][newY] == NumFish) {
                fishPositions.push_back(std::make_pair(newX, newY));
            }
        }
    }

    if (!fishPositions.empty()) {
        int randomIndex = std::rand() % fishPositions.size();
        newX = fishPositions[randomIndex].first;
        newY = fishPositions[randomIndex].second;
        energy++;
        world[newX][newY] = EMPTY;
    } else {
        moveSharkWithBreeding(x, y, newX, newY, xdim, ydim);
        energy--;

        if (energy <= 0) {
            world[x][y] = EMPTY;
            energy = 0;
            // for debugging
          //  std::cout << "Shark at (" << x << ", " << y << ") died due to lack of energy." << std::endl;

            return;
        }

        // Debugging information for sharks
       // std::cout << "Shark at (" << x << ", " << y << ") moved to (" << newX << ", " << newY << ") with energy " << energy << std::endl;
    }
}


/** @brief Move a shark considering breeding and empty spaces.
 * @param x Current x-coordinate of the shark.
 * @param y Current y-coordinate of the shark.
 * @param[out] newX New x-coordinate after the move.
 * @param[out] newY New y-coordinate after the move.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 */
void moveSharkWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    int directions[4][2] = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    std::vector<int> possibleMoves;

    for (int i = 0; i < 4; ++i) {
        int newX = x + directions[i][0];
        int newY = y + directions[i][1];

        if (newX >= 0 && newX < xdim && newY >= 0 && newY < ydim && world[newX][newY] == EMPTY) {
            possibleMoves.push_back(i);
        }
    }

    if (!possibleMoves.empty()) {
        int randomIndex = std::rand() % possibleMoves.size();
        newX = x + directions[possibleMoves[randomIndex]][0];
        newY = y + directions[possibleMoves[randomIndex]][1];
    } else {
        newX = x;
        newY = y;
    }
}


/** @brief Count the current shark population in the world.
 * @return Current shark population.
 */
int countSharkPopulation() {
    int sharkCount = 0;
    for (int i = 0; i < 100; ++i) {
        for (int k = 0; k < 100; ++k) {
            if (world[i][k] == NumShark) {
                sharkCount++;
            }
        }
    }
    return sharkCount;
}


/** @brief Update the world for one iteration of the simulation.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 */
void updateWorld(int xdim, int ydim) {
    int newWorld[100][100];

    #pragma omp parallel for shared(newWorld, world, fishBreedingTime) collapse(2)
    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            if (world[i][k] == NumFish) {
                int newX, newY;
                moveFishWithBreeding(i, k, newX, newY, xdim, ydim);
                newWorld[newX][newY] = NumFish;
                newWorld[i][k] = EMPTY;
            } else if (world[i][k] == NumShark) {
                int newX, newY;
                int energy = 2;
                moveShark(i, k, newX, newY, energy, xdim, ydim);

                if (energy >= SharkBreed) {
                    newWorld[newX][newY] = NumShark;
                } else if (energy <= 0) {
                    newWorld[i][k] = EMPTY;
                } else {
                    newWorld[newX][newY] = NumShark;
                    newWorld[i][k] = EMPTY;
                }
            }
        }
    }

    // Debugging information for sharks
   /* #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Number of Sharks: " << countSharkPopulation() << std::endl;
        }
    } */

    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            world[i][k] = newWorld[i][k];
        }
    }
}

/** @brief Count the current fish population in the world.
 * @return Current fish population.
 */
int countFishPopulation() {
    int fishCount = 0;
    for (int i = 0; i < 100; ++i) {
        for (int k = 0; k < 100; ++k) {
            if (world[i][k] == NumFish) {
                fishCount++;
            }
        }
    }
    return fishCount;
}

/** @brief Print the breeding times of fish for debugging info. */
void printBreedingTimes() {
    for (int i = 0; i < 100; ++i) {
        for (int k = 0; k < 100; ++k) {
            std::cout << fishBreedingTime[i][k] << "\t";
        }
        std::cout << std::endl;
    }
}

/** @brief Measure the execution time of the simulation with different thread counts.
 * @param xdim X-dimension of the world grid.
 * @param ydim Y-dimension of the world grid.
 * @param numThreads Number of threads to use for parallel execution.
 * @return Execution time of the simulation.
 */
double measureExecutionTime(int xdim, int ydim, int numThreads) {
    omp_set_num_threads(numThreads);

    double startTime = omp_get_wtime();

    int counter = 0;
    int maxIterations = 1000; // number of iterations

    while (counter < maxIterations) {
        updateWorld(xdim, ydim);
        counter++;
    }

    double endTime = omp_get_wtime();

    return endTime - startTime;
}


/**
 * @brief Main function of the Wa-Tor simulation.
 */
int main() {
 // Constants and variables for window dimensions, cell sizes, and SFML setup
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 600;
    int cellXSize = WindowXSize / xdim;
    int cellYSize = WindowYSize / ydim;

    sf::RectangleShape recArray[100][100];

 // Initialize the world with fish (40% of cells, had an issue with fish populating every cell)
    initializeWorld(xdim, ydim, 40);

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Output debugging information
        // std::cout << "Fish Population: " << countFishPopulation() << std::endl;
        // std::cout << "Breeding Times:" << std::endl;
        // printBreedingTimes();

        // Update the world
        updateWorld(xdim, ydim);

        // Render the world
        window.clear(sf::Color::White); // Set the background color to white

        for (int i = 0; i < xdim; ++i) {
            for (int k = 0; k < ydim; ++k) {
                if (world[i][k] == NumFish) {
                    recArray[i][k].setFillColor(sf::Color::Green);
                } else if (world[i][k] == NumShark) {
                    recArray[i][k].setFillColor(sf::Color::Red);
                } else {
                    recArray[i][k].setFillColor(sf::Color::Blue);
                }

                recArray[i][k].setPosition(i * cellXSize, k * cellYSize);
                recArray[i][k].setSize(sf::Vector2f(cellXSize, cellYSize));
                window.draw(recArray[i][k]);
            }
        }

        window.display();
         sf::sleep(sf::milliseconds(500)); 
    }

  // thread counts for performance measurement
    int threadCounts[] = {1, 2, 4, 8, 16};

    std::vector<double> executionTimes;

    for (size_t i = 0; i < sizeof(threadCounts) / sizeof(threadCounts[0]); ++i) {
        int numThreads = threadCounts[i];
        double executionTime = measureExecutionTime(xdim, ydim, numThreads);
        executionTimes.push_back(executionTime);
    }

    std::cout << "Number of Threads\tExecution Time (s)\tSpeedup\n";
    double baseTime = executionTimes[0];

  // Measure and record execution times for different thread counts
    for (size_t i = 0; i < executionTimes.size(); ++i) {
        int numThreads = threadCounts[i];
        double executionTime = executionTimes[i];
        double speedup = baseTime / executionTime;
        std::cout << numThreads << "\t\t\t" << executionTime << "\t\t\t" << speedup << "\n";
    }

    return 0;
}

