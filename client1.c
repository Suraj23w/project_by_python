#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 12345
#define SERVER_IP "127.0.0.1"

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int number, sum;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    printf("Client 1: Enter a number: ");
    scanf("%d", &number);

    sendto(sockfd, &number, sizeof(number), 0,
           (struct sockaddr *)&server_addr, sizeof(server_addr));

    socklen_t addr_len = sizeof(server_addr);
    recvfrom(sockfd, &sum, sizeof(sum), 0,
             (struct sockaddr *)&server_addr, &addr_len);

    printf("Client 1: Received Sum = %d\n", sum);

    close(sockfd);
    return 0;
}