// Copyright 2025 Wincenty Wensker Dawid Łapiński
//
// Client side implementation of socket-based chat application.
// This file contains the client code that connects to the server,
// sends and receives messages, and handles user input.

#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <cstring>
#include <atomic>

#define PORT 8080
#define BUFFER_SIZE 1024
#define SERVER_IP "127.0.0.1"

// Mutex for synchronizing console output
std::mutex console_mutex;
// Flag to control thread execution
std::atomic<bool> running(true);

// Function to receive messages from the server
void receive_messages(int sock_fd) {
    char buffer[BUFFER_SIZE];
    
    while (running) {
        memset(buffer, 0, sizeof(buffer));
        int bytes_read = recv(sock_fd, buffer, BUFFER_SIZE, 0);
        
        if (bytes_read <= 0) {
            // Server disconnected or error
            std::lock_guard<std::mutex> lock(console_mutex);
            std::cout << "\nDisconnected from server." << std::endl;
            running = false;
            break;
        }
        
        // Print the received message
        std::lock_guard<std::mutex> lock(console_mutex);
        std::cout << "\r" << buffer << "\n> " << std::flush;
    }
}

int main() {
    int sock_fd = 0;
    struct sockaddr_in serv_addr;
    
    // Create socket
    if ((sock_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return -1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    // Convert IPv4 address from text to binary form
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address / Address not supported" << std::endl;
        return -1;
    }
    
    // Connect to server
    if (connect(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection failed" << std::endl;
        return -1;
    }
    
    std::cout << "Connected to chat server at " << SERVER_IP << ":" << PORT << std::endl;
    
    // Get user's name
    std::string name;
    std::cout << "Enter your name: ";
    std::getline(std::cin, name);
    
    // Send name to server
    send(sock_fd, name.c_str(), name.length(), 0);
    
    // Start thread to receive messages
    std::thread receive_thread(receive_messages, sock_fd);
    
    std::string message;
    std::cout << "Start chatting (type 'quit' to exit):" << std::endl;
    
    while (running) {
        std::cout << "> ";
        std::getline(std::cin, message);
        
        if (message == "quit") {
            running = false;
            break;
        }
        
        // Send message to server
        send(sock_fd, message.c_str(), message.length(), 0);
    }
    
    // Clean up
    close(sock_fd);
    
    if (receive_thread.joinable()) {
        receive_thread.join();
    }
    
    std::cout << "Disconnected from chat server. Goodbye!" << std::endl;
    
    return 0;
}
