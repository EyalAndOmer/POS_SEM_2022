#include "logger.h"

void write_to_logger(char* file_name, int size, int download_status) {
    FILE *f;
    f = fopen("../download.log", "a+");
    if (f == NULL) {
        perror("Error while openning log file");
        exit(1);
    }
    fprintf(f, "file: %s; size: %d; download status: %d\n", file_name, size, download_status);
}
