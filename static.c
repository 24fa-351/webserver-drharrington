#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "server.h"

void send_static_file(int client_socket, const char *path) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", STATIC_DIR, path + 7);

    FILE *file = fopen(filepath, "rb");
    if (!file) {
        send(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n", 26, 0);
        return;
    }

    struct stat st;
    stat(filepath, &st);

    char header[128];
    int header_len =
        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\nContent-Length: %ld\r\n\r\n", st.st_size);
    send(client_socket, header, header_len, 0);

    char buffer[1024];
    int bytes;
    while ((bytes = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(client_socket, buffer, bytes, 0);
        pthread_mutex_lock(&stats_lock);
        sent_bytes += bytes;
        pthread_mutex_unlock(&stats_lock);
    }
    fclose(file);
}
