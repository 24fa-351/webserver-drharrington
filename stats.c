#include <stdio.h>

#include "server.h"

void send_stats(int client_socket) {
    pthread_mutex_lock(&stats_lock);
    int requests = request_count;
    size_t received = received_bytes;
    size_t sent = sent_bytes;
    pthread_mutex_unlock(&stats_lock);

    char response[512];
    int response_len =
        snprintf(response, sizeof(response),
                 "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
                 "<html><body><h1>Server Stats</h1>"
                 "<p>Requests: %d</p><p>Received bytes: %zu</p><p>Sent bytes: "
                 "%zu</p></body></html>",
                 requests, received, sent);
    send(client_socket, response, response_len, 0);
}
