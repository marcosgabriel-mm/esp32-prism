#ifndef SOCCKET_ESP_H
#define SOCCKET_ESP_H


void socket_client_connect(int sock);
bool send_message(int sock, char *message);
bool verify_creation_socket(int sock);

#endif