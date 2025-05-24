// Copyright 2025 Wincenty Wensker

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>

std::vector<std::mutex> forks;
std::mutex mutexForTerm;

void cursorPlacement(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

void philosopher(int id, int numPhilo) {
    int row = id + 6;  // Start philosopher rows below the header, consistent with their id

    while (true) {
        // Thinking
        {
            std::lock_guard<std::mutex> lock(mutexForTerm);
            cursorPlacement(row, 0);
            std::cout << "💭 Philosopher " << id << " is THINKING.";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Hungry
        {
            std::lock_guard<std::mutex> lock(mutexForTerm);
            cursorPlacement(row, 0);
            std::cout << "🥪 Philosopher " << id << " is HUNGRY.  ";
        }

        // Fork picking, lower-numbered first to avoid deadlock
        int leftFork = id;
        int rightFork = (id + 1) % numPhilo;

        if (id % 2 == 0) {
            forks[leftFork].lock();
            forks[rightFork].lock();
        } else {
            forks[rightFork].lock();
            forks[leftFork].lock();
        }

        // Eating
        {
            std::lock_guard<std::mutex> lock(mutexForTerm);
            cursorPlacement(row, 0);
            std::cout << "🍴 Philosopher " << id << " is EATING.  ";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        // Put down forks
        forks[leftFork].unlock();
        forks[rightFork].unlock();
    }
}

int main(int argc, char* argv[]) {
    int numPhilo = std::stoi(argv[1]);

    if (numPhilo < 2) {
        std::cerr << "Number of philosophers must be at least 2.\n";
        return 1;
    }

    // Clears terminal, hides cursor, and puts it at the top left corner
    std::cout << "\033[2J\033[H\033[?25l";

    // Header of the program
    {
        std::lock_guard<std::mutex> lock(mutexForTerm);
        cursorPlacement(1, 0);
        std::cout << "Press Ctrl+C to exit.";
        cursorPlacement(2, 0);
        std::cout << "=============================================";
        cursorPlacement(3, 0);
        std::cout << "=============Dining Philosophers=============";
        cursorPlacement(4, 0);
        std::cout << "=============================================";
        cursorPlacement(5, 0);
        std::cout << "                                             ";
    }

    // Initialization of the forks vector with the required number of mutexes
    forks = std::vector<std::mutex>(numPhilo);

    std::vector<std::thread> philosophers;

    // Create and launch philosopher threads
    for (int i = 0; i < numPhilo; ++i) {
        philosophers.emplace_back(philosopher, i, numPhilo);
    }

    // Wait for all philosopher threads to complete, which they never will
    for (auto& p : philosophers) {
        p.join();
    }

    // Clear the screen and show the cursor again before exiting
    std::cout << "\033[2J\033[H\033[?25h";

    return 0;
}
