#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#define BUFSIZE 8192

/* send QUIT command */
void quit(int sockfd, int n, char *recvline, char *sendline) {
    sprintf(sendline, "QUIT\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
    close(sockfd);
}

/* read response from server */
void readResponse(int sockfd, int n, char *recvline) {
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s\n", recvline);
}

/* read list of files from data connection */
void readFiles(int sockfd, int n, int connfd, char *recvline, char *sendline) {
    char filelist[BUFSIZE];
    int total_bytes = 0;
    while ((n = read(connfd, recvline, BUFSIZE)) > 0) {
        strcat(filelist, recvline);
        total_bytes += n;
    }

    printf("%s\n", filelist);

    // Split the received data into lines
    char *line = strtok(filelist, "\n");
    while (line != NULL) {
        // Parse the line and extract the relevant information as described above
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

        // Use the extracted information as needed
        printf("File: %s\n", name);
        printf("Size: %lld\n", size);

        // Get the next line
        line = strtok(NULL, "\n");
    }
    sprintf(sendline, "RETR pom.txt\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
}

/* connect to data port */
int connectToDataPort(int connfd, sockaddr_in &client, const char *data_ip, int data_port) {
    memset(&client, 0, sizeof(client));
    client.sin_family = AF_INET;
    client.sin_port = htons(data_port);
    client.sin_addr.s_addr = inet_addr(data_ip);
    connfd = socket(AF_INET, SOCK_STREAM, 0);
    if (connfd < 0) {
        printf("socket error\n");
        exit(1);
    }

    if (connect(connfd, (struct sockaddr *) &client, sizeof(client)) < 0) {
        printf("connect error\n");
        exit(1);
    }

    printf("Connected!\n");
    return connfd;
}

/* parse IP and port from PASV response */
int parseIPandPort(const char *recvline) {
    int ip1, ip2, ip3, ip4, port1, port2;
    sscanf(recvline, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &port1, &port2);
    int data_port = port1 * 256 + port2;
    char data_ip[16];
    sprintf(data_ip, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
    return data_port;
}


/* send TYPE command */
void sendTYPE(int sockfd, int n, char *recvline, char *sendline) {
    sprintf(sendline, "TYPE I\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
    /* send PASV command to enter passive mode */
    sprintf(sendline, "PASV\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
}

/* send PASS command */
void sendPassword(int sockfd, int n, char *recvline, char *sendline) {
    sprintf(sendline, "PASS st123ftp\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
}

/* send USER command */
void sendUsername(int sockfd, int n, char *recvline, char *sendline) {
    sprintf(sendline, "USER ftpstudent\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
}

/* get server IP address */
void getServerIPAddress(const char *hname, int &sockfd, hostent *&hptr) {
    hptr = gethostbyname(hname);
    if (hptr == NULL) {
        printf("invalid hostname: %s\n", hname);
        exit(1);
    }
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        printf("socket error\n");
        exit(1);
    }
}

/* read welcome message from server */
void readWelcomeMessage(int sockfd, int n, char *recvline) {
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);
}

/* connect to server */
void connectToServer(struct sockaddr_in servaddr, int port, struct hostent *hptr, int sockfd){
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    memcpy(&servaddr.sin_addr, hptr->h_addr, hptr->h_length);
    if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0) {
        printf("connect error\n");
        exit(1);
    }
}
int main(int argc, char *argv[]) {
    int sockfd, n;
    int connfd;
    char recvline[BUFSIZE];
    char sendline[BUFSIZE];
    struct sockaddr_in servaddr;
    struct sockaddr_in client;
    char *hname;
    struct hostent *hptr;
    int port;
    hname = "158.193.128.160";
    port = 21;
    /* get server IP address */
    getServerIPAddress(hname, sockfd, hptr);
    /* connect to server */
    connectToServer(servaddr, port, hptr, sockfd);

    /* read welcome message from server */
    readWelcomeMessage(sockfd, n, recvline);
    /* send USER command */
    sendUsername(sockfd, n, recvline, sendline);

    /* send PASS command */
    sendPassword(sockfd, n, recvline, sendline);

    /* send TYPE command */
    // TYPE I
    sendTYPE(sockfd, n, recvline, sendline);

    /* parse IP and port from PASV response */
    char data_ip[16];
    int data_port = parseIPandPort(recvline);

    /* connect to data port */
    connfd = connectToDataPort(connfd, client, data_ip, data_port);

        sprintf(sendline, "LIST\r\n");
    write(sockfd, sendline, strlen(sendline));
    n = read(sockfd, recvline, BUFSIZE);
    recvline[n] = '\0';
    printf("%s", recvline);


    /* read list of files from data connection */
    readFiles(sockfd, n, connfd, recvline, sendline);

    /* close data connection */
    close(connfd);
    /* read response from server */
    readResponse(sockfd, n, recvline);

    /* send QUIT command */
    quit(sockfd, n, recvline, sendline);
    return 0;
}

















