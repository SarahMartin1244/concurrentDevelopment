/**
 * @file main.cpp
 * @brief This program demonstrates the dining philosophers problem using semaphores. It simulates multiple philosophers thinking, acquiring forks, eating, and releasing forks concurrently.
 *
 * 
 * @StudentName: Sarah Martin
 * @studentID: C00257967
 * @date 5/11/2023
 */


/* Commentary:
 * 
 * 
 * 
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Emacs. If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */
#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <ctime>

const int COUNT = 5; // Number of philosophers
const int THINKTIME = 3;
const int EATTIME = 5;
std::vector<Semaphore> forks(COUNT);

/*! Function to simulate a philosopher thinking */
void think(int myID) {
    int seconds = std::rand() % THINKTIME + 1;
    std::cout << myID << " is thinking!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

/*! Function for a philosopher to acquire both forks */
void get_forks(int philID) {
    forks[philID].Wait();
    forks[(philID + 1) % COUNT].Wait();
}

/*! Function for a philosopher to release both forks */
void put_forks(int philID) {
    forks[philID].Signal();
    forks[(philID + 1) % COUNT].Signal();
}

/*! Function to simulate a philosopher eating */
void eat(int myID) {
    int seconds = std::rand() % EATTIME + 1;
    std::cout << myID << " is eating!" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

/*! Function representing the behavior of a philosopher */
void philosopher(int id) {
    while (true) {
        think(id);
        get_forks(id);
        eat(id);
        put_forks(id);
    }
}

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));
    std::vector<std::thread> vt(COUNT);
    int id = 0;

    /*! Initialize all forks to 1 (unlocked) */
    for (int i = 0; i < COUNT; i++) {
        forks[i].Signal();
    }

    /*! Create and start philosopher threads */
    for (std::thread& t : vt) {
        t = std::thread(philosopher, id);
        id++;
    }
    
    /*! Create and start philosopher threads */
    for (auto& v : vt) {
        v.join();
    }
    return 0;
}

/* main.c ends here */

