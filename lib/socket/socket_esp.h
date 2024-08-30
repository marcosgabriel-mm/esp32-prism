#ifndef SOCCKET_ESP_H
#define SOCCKET_ESP_H

// initialize the socket connection with the server
esp_err_t socket_client_connect(int sock);

// recive a message from the server
bool recive_message(int sock);

// send a message to the server
bool send_message(int sock, char *message);

#endif