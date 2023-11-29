#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <ctime>

const int EMPTY = 0;
const int NumFish = 1;
const int NumShark = 2;

const int FishBreed = 1;  // Time for a fish to breed
const int SharkBreed = 4; // Time for a shark to breed
const int sharkStarveTime = 10; // Time for a shark to starve

int world[100][100];            // array to represent the world
int fishBreedingTime[100][100]; // Array to store the time since each fish last bred

// Forward declaration
void moveSharkWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim);

void initializeWorld(int xdim, int ydim) {
    std::srand(static_cast<unsigned>(std::time(0)));

    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            world[i][k] = std::rand() % 3; // Initialize with fish, shark, or empty
            fishBreedingTime[i][k] = 0;   // Initialize fish breeding time
        }
    }
}

void moveFish(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    // Fish moves randomly to an adjacent unoccupied square
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

void moveFishWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    // Fish moves randomly to an adjacent unoccupied square
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

        // Check if the fish can breed
        if (fishBreedingTime[x][y] >= FishBreed) {
            // Breed a new fish
            int breedX, breedY;
            moveFish(x, y, breedX, breedY, xdim, ydim);
            world[breedX][breedY] = NumFish;
            fishBreedingTime[x][y] = 0; // Reset breeding time for the original fish
            fishBreedingTime[breedX][breedY] = 0; // Reset breeding time for the new fish
        } else {
            // Increment the time since last breeding
            fishBreedingTime[x][y]++;
        }
    } else {
        newX = x;
        newY = y;
    }
}

void moveShark(int x, int y, int& newX, int& newY, int& energy, int xdim, int ydim) {
    // Shark moves to an adjacent square occupied by a fish if available, or an unoccupied square
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
        // Move to a random adjacent square occupied by a fish
        int randomIndex = std::rand() % fishPositions.size();
        newX = fishPositions[randomIndex].first;
        newY = fishPositions[randomIndex].second;

        // Shark eats the fish and gains energy
        energy++;

        // Reset fish reproduction time
        world[newX][newY] = EMPTY;
    } else {
        // Move to a random unoccupied square
        moveSharkWithBreeding(x, y, newX, newY, xdim, ydim);
        // Decrease shark's energy
        energy--;

        // Check if the shark dies due to starvation
        if (energy <= 0) {
            world[x][y] = EMPTY;
            // Reset energy to 0
            energy = 0;
            return;
        }
    }
}

void moveSharkWithBreeding(int x, int y, int& newX, int& newY, int xdim, int ydim) {
    // Move shark to an adjacent unoccupied square for breeding
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

void updateWorld(int xdim, int ydim) {
    // Temporary array to store the updated world
    int newWorld[100][100];

    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            newWorld[i][k] = world[i][k]; // Copy current state to temporary array
        }
    }

    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            if (world[i][k] == NumFish) {
                int newX, newY;
                moveFishWithBreeding(i, k, newX, newY, xdim, ydim);
                newWorld[newX][newY] = NumFish;
                newWorld[i][k] = EMPTY;
            } else if (world[i][k] == NumShark) {
                int newX, newY;
                int energy = 2; // Initialize with zero
                moveShark(i, k, newX, newY, energy, xdim, ydim);

                // Check if shark can breed or dies
                if (energy >= SharkBreed) {
                    // Breed a new shark
                    newWorld[newX][newY] = NumShark;
                    // energy is not reset here
                } else if (energy <= 0) {
                    // Shark dies due to starvation (RIP)
                    newWorld[i][k] = EMPTY;
                } else {
                    newWorld[newX][newY] = NumShark;
                    newWorld[i][k] = EMPTY;
                }
            }
        }
    }

    // Update the main world array with the changes
    for (int i = 0; i < xdim; ++i) {
        for (int k = 0; k < ydim; ++k) {
            world[i][k] = newWorld[i][k];
        }
    }
}

// Function to count the fish population
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

// Function to print the breeding times (testing purposes) 
void printBreedingTimes() {
    for (int i = 0; i < 100; ++i) {
        for (int k = 0; k < 100; ++k) {
            std::cout << fishBreedingTime[i][k] << "\t";
        }
        std::cout << std::endl;
    }
}

int main() {
    int xdim = 100;
    int ydim = 100;
    int WindowXSize = 800;
    int WindowYSize = 600;
    int cellXSize = WindowXSize / xdim;
    int cellYSize = WindowYSize / ydim;

    sf::RectangleShape recArray[100][100];

    initializeWorld(xdim, ydim);

    sf::RenderWindow window(sf::VideoMode(WindowXSize, WindowYSize), "SFML Wa-Tor world");

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Output debugging information for fishies 
        std::cout << "Fish Population: " << countFishPopulation() << std::endl;
        std::cout << "Breeding Times:" << std::endl;
        printBreedingTimes();

        // Update the world
        updateWorld(xdim, ydim);

        // Create the world
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
        sf::sleep(sf::milliseconds(500)); // Creates delay
    }

    return 0;
}
