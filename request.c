#include <stdio.h>
#include <string.h>

#include "server.h"

void handle_request(int client_socket) {
    char buffer[1024];
    int bytes_received = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received < 0) return;
    buffer[bytes_received] = '\0';

    pthread_mutex_lock(&stats_lock);
    request_count++;
    received_bytes += bytes_received;
    pthread_mutex_unlock(&stats_lock);

    if (strncmp(buffer, "GET /static/", 12) == 0) {
        char *path = buffer + 4;  // Skip "GET "
        send_static_file(client_socket, path);
    } else if (strncmp(buffer, "GET /stats", 10) == 0) {
        send_stats(client_socket);
    } else if (strncmp(buffer, "GET /calc", 9) == 0) {
        char *query = strchr(buffer, '?');
        send_calc(client_socket, query ? query + 1 : "");
    } else {
        send(client_socket, "HTTP/1.1 404 Not Found\r\n\r\n", 26, 0);
    }
}
