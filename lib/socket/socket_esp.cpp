#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "esp_log.h"
#include <tags_log.h>
#include <esp_system.h>
#include <esp_mac.h>

char mac_str[18];

bool first_connection_flag = true;
void first_connection(int sock) {
    uint8_t mac[6];  
    esp_read_mac(mac, ESP_MAC_WIFI_STA);
    snprintf(mac_str, sizeof(mac_str), "%02X:%02X:%02X:%02X:%02X:%02X",
             mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

    int err = send(sock, mac_str, strlen(mac_str), 0);
    if (err < 0) {
        ESP_LOGE(SOCKET, "Error occurred during sending: errno %d", errno);
        return;
    }
    first_connection_flag = false;
}

bool recive_message(int sock) {
    char rx_buffer[128];
    int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
    if (len < 0) {
        ESP_LOGE(SOCKET, "recv failed: errno %d", errno);
        return false;
    }
    rx_buffer[len] = 0;
    if (strcmp(rx_buffer, "200") == 0) {
        ESP_LOGI(SOCKET, "Message sent successfully");
        return true;
    }

    return false;
}

bool send_message(int sock, char *message) {
    int err = send(sock, message, strlen(message), 0);
    if (err < 0) {
        ESP_LOGE(SOCKET, "Error occurred during sending: errno %d", errno);
        return false;
    }

    if (recive_message(sock)) {
        return false;
    }

    return true;
}


void socket_client_connect(int sock, const char *host_ip_addr, int port) {

    if (sock < 0) {
        ESP_LOGE(SOCKET, "Unable to create socket: errno %d", errno);
        return;
    }
    ESP_LOGI(SOCKET, "Socket created, connecting to %s:%d", host_ip_addr, port);

    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(port);
    inet_pton(AF_INET, host_ip_addr, &dest_addr.sin_addr);

    int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
    if (err != 0) {
        ESP_LOGE(SOCKET, "Socket unable to connect: errno %d", errno);
        close(sock);
        return;
    }
    ESP_LOGI(SOCKET, "Successfully connected");

    if (first_connection_flag) {
        first_connection(sock);
    }

}


    // while (1) {

    //     if (first_connection_flag) {
    //         first_connection(sock);
    //     } else {
    //         char message[32];
    //         strcpy(message, mac_str);
    //         strcat(message, " - UID");
    //         if (!send_message(sock, message)) {break;}
    //     }
    //     if (!recive_message(sock)) {break;}

    // }

    // close(sock);