#include<stdio.h>
#include<arpa/inet.h> // define htons()
#include<netinet/in.h> // define type sockaddr_in
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

#define PORT 3000
#define MSG "Hello from server"
#define MAX_SIZE 1000

int main(){

    int socket_fd;
    struct sockaddr_in server_addr, client_addr;
    /*
    struct in_addr{  //IPv4 4-byte address
        in_addr_t s_addr; --> Unsigned 32-bit integer
    }
    struct sockaddr_in {  // IPv4 socket address
        sa_family_t  sin_family; -->Address family (AF_INET)
        in_port_t  sin_port;  -->Port number
        struct in_addr sin_addr; -->IPv4 address
        unsigned char __pad[X]; --> Pad to size of 'sockaddr' structure (16 bytes)
    }
    */
    socklen_t addr_len;
    ssize_t bytes_send, bytes_recv;
    char buffer[MAX_SIZE];

    /*creat socket*/
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0); //creates a new socket
    // int socket(int domain, int type, int potocol);
    // return file descriptor on success, or -1 on error
    // AF_INET: IPv4, SOCK_DGRAM: UDP, 0: default protocol 
    if(socket_fd == -1){
        perror("Failed to create the socket");
        return 1;
    }

    /* Initialize */
    memset(&buffer, '\0', sizeof(buffer));
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));
    addr_len = sizeof(client_addr);

    /* Configure the Socket Address */
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    /* uint16_t htons(uin16_t host_uint16); >> #include<arpa/inet.h>
    -->convert "host byte order" to "network byte order" (big endian)
    */
    server_addr.sin_addr.s_addr = INADDR_ANY;
    /*
    IPv4 wildcard address equals 0.0.0.0 
    INADDR_ANY ensure we bind to all available network cards 
    */

    /* Bind the socket with the server address */
    if(bind(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1){
        perror("Failed to bind the socket");
        return 1;
    }
    /*
    int bind (int sockfd, const struct sockaddr *addr, socklen_t addrlen);
    return 0 on success, -1 on error
    */
   /* Recieve the message from Client */
   bytes_recv = recvfrom(socket_fd, buffer, MAX_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
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

   /* Send the message to Client */
   bytes_send = sendto(socket_fd, MSG, strlen(MSG), 0, (struct sockaddr *)&client_addr, addr_len);
    if(bytes_send == -1){
        perror("Failed to send from socket");
        return 1;    
   }
   printf("Message sent, total %ld bytes. \n",bytes_send);

   close(socket_fd);
   return 0;
}