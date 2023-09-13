// Client Code

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;

    // Create socket
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error in socket");
        exit(1);
    }

    // Initialize server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345); // Use the same port as the server
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Replace with the server's IP address

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error in connect");
        exit(1);
    }

    char str1[256], str2[256];

    // Prompt the user to enter two strings
    printf("Enter the first string: ");
    scanf("%s", str1);
    printf("Enter the second string: ");
    scanf("%s", str2);

    // Send the strings to the server
    send(clientSocket, str1, sizeof(str1), 0);
    send(clientSocket, str2, sizeof(str2), 0);

    int result;

    // Receive the result from the server
    recv(clientSocket, &result, sizeof(int), 0);

    if (result)
        printf("The strings are anagrams.\n");
    else
        printf("The strings are not anagrams.\n");

    // Close the client socket
    close(clientSocket);

    return 0;
}

