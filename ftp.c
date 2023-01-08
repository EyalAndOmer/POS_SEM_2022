#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define BUFFER_SIZE 8192

int connect_to_server(char* server_hostname, char* username, char* password);
int create_data_connection(int control_socket);
void show_files(int data_socket, int control_socket);

int xd(int argc, char *argv[]) {
    char* hname = "158.193.128.160";
    char* username = "ftpstudent";
    char* password = "st123ftp";
    int control_socket = connect_to_server(hname, username, password);
    int data_socket = create_data_connection(control_socket);

    show_files(data_socket, control_socket);
    close(data_socket);

    return 0;
}

// Function that connects us to the server and prepares it for a passive mode connection
int connect_to_server(char* server_hostname, char* username, char* password) {
    int control_socket;
    struct hostent* server;
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];

    int bytes_received;

    control_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (control_socket < 0) {
        perror("Control socket error");
        exit(1);
    }

    server = gethostbyname(server_hostname);
    if (server == NULL) {
        perror("No such host error");
        exit(1);
    }

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(21);
    memcpy(&server_address.sin_addr, server->h_addr, server->h_length);
    if (connect(control_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        perror("Connect error");
        exit(1);
    }

    printf("Connected!\n");

    memset(&buffer, 0, sizeof(buffer));
    bytes_received = recv(control_socket, buffer,BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        exit(1);
    }
    printf("%s", buffer);


    sprintf(buffer, "USER %s\r\n", username);
    if (send(control_socket, buffer, strlen(buffer), 0) < 0) {
        perror("Error sending username");
        exit(1);
    }

    memset(&buffer, 0, sizeof(buffer));
    bytes_received = recv(control_socket, buffer,BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        exit(1);
    }
    printf("%s", buffer);

    memset(&buffer, 0, sizeof(buffer));

    printf("%s", buffer);

    sprintf(buffer, "PASS %s\r\n", password);
    if (send(control_socket, buffer, strlen(buffer), 0) < 0) {
        perror("Error sending password");
        exit(1);
    }

    memset(&buffer, 0, sizeof(buffer));
    bytes_received = recv(control_socket, buffer,BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        exit(1);
    }
    printf("%s", buffer);

    return control_socket;
}

// Function that establishes data connection with the control socket and creates a new socket with that connection
int create_data_connection(int control_socket) {
    struct sockaddr_in server_address;
    char buffer[BUFFER_SIZE];
    int bytes_received;

    memset(&buffer, 0, sizeof(buffer));


    sprintf(buffer, "PASV\r\n");
    send(control_socket, buffer, strlen(buffer), 0);
    bytes_received = recv(control_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        exit(1);
    }

    printf("%s", buffer);

    int ip1, ip2, ip3, ip4, port1, port2;
    sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
    int data_port = port1 * 256 + port2;
    char data_ip[16];
    sprintf(data_ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);

    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(data_port);
    server_address.sin_addr.s_addr = inet_addr(data_ip);

    int data_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (data_socket < 0) {
        printf("socket error\n");
        exit(1);
    }

    if (connect(data_socket, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        printf("connect error\n");
        exit(1);
    }

    printf("Connected!\n");

    return data_socket;
}

void show_files(int data_socket, int control_socket) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    memset(&buffer, 0, sizeof(buffer));
    sprintf(buffer, "LIST\r\n");

    send(control_socket, buffer, strlen(buffer), 0);
    bytes_received = recv(control_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_received < 0) {
        perror("Error receiving data");
        exit(1);
    }
    printf("%s", buffer);

    while ((bytes_received = recv(data_socket, buffer, BUFFER_SIZE, 0)) > 0) {
    }

    printf("%s", buffer);

    // Split the received data into lines
    char *line = strtok(buffer, "\n");
    while (line != NULL) {
        char type;
        char permissions[10];
        int links;
        char owner[20];
        char group[20];
        long long size;
        char date[20];
        char day[20];
        char time[20];
        char name[100];

        sscanf(line, "%c%s %d %s %s %lld %s %s %s %s", &type, permissions, &links, owner, group, &size, date, day, time, name);

        printf("File: %s, Size: %lld\n", name, size);

        line = strtok(NULL, "\n");
    }
    memset(&buffer, 0, sizeof(buffer));
}