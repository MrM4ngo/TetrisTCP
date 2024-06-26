#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdbool.h>
#include <ncurses.h>
#include <string.h>

#include "definitions.h"
#include "PORT.h"


#define PACKET_SIZE 1024  // Example packet size, adjust according to your needs

char GameOn = TRUE; 
int input;

int main() {

    initscr();
    cbreak();  // Disable line buffering
    noecho();  // Disable echoing of characters
    nodelay(stdscr, TRUE);  // Set non-blocking mode for getch()
    // Connect to the server
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        endwin();
        exit(EXIT_FAILURE);
    }

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"Error, no such host\n");
        endwin();
        exit(EXIT_FAILURE);
    }

    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr_list[0], server->h_length);
    serv_addr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("Error connecting");
        endwin();
        exit(EXIT_FAILURE);
    }

    // Main game loop
    int packet_count = 0;
    // int timer = 0;

    while (GameOn) {
        // Send packets to the server
        // Populate the buffer with packet data
        char buffer[PACKET_SIZE];
        if ((input = getch()) != ERR) {
        // printf("%c",input);

        if (input != EOF){
            snprintf(buffer, PACKET_SIZE, "%c", (char)input);
        }
        // Send the packet to the server
        n = write(sockfd, buffer, strlen(buffer));
        if (n < 0) {
            perror("Error writing to socket");
            endwin();
            exit(EXIT_FAILURE);
        }

        // Output information about the transmitted packet
        fflush(stdout);  // Flush the output to ensure immediate printing

        // Increment the packet count
        packet_count++;

        // Sleep for some time before sending the next packet (adjust as needed)
        // sleep(1);  // Sleep for 1 second
        }
        else{
        sleep(1);
        snprintf(buffer, PACKET_SIZE, " ");
        n = write(sockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                endwin();
                exit(EXIT_FAILURE);
            }
         }
        printf("Packet n transmitted: %d.\n", n );
    }
    printw("Number of data packets transmitted: %d", packet_count+1);
    // Close the socket
    close(sockfd);
    // printf("we left\n");

    return 0;
}
