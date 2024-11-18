#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>
#include <pthread.h>

// Server settings
#define DEFAULT_PORT 80
#define STATIC_DIR "./static"

// Statistics data
extern int request_count;
extern size_t received_bytes;
extern size_t sent_bytes;
extern pthread_mutex_t stats_lock;

// Function declarations
void handle_request(int client_socket);
void send_static_file(int client_socket, const char* path);
void send_stats(int client_socket);
void send_calc(int client_socket, const char* query);

#endif
