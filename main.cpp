#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <string>

std::vector<std::mutex> forks;
std::mutex theMutex;

void cursorPlacement(int row, int col) {
    std::cout << "\033[" << row << ";" << col << "H";
}

void philosopher(int id, int numPhilo) {
    
    int row = id + 4; //start philosopher rows below the header. have their row consistent with their id
    
    while (true) {
        //thinking
        {
            std::lock_guard<std::mutex> lock(theMutex);
            cursorPlacement(row, 0);
            std::cout << "💭 Philosopher " << id << " is THINKING.";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        //hungry
        {
            std::lock_guard<std::mutex> lock(theMutex);
            cursorPlacement(row, 0);
            std::cout << "🥪 Philosopher " << id << " is HUNGRY.";
        }

        //fork picking, lower-numbered first to avoid deadlock
        int leftFork = id;
        int rightFork = (id + 1) % numPhilo;

        if (id % 2 == 0) {
            forks[leftFork].lock();
            forks[rightFork].lock();
        } else {
            forks[rightFork].lock();
            forks[leftFork].lock();
        }

        //eating
        {
            std::lock_guard<std::mutex> lock(theMutex);
            cursorPlacement(row, 0);
            std::cout << "🍴 Philosopher " << id << " is EATING.";
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        //put down forks
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

    //clears terminal, hides cursor and puts it at the top left corner
    std::cout << "\033[2J\033[H\033[?25l";

    //header of the program
    {
        std::lock_guard<std::mutex> lock(theMutex);
        cursorPlacement(1, 0);
        std::cout << "Press Ctrl+C to exit.";
        cursorPlacement(2, 0);
        std::cout << "=============================================";
        cursorPlacement(3, 0);
        std::cout << "=============Dining Philosophers=============";
    }

    //initialization of the forks vector with the required number of mutexes
    forks = std::vector<std::mutex>(numPhilo);

    std::vector<std::thread> philosophers;

    //create and launch philosopher threads
    for (int i = 0; i < numPhilo; ++i) {
        philosophers.emplace_back(philosopher, i, numPhilo);
    }

    //wait for all philosopher threads to complete, which they never will
    for (auto& p : philosophers) {
        p.join();
    }

    //show the cursor again before exiting
    std::cout << "\033[?25h";

    return 0;
}
