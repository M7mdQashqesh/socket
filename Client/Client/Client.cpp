#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>
using namespace std;
#pragma comment(lib, "ws2_32.lib") // Link with ws2_32.lib

int main() {
    WSADATA wsaData;
    SOCKET serverSocket;
    struct sockaddr_in server;
    char server_reply[2000];
    string message;
    int recv_size;

    cout << "Initializing Winsock..." << endl;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cerr << "WSAStartup failed: " << WSAGetLastError();
        return 1;
    }
    cout << "Initialized." << endl;

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        cerr << "Could not create socket: " << WSAGetLastError();
        return 1;
    }
    cout << "Socket created." << endl;

    // Setup the server structure
    server.sin_family = AF_INET;
    server.sin_port = htons(8888);
    if (inet_pton(AF_INET, "127.0.0.1", &server.sin_addr) <= 0) {  // Replace "172.0.0.1 to the server ip
        cerr << "Invalid address/ Address not supported" << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Connect to the server
    if (connect(serverSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        cerr << "Connect error: " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    cout << "Connected to server. You can start chatting now." << endl;

    // Communication loop
    while (true) {
        cout << "You: ";
        getline(cin, message); // Get input from the user

        // Send the message to the server
        if (send(serverSocket, message.c_str(), message.length(), 0) < 0) {
            cerr << "Send failed" << endl;
            break;
        }

        // Receive a reply from the server
        ZeroMemory(server_reply, 2000); // Clear the buffer
        if ((recv_size = recv(serverSocket, server_reply, sizeof(server_reply) - 1, 0)) == SOCKET_ERROR) {
            cerr << "recv failed: " << WSAGetLastError() << endl;
            break;
        }
        server_reply[recv_size] = '\0'; // Null-terminate the reply
        cout << "Server: " << server_reply << endl;
    }

    // Cleanup
    closesocket(serverSocket);
    WSACleanup();


    system("pause");
    return 0;
}