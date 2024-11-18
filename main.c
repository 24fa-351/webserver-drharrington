#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "server.h"

int request_count = 0;
size_t received_bytes = 0;
size_t sent_bytes = 0;
pthread_mutex_t stats_lock;

void *handle_connection(void *client_socket_ptr) {
    int client_socket = *(int *)client_socket_ptr;
    free(client_socket_ptr);
    handle_request(client_socket);
    close(client_socket);
    return NULL;
}

int main(int argc, char *argv[]) {
    int port = DEFAULT_PORT;
    if (argc > 1 && strcmp(argv[1], "-p") == 0 && argc > 2) {
        port = atoi(argv[2]);
    }

    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr = {.sin_family = AF_INET,
                                      .sin_port = htons(port),
                                      .sin_addr.s_addr = INADDR_ANY};

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    pthread_mutex_init(&stats_lock, NULL);

    while (1) {
        int *client_socket_ptr = malloc(sizeof(int));
        *client_socket_ptr = accept(server_socket, NULL, NULL);
        pthread_t thread;
        pthread_create(&thread, NULL, handle_connection, client_socket_ptr);
        pthread_detach(thread);
    }

    close(server_socket);
    pthread_mutex_destroy(&stats_lock);
    return 0;
}
