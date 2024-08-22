#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"

#include <tags_log.h>
#include <wifi_esp.h>
#include <http_esp.h>

#include <socket_esp.h>
#include <reader_esp.h>

#define WIFI_SSID "brisa-2514576"
#define WIFI_PASS "9lye1rbb"

#define HOST_IP_ADDR "192.168.0.23" 
#define PORT 1234

extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");    
    ESP_ERROR_CHECK(wifi_init(WIFI_SSID, WIFI_PASS));

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    socket_client_connect(sock, HOST_IP_ADDR, PORT);

    printf("Socket: %d\n", sock);
    xTaskCreate(&dummy_reader_esp, "dummy_reader_esp", 4096, &sock, 1, NULL);
    

    // xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    // xTaskCreate(&http_get_task, "http_get_task", 8192, NULL, 5, NULL);
    // xTaskCreate(&socket_client_task, "tcp_client_task", 4096, NULL, 5, NULL);
}