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
#include <time_esp.h>

#define WIFI_SSID "brisa-2514576"
#define WIFI_PASS "9lye1rbb"

#define HOST_IP_ADDR "192.168.0.23"
#define PORT 1234

extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");    
    ESP_ERROR_CHECK(wifi_init(WIFI_SSID, WIFI_PASS));
    ESP_ERROR_CHECK(sync_time());

    int *sock = (int *)pvPortMalloc(sizeof(int));
    if (sock == NULL) {
        ESP_LOGE(MAIN, "Failed to allocate memory for socket");
        return;
    }

    // make the main code and in the future try to make verifications for the socket connection
    *sock = socket(AF_INET, SOCK_STREAM, 0);
    ESP_ERROR_CHECK_WITHOUT_ABORT(socket_client_connect(*sock, HOST_IP_ADDR, PORT));

    size_t free_heap_size = esp_get_free_heap_size();
    ESP_LOGI(MAIN, "Free heap size: %d bytes | %d megabytes", free_heap_size, free_heap_size / 1024);

    xTaskCreate(&dummy_reader_esp, "dummy_reader_esp", 1024*4, sock, 5, NULL);

    // clean up the memory allocated when isn't needed anymore
    // vPortFree(sock);
}