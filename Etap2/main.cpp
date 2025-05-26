// Copyright 2025 Wincenty Wensker Dawid Łapiński
//
// Server side implementation of socket-based chat application.
// This file contains the server code that handles multiple client connections,
// broadcasts messages, and manages client threads.

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <algorithm>
#include <map>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// ANSI color codes for terminal output
const std::string COLORS[] = {
    // Red
    "\033[1;31m", 
    // Green
    "\033[1;32m", 
    // Yellow
    "\033[1;33m", 
    // Blue
    "\033[1;34m", 
    // Magenta
    "\033[1;35m", 
    // Cyan
    "\033[1;36m", 
};
const std::string RESET_COLOR = "\033[0m";
const int NUM_COLORS = sizeof(COLORS) / sizeof(COLORS[0]);

// Mutex for synchronizing access to messages
std::mutex message_mutex;
// Condition variable for signaling new messages
std::condition_variable message_cv;
// Vector to store all messages
std::vector<std::string> messages;
// Vector to store client sockets
std::vector<int> client_sockets;
// Mutex for synchronizing access to client_sockets
std::mutex clients_mutex;
// Map to store user colors
std::map<std::string, std::string> user_colors;
// Mutex for synchronizing access to user_colors
std::mutex color_mutex;
// Current color index
int color_index = 0;

// Function to broadcast a message to all clients
void broadcast_message(const std::string& message) {
    std::lock_guard<std::mutex> lock(message_mutex);
    messages.push_back(message);
    // Notify all waiting threads
    message_cv.notify_all(); 

// Function to handle a client connection
void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    std::string name;
    std::string user_color;
    
    // Get client's name
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return;
    }
    
    buffer[bytes_read] = '\0';
    name = buffer;
    
    // Assign a color to the user
    {
        std::lock_guard<std::mutex> lock(color_mutex);
        user_color = COLORS[color_index];
        user_colors[name] = user_color;
        // Increment and cycle color index
        color_index = (color_index + 1) % NUM_COLORS;
    }
    
    // Announce new client
    std::string welcome_message = user_color + name + RESET_COLOR + " has joined the chat!";
    
    broadcast_message(welcome_message);
    std::cout << welcome_message << std::endl;
    
    // Create a thread to send messages to this client
    std::thread send_thread([client_socket, name]() {
        std::unique_lock<std::mutex> lock(message_mutex);
        size_t last_message_index = 0;
        
        while (true) {
            // Wait until there's a new message
            message_cv.wait(lock, [&]() { return messages.size() > last_message_index; });
            
            // Send all new messages to the client
            while (last_message_index < messages.size()) {
                std::string message = messages[last_message_index];
                send(client_socket, message.c_str(), message.length(), 0);
                last_message_index++;
            }
        }
    });
    
    // Main thread receives messages from the client
    while (true) {
        bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
        
        if (bytes_read <= 0) {
            // Client disconnected
            break;
        }
        
        buffer[bytes_read] = '\0';
        std::string color = user_colors[name];
        
        // Format the message with color
        std::string colored_message = color + name + RESET_COLOR + ": " + buffer;
        
        broadcast_message(colored_message);
        std::cout << colored_message << std::endl;
    }
    
    // Client disconnected
    std::string color = user_colors[name];
    std::string disconnect_message = color + name + RESET_COLOR + " has left the chat.";
    
    broadcast_message(disconnect_message);
    std::cout << disconnect_message << std::endl;
    
    // Remove client socket from the list
    {
        std::lock_guard<std::mutex> lock(clients_mutex);
        auto it = std::find(client_sockets.begin(), client_sockets.end(), client_socket);
        if (it != client_sockets.end()) {
            client_sockets.erase(it);
        }
    }
    
    close(client_socket);
    send_thread.detach(); // Detach the send thread
}

int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    
    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    // Bind the socket to the specified port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    std::cout << "Server started on port " << PORT << std::endl;
    std::cout << "Waiting for connections..." << std::endl;
    
    std::vector<std::thread> client_threads;
    
    while (true) {
        int new_socket;
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Add client socket to the list
        {
            std::lock_guard<std::mutex> lock(clients_mutex);
            client_sockets.push_back(new_socket);
        }
        
        // Create a new thread to handle this client
        std::thread client_thread(handle_client, new_socket);
        client_threads.push_back(std::move(client_thread));
    }
    
    // Clean up
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    close(server_fd);
    
    return 0;
}