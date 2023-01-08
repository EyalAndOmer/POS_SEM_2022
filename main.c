#include "https.h"
#include "http.h"

int main() {
    int selection = 0;
    while (selection < 1 || selection > 2) {
        printf("1. HTTP\n");
        printf("2. HTTPS\n");
        printf("Zvol si protokol:");
        scanf("%d", &selection);
    }
    if (selection == 1) {
        http_connection();
    } else {
        https_connection();
    }
}
