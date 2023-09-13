// Server Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <time.h>

#define MAX_CLIENTS 5

// Function to check if two strings are anagrams
int areAnagrams(char *str1, char *str2) {
    int count[256] = {0};

    // If lengths of both strings are not the same, they can't be anagrams
    if (strlen(str1) != strlen(str2))
        return 0;

    // Count characters in str1
    for (int i = 0; str1[i] && str2[i]; i++) {
        count[str1[i]]++;
        count[str2[i]]--;
    }

    // If all characters are canceled out, they are anagrams
    for (int i = 0; i < 256; i++) {
        if (count[i] != 0)
            return 0;
    }

    return 1;
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    // Create socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error in socket");
        exit(1);
    }

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Choose any port you like
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the server socket
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error in bind");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(serverSocket, MAX_CLIENTS) == -1) {
        perror("Error in listen");
        exit(1);
    }

    printf("Server is listening...\n");

    while (1) {
        // Accept incoming connection
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen);
        if (clientSocket == -1) {
            perror("Error in accept");
            exit(1);
        }

        // Get current date and time
        time_t currentTime;
        struct tm *timeInfo;
        time(&currentTime);
        timeInfo = localtime(&currentTime);
        char timeString[80];
        strftime(timeString, 80, "%Y-%m-%d %H:%M:%S", timeInfo);

        printf("Connected to client at %s:%d. Current time: %s\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port), timeString);

        char str1[256], str2[256];
        // Receive two strings from the client
        recv(clientSocket, str1, sizeof(str1), 0);
        recv(clientSocket, str2, sizeof(str2), 0);

        // Check if the strings are anagrams
        int result = areAnagrams(str1, str2);

        // Send the result to the client
        send(clientSocket, &result, sizeof(int), 0);

        // Close the client socket
        close(clientSocket);

        printf("Result sent to client at %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));
    }

    // Close the server socket
    close(serverSocket);

    return 0;
}

