#include<stdio.h>
#include<arpa/inet.h> // define htons()
#include<netinet/in.h> // define type sockaddr_in
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

#define PORT 12345
#define MSG "Hello from client"
#define MAX_SIZE 1000

int main(){

    int socket_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len;
    ssize_t bytes_send, bytes_recv;
    char buffer[MAX_SIZE];

    /*creat socket*/
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); //creates a new socket
    if(socket_fd == -1){
        perror("Failed to create the socket");
        return 1;
    }

    /* Initialize */
    memset(&buffer, '\0', sizeof(buffer));
    memset(&server_addr, 0, sizeof(server_addr));
    addr_len = sizeof(server_addr);

    /* Configure the Socket Address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);


    if(inet_pton(AF_INET, "192.168.1.184",&server_addr.sin_addr) <= 0){
        perror("Failed to convert the address");
        return 1;
    }
    /*
    int inet_pon(int domain, const char *src_str, void *addrptr);
    convert human-readable IP address to binary form
    src_str: server's IPv4 address
    */

    /* Send the message to server */
    bytes_send = sendto(socket_fd, MSG, strlen(MSG), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(bytes_send == -1){
        perror("Failed to send from socket");
        return 1;    
    }
    printf("Message sent, total %ld bytes. \n",bytes_send);


   /* Recieve the message from Client */
   bytes_recv = recvfrom(socket_fd, buffer, MAX_SIZE, 0, (struct sockaddr *)&server_addr, &addr_len);
   /*
   ssize_t recvfrom(int sockfd, void *buffer, size_t length, int flags, struct sockaddr *src_addr, socklen_t *addrlen);
   return number of bytes received, 0 on EOF, -1 on error
   flags: A bit mask controlling socket-specific I/O features. don't need>> set 0
   src_addr: sender's socket address structure
   addrlen: length of src_addr  
   */
   if(bytes_recv == -1){
        perror("Failed to recieved from socket");
        return 1;    
   }
   buffer[bytes_recv] = '\0';
   printf("Client(recieved %ld bytes): %s\n",bytes_recv, buffer);

   close(socket_fd);
   return 0;
}