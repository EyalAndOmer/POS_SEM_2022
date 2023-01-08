#include "http.h"
#include "logger.h"


// sets the priority of a given thread
void set_thread_priority(pthread_t thread, int priority) {
    int policy;
    struct sched_param param;
    pthread_getschedparam(thread, &policy, &param);
    param.sched_priority = priority;
    pthread_setschedparam(thread, policy, &param);
}

// Function that downloads file from socket
void* download_file(void *arg) {
    // Simulate download start at specific time
    sleep(rand() % 10);

    thread_args *args = (thread_args *)arg;
    char buffer[BUFFER_SIZE];
    memset(&buffer, 0, sizeof(buffer));
    int bytes_received;
    int content_length = 0;

    // Socket creation and connect code
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    struct hostent *server = gethostbyname(args->hostname);
    if (server == NULL) {
        fprintf(stderr, "Error getting host name\n");
        exit(1);
    }

    // Set up the server address struct
    struct sockaddr_in server_address;
    memset (&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(80);
    memcpy(&server_address.sin_addr.s_addr, server->h_addr, server->h_length);

    if (connect(sockfd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror("Error connecting to server");
        exit(1);
    }

    printf("Connected!\n");

    // Send the HTTP request to the server
    char request[BUFFER_SIZE];
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", args->file_url_path, args->hostname);
    send(sockfd, request, strlen(request), 0);

    printf("Received http response!\n");

    // parse the HTTP response
    char* ptr=buffer;
    int status;

    // Search for status of response
    bytes_received = recv(sockfd, ptr, 1, 0);
    while(bytes_received){
        if(bytes_received==-1) {
            perror("ReadHttpStatus");
            exit(1);
        }
        // Check for the end of line
        if((ptr[-1]=='\r')  && (*ptr=='\n' ))  {
            break;
        }
        ptr++;
        bytes_received = recv(sockfd, ptr, 1, 0);
    }

    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;
    ptr=strstr(ptr,"HTTP/1.1 ");
    sscanf(ptr,"%*s %d ", &status);

    if (status != 200) {
        perror("Status is not 200!");
        exit(1);
    }

    // Search for content length of response
    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;
    bytes_received = recv(sockfd, ptr, 1, 0);
    while(bytes_received) {

        if(bytes_received==-1) {
            perror("Parse Header");
            exit(1);
        }

        if( (ptr[-3]=='\r')  && (ptr[-2]=='\n') && (ptr[-1]=='\r')  && (*ptr=='\n')) {
            break;
        }

        ptr++;
        bytes_received = recv(sockfd, ptr, 1, 0);
    }

    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;

    if(bytes_received){
        ptr=strstr(ptr,"Content-Length:");
        if(ptr) {
            sscanf(ptr,"%*s %d",&content_length);
        } else {
            perror("Request without content length are ignored!");
            exit(1);
        }
    }

    // Open file in binary writing mode
    FILE* fp = fopen(args->file_name, "wb");
    if (fp == NULL) {
        fprintf(stderr, "Error opening file\n");
        pthread_exit(NULL);
    }

    printf("File created!\n");

    // Read the data from the server and write it to the file
    int remaining = content_length;
    int received_byte_count;

    // Download while you dont have all the data
    memset(&buffer, 0, sizeof(buffer));
    while (remaining > 0) {
        pthread_mutex_lock(args->mutex);
        int pause = *args->pause_flag;
        pthread_mutex_unlock(args->mutex);
        // Pause the download
        if (pause) {
            printf("Paused the download!\n");
            while (1) {
                pthread_mutex_lock(args->mutex);
                pause = *args->pause_flag;
                pthread_mutex_unlock(args->mutex);
                if (!pause) {
                    break;
                }
            }
        }

        // Get data and write it to file
        received_byte_count = recv(sockfd, buffer, BUFFER_SIZE, 0);
        remaining -= received_byte_count;
        fwrite(buffer, 1, received_byte_count, fp);
        memset(&buffer, 0, sizeof(buffer));
    }

    write_to_logger(args->file_name, content_length, 1);

    fclose(fp);
    close(sockfd);
    pthread_exit(NULL);
}

int xdd() {
    srand(time(NULL));
//  Thread code
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    thread_args args[5];
    pthread_t file_download_threads[5];

    char* file_paths[5] = {"/code/intro.txt", "/code/emaildb.py", "/code/pagerank.zip",
                              "/code/twfriends.py", "/code/twspider.py"};

    char* file_names[5] = {"intro.txt", "emaildb.py", "pagerank.zip",
                           "twfriends.py", "twspider.py"};

    int file_pause[5] = {0, 0, 0, 0, 0};

    for (int i = 0; i < 5; ++i) {
        args[i].mutex = &mutex;
        args[i].pause_flag = &file_pause[i];
        args[i].file_name = file_names[i];
        args[i].hostname = "py4inf.com";
        args[i].file_url_path = file_paths[i];

        pthread_create(&file_download_threads[i], NULL, download_file, &args[i]);
        set_thread_priority(file_download_threads[i], rand() % 50 + 1);
    }

    for (int i = 0; i < 5; ++i) {
        pthread_join(file_download_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    return 0;
}