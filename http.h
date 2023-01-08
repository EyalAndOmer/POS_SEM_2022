#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>
#define BUFFER_SIZE 1024

// structure used in threads
typedef struct {
    char* file_name;
    char* hostname;
    char* file_url_path;
    pthread_mutex_t *mutex;
    int* pause_flag;
} thread_args_http;

void set_thread_priority(pthread_t thread, int priority);
void* download_file(void *arg);
void http_connection();
