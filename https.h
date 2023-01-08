#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <unistd.h>
#include <netdb.h>
#include "logger.h"
#define BUFFER_SIZE 1024

typedef struct {
    char* file_name;
    char* hostname;
    char* file_url_path;
    pthread_mutex_t *mutex;
    int* pause_flag;
} thread_args_https;

void* download_file_https(void *arg);
void set_thread_priority_https(pthread_t thread, int priority);
void https_connection();