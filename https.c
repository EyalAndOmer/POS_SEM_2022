#include "https.h"

// sets the priority of a given thread
void set_thread_priority_https(pthread_t thread, int priority) {
    int policy;
    struct sched_param param;
    pthread_getschedparam(thread, &policy, &param);
    param.sched_priority = priority;
    pthread_setschedparam(thread, policy, &param);
}

void* download_file_https(void *arg) {
    char buffer[BUFFER_SIZE];
    memset(&buffer, 0, sizeof(buffer));
    SSL_CTX *ctx;
    SSL *ssl;
    int bytes_received;
    int content_length = 0;
    thread_args_https *args = (thread_args_https *)arg;

    // Get the server address
    struct hostent *server = gethostbyname(args->hostname);
    if (server == NULL) {
        perror("Server neexistuje.");
        exit(1);
    }

    // Initialize SSL
    SSL_library_init();
    ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) {
        perror("Unable to create SSL context");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }

    // Create the socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Unable to create socket");
        exit(EXIT_FAILURE);
    }
    printf("Socket created!\n");
    // Set up the server address

    struct sockaddr_in server_addr;
    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_addr.sin_addr.s_addr, server->h_length);
    server_addr.sin_port = htons(443);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Unable to connect to server");
        exit(EXIT_FAILURE);
    }
    printf("Connected to server!\n");
    // Create the SSL object
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, sockfd);

    // Perform the SSL handshake
    if (SSL_connect(ssl) == -1) {
        perror("Unable to perform SSL handshake");
        ERR_print_errors_fp(stderr);
        exit(EXIT_FAILURE);
    }
    // Send a message to the server
    char request[BUFFER_SIZE];
    printf("Filename: %s, hostname: %s\n", args->file_url_path, args->hostname);
    sprintf(request, "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n", args->file_url_path, args->hostname);

    bytes_received = SSL_write(ssl, request, strlen(request));
    if (bytes_received < 0) {
        perror("Unable to write to server");
        exit(EXIT_FAILURE);
    }

    // parse the HTTP response
    char* ptr=buffer;
    int status;

    // Search for status of response
    bytes_received = SSL_read(ssl, ptr, 1);
    while(bytes_received) {
        if(bytes_received == -1) {
            perror("Error receiving data from server");
            pthread_exit(NULL);
        }
        // Check for the end of line
        if((ptr[-1]=='\r')  && (*ptr=='\n' ))  {
            break;
        }
        ptr++;
        bytes_received = SSL_read(ssl, ptr, 1);
    }

    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;
    ptr=strstr(ptr,"HTTP/1.1 ");
    sscanf(ptr,"%*s %d ", &status);

    if (status != 200) {
        perror("Status is not 200!");
        pthread_exit(NULL);
    }

    // Search for content length of response
    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;
    bytes_received = SSL_read(ssl, ptr, 1);
    while(bytes_received) {

        if(bytes_received==-1) {
            perror("Parse Header");
            pthread_exit(NULL);
        }

        if( (ptr[-3]=='\r')  && (ptr[-2]=='\n') && (ptr[-1]=='\r')  && (*ptr=='\n')) {
            break;
        }

        ptr++;
        bytes_received = SSL_read(ssl, ptr, 1);
    }

    memset(&ptr, 0, sizeof(ptr));
    ptr=buffer;

    if (bytes_received) {
        ptr=strstr(ptr,"Content-Length:");
        if(ptr) {
            sscanf(ptr,"%*s %d",&content_length);
        } else {
            perror("Request without content length are ignored!");
            pthread_exit(NULL);
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
        bytes_received = SSL_read(ssl, buffer, BUFFER_SIZE);
        if (bytes_received == -1) {
            perror("Error receiving data from server");
            pthread_exit(NULL);
        }
        remaining -= bytes_received;
        fwrite(buffer, 1, bytes_received, fp);
        memset(&buffer, 0, sizeof(buffer));
    }

    write_to_logger(args->file_name, content_length, 1);

    // Close the connection
    SSL_shutdown(ssl);
    SSL_free(ssl);
    close(sockfd);
}

void https_connection() {
    srand(time(NULL));
    //  Thread code
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    thread_args_https args[5];
    pthread_t file_download_threads[5];

    char* file_paths[5] = {"/~chochlik/frios/frios/sk/cvicenia/bash_uvod.html", "/~chochlik/frios/frios/sk/cvicenia/procesy.html",
                           "/~chochlik/frios/frios/sk/cvicenia/rury.html"};

    char* file_names[5] = {"bash_uvod.html", "procesy.html", "rury.html"};

    int file_pause[5] = {0, 0, 0, 0, 0};

    for (int i = 0; i < 3; ++i) {
        args[i].mutex = &mutex;
        args[i].pause_flag = &file_pause[i];
        args[i].file_name = file_names[i];
        args[i].hostname = "frios2.fri.uniza.sk";
        args[i].file_url_path = file_paths[i];

        pthread_create(&file_download_threads[i], NULL, download_file_https, &args[i]);
        set_thread_priority_https(file_download_threads[i], rand() % 50 + 1);
    }

    for (int i = 0; i < 3; ++i) {
        pthread_join(file_download_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
}