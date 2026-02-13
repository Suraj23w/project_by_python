#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 3

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    int received_numbers[MAX_CLIENTS];
    struct sockaddr_in clients[MAX_CLIENTS];
    int client_count = 0;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    while (1) {
        client_count = 0;
        printf("\nWaiting for 3 clients...\n");

        while (client_count < MAX_CLIENTS) {
            int num;
            addr_len = sizeof(client_addr);

            recvfrom(sockfd, &num, sizeof(num), 0,
                     (struct sockaddr *)&client_addr, &addr_len);

            printf("Received %d from %s:%d\n",
                   num,
                   inet_ntoa(client_addr.sin_addr),
                   ntohs(client_addr.sin_port));

            received_numbers[client_count] = num;
            clients[client_count] = client_addr;
            client_count++;
        }

        int sum = 0;
        for (int i = 0; i < MAX_CLIENTS; i++) {
            sum += received_numbers[i];
        }

        printf("Sum = %d\n", sum);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            sendto(sockfd, &sum, sizeof(sum), 0,
                   (struct sockaddr *)&clients[i],
                   sizeof(clients[i]));
        }

        printf("Result broadcasted to all clients.\n");
    }

    close(sockfd);
    return 0;
}