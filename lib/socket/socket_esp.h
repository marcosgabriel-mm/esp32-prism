#ifndef SOCCKET_ESP_H
#define SOCCKET_ESP_H


esp_err_t socket_client_connect(int sock, const char *HOST_IP_ADDR, const int PORT);
bool send_message(int sock, char *message);
bool verify_creation_socket(int sock);

#endif