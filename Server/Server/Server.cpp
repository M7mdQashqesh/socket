#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET listeningSocket, clientSocket;
    struct sockaddr_in server, client;
    int c, recv_size;
    char buffer[2000];

    // Create a Winsock
    cout << "Initializing Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed: " << WSAGetLastError();
        return 1;
    }
    cout << "Initialized." << endl;

    // Create a socket
    if ((listeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Could not create socket: " << WSAGetLastError();
        return 1;
    }
    cout << "Socket created." << endl;

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(8888);

    // Bind
    if (bind(listeningSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        cerr << "Bind failed: " << WSAGetLastError();
        return 1;
    }
    cout << "Bind done." << endl;

    // Listen to incoming connections
    listen(listeningSocket, 3);
    cout << "Waiting for incoming connections..." << endl;

    c = sizeof(struct sockaddr_in);
    clientSocket = accept(listeningSocket, (struct sockaddr*)&client, &c);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Accept failed: " << WSAGetLastError();
        return 1;
    }
    cout << "Connection accepted. You can start chatting now." << endl;

    // This loop allows for continuous bidirectional communication
    while (true) {
        ZeroMemory(buffer, 2000); // Clear the buffer
        // Receive a message from client
        recv_size = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
        if (recv_size == SOCKET_ERROR) {
            cerr << "recv failed: " << WSAGetLastError() << endl;
            break;
        }
        if (recv_size == 0) {
            cout << "Client disconnected." << endl;
            break;
        }
        buffer[recv_size] = '\0';
        cout << "Client: " << buffer << endl;

        if (strcmp(buffer, "exit") == 0) {
            cout << "Exit command received. Shutting down." << endl;
            break; // Exit the loop
        }

        // Send a reply to the client
        cout << "Server: ";
        string reply;
        getline(cin, reply);
        send(clientSocket, reply.c_str(), reply.length(), 0);

        if (reply == "exit") {
            cout << "Exit command typed. Shutting down." << endl;
            send(clientSocket, reply.c_str(), reply.length(), 0); // Optionally notify the server
            break; // Exit the loop
        }
    }


    // Cleanup
    closesocket(clientSocket);
    closesocket(listeningSocket);
    WSACleanup();

    system("pause");
    return 0;
}
