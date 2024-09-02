#ifndef SOCCKET_ESP_H
#define SOCCKET_ESP_H


void socket_client_connect(void *pvParameters);
bool send_message(char *message);
bool verify_creation_socket();
esp_err_t create_socket();

#endif