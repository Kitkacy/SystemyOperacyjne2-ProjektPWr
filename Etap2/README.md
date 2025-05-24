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

## Technical Requirements

- C++17
- POSIX threads
- Socket programming
