#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <esp_log.h>
#include <tags_log.h>
#include <esp_system.h>
#include <esp_mac.h>

#define HOST_IP_ADDR "10.0.0.115"
#define PORT 1234

int sock = -1;
char mac_str[18];
bool first_connection_flag = true;


void socket_client_connect(void *pvParameters);

esp_err_t create_socket() {
    
    if (sock >= 1) {
        close(sock);
        sock = -1;
    }
    
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        ESP_LOGE(SOCKET, "Error to create socket: errno %d", errno);
        return ESP_FAIL;
    }
    return ESP_OK;
}

void first_connection() {
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

bool recive_confirmation() {
    char rx_buffer[128];
    int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
    if (len < 0) {
        ESP_LOGE(SOCKET, "recv failed: errno %d", errno);
        return false;
    }

    rx_buffer[len] = 0;
    if (strcmp(rx_buffer, "200") == 0) {
        ESP_LOGI(SOCKET, "Received %s", rx_buffer);
        return true;
    }

    return false;
}

static bool reconnection_task_flag = false;
bool send_message(char *message) {

    int err = send(sock, message, strlen(message), 0);
    if (err < 0 || !recive_confirmation()) {
        ESP_LOGE(SOCKET, "Error occurred during sending: errno %d", errno);
        first_connection_flag = true;

        // if the flag indicates that the connection is being reestablished, don't create a new task
        if (!reconnection_task_flag) {
            ESP_LOGI(SOCKET, "Creating reconnection task...");
            reconnection_task_flag = true;
            xTaskCreate(&socket_client_connect, "socket_client_connect", 1024*4, NULL, 5, NULL);
        }
        return false;
    }
    return true;
}


void socket_client_connect(void *pvParameters) {   



    struct sockaddr_in dest_addr;
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    dest_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, HOST_IP_ADDR, &dest_addr.sin_addr);

    while (true) {

        if (create_socket() != ESP_OK) {
            vTaskDelay(5000 / portTICK_PERIOD_MS);
            continue;
        }

        ESP_LOGI(SOCKET, "Attempting to connect to the server...");
        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(dest_addr));
        if (err == 0) {
            ESP_LOGI(SOCKET, "Successfully connected to the server");
            if (first_connection_flag) {
                first_connection();
            }
            break;
        } else {
            ESP_LOGE(SOCKET, "Failed to reconnect to the server: errno %d", errno);
            ESP_LOGI(SOCKET, "Retrying in 5 seconds...");
            vTaskDelay(5000 / portTICK_PERIOD_MS);
        }
    }

    ESP_LOGI(SOCKET, "Reconnection task completed");
    reconnection_task_flag = false;
    vTaskDelete(NULL);
}