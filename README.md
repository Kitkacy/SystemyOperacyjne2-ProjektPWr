# Socket Chat Application

This is a simple socket-based chat application using mutexes and threads for synchronization between multiple clients.

## Features

- Multi-client support
- Real-time message broadcasting
- Thread-safe message handling with mutexes
- Support for user names
- Connection and disconnection notifications

## Building the Application

To build both the server and client applications, run:

```bash
make
```

This will compile both the server (`server`) and client (`client`) executables.

## Running the Application

1. First, start the server in one terminal:

```bash
./server
```

2. Then, start one or more clients in separate terminals:

```bash
./client
```

3. When prompted, enter your name in each client terminal.

4. Start chatting! Messages will be broadcasted to all connected clients.

5. To exit a client, type `quit` and press Enter.

## Implementation Details

- The server creates a separate thread for each connected client
- Mutexes are used to synchronize access to shared resources (message queue, client list)
- A condition variable is used to notify clients about new messages
- The client application uses one thread for receiving messages and another for sending messages

## Problems
Single-threaded server design has problems with handling communication with clients, because server process messages sequentially which causes blocking until message from  client is processed. It cannot process messages from other clients until that operation is completed. If a client becomes unresponsive or slow, it blocks the server, affecting the responsiveness for all other clients. This leads to poor scalability and performance as the number of users grows.

In multi-threaded server race conditions can happen when threads try to access shared resources, eg. the message queue, client list. These simultaneous accesses can lead to race conditions, causing:
- Data corruption (e.g., lost or duplicated messages),
- Crashes due to invalid memory access,

### Threads
Our chat server is designed to handle multiple clients concurrently. For each connected client, the server creates two separate threads. One for sending data to client and one for receiving from him. This prevents blocking I/O operations which improves responsivnes when many users uses server.

### Mutexes
To handle synchronization safely between threads and avoid race conditions, we use mutexes (std::mutex from C++ standard library) to protect all critical sections — parts of the code that access or modify shared resources.

Protected Shared Resources:
1. Message Queue:
    - Shared between all receiving threads (writing to it) and all sending threads (reading from it).
    - Without protection, simultaneous read/write operations could corrupt the queue or data.

2. Client List:
    - Shared structure that stores all active client connections.
    - Accessed during client join and disconnect.



## Technical Requirements

- C++17
- POSIX threads
- Socket programming
