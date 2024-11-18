#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "server.h"

void send_calc(int client_socket, const char *query) {
    int a = 0, b = 0;
    sscanf(query, "a=%d&b=%d", &a, &b);
    int result = a + b;

    char response[256];
    int response_len = snprintf(
        response, sizeof(response),
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<html><body><h1>Calc Result</h1><p>%d + %d = %d</p></body></html>",
        a, b, result);
    send(client_socket, response, response_len, 0);
}
