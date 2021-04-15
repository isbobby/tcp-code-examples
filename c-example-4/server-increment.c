#include "headsock.h"

// Function Protoypes
void readfromsocket(int sockfd); // Receive data from socket sockfd
void send_ack(int sockfd, struct sockaddr *client_addr, socklen_t client_addrlen, long fileoffset); // Block until acknowledge is sent

int main(int argc, char *argv[]) {
    // Setup server socket address
    struct sockaddr_in server_addr_in;
    server_addr_in.sin_family = AF_INET;
    server_addr_in.sin_port = htons(MYUDP_PORT); // htons() converts port number to big endian
    server_addr_in.sin_addr.s_addr = INADDR_ANY; // INADDR_ANY=0, 0 means receive data from any IP address
    memset(&(server_addr_in.sin_zero), 0, sizeof(server_addr_in.sin_zero));
    // Typecast internet socket address (struct sockaddr_in) to generic socket address (struct sockaddr)
    struct sockaddr *server_addr = (struct sockaddr *)&server_addr_in;
    socklen_t server_addrlen = sizeof(struct sockaddr);

    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { 
        // printf("error creating socket"); exit(1); 
    }

    // Bind socket address to socket (server only, client don't need)
    if (bind(sockfd, server_addr, server_addrlen) == -1) { 
        // printf("error in binding"); exit(1); 
    }

    // Read file data from socket
    while (1) {
        readfromsocket(sockfd);
    }
    
    close(sockfd);

    // Compare myfile.txt and myUDPreceive.txt
    // printf("Differences between myfile.txt and myUDPreceive.txt:\n");
    char * const args[]={"diff", "myfile.txt", "myUDPreceive.txt"}; 
    execv("/usr/bin/diff", args);
}

void readfromsocket(int sockfd) {
    char packet[DATAUNIT];
    struct sockaddr client_addr;
    socklen_t client_addrlen = sizeof(struct sockaddr);
    
    // handle file size
    long filesize;
    int n = recvfrom(sockfd, &filesize, sizeof(filesize), 0, &client_addr, &client_addrlen);

    if (filesize > 100397238061055) {
        // catch seg fault
        return;
    }

    // printf("file size: %ld bytes, DATAUNIT %d bytes\n", filesize-1, DATAUNIT);

    // if (n < 0) { printf("error in receiving packet\n"); exit(1); }
    send_ack(sockfd, &client_addr, client_addrlen, 0); // Acknowledge that filesize has been received

    // create file buffer
    char filebuffer[filesize];

    int ack_limit = 1;
    long fileoffset = 0; // Tracks how many bytes have been received so far
    char packetlastbyte; // Tracks the last byte of the latest packet received

    do {
        // cycle through 1-3 data units before sending an ACK
        for (int i=0; i<ack_limit; i++) {
            int bytesreceived = recvfrom(sockfd, &packet, DATAUNIT, 0, &client_addr, &client_addrlen);
            if (bytesreceived < 0) {
                exit(1);
            }

            // printf("packet of size %d received\n", bytesreceived);

            // terminate if incoming packet has no more data
            if (bytesreceived == 0){
                return;
            }

            // Append packet data to filebuffer
            if (bytesreceived > 0){
                memcpy((filebuffer + fileoffset), packet, bytesreceived);
                fileoffset += bytesreceived;
            }

            packetlastbyte = packet[bytesreceived-1];
            if (packetlastbyte == 0x4){ break; };
        }
        // printf("send ack: %ld\n", fileoffset);
        // Acknowledge that packet has been received
        send_ack(sockfd, &client_addr, client_addrlen, fileoffset);
        
        // cycle ack
        ack_limit = (++ack_limit % 4 == 0) ? 1 : ack_limit % 4;

    } while (packetlastbyte != 0x4);

    fileoffset-=1;

    // write file
    char filename[] = "received.txt";
    FILE* fp = fopen(filename, "wt");
    fwrite(filebuffer, 1, fileoffset, fp);
    fclose(fp);
    // printf("Received.txt size: %d bytes\n", (int)fileoffset);
}

void send_ack(int sockfd, struct sockaddr *addr, socklen_t addrlen, long fileoffset) {
    const int ACKNOWLEDGE = 1;
    int ack_sent = 0;
    int ack_thresh = 10;
    while (!ack_sent) {
        if (sendto(sockfd, &ACKNOWLEDGE, sizeof(ACKNOWLEDGE), 0, addr, addrlen) >= 0) {
            ack_sent = 1;
        } else {
            if (ack_thresh-- <= 0) {
                exit(1);
            } else {
                printf("error sending ack, trying again..\n");}
        }
    }
}