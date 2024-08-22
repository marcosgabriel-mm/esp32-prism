#ifndef SOCCKET_ESP_H
#define SOCCKET_ESP_H


void socket_client_connect(int sock, const char *host_ip_addr, int port);
bool send_message(int sock, char *message);


#endif