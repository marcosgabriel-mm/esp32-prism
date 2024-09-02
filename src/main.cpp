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

#define WIFI_SSID "ZEMARCOS"
#define WIFI_PASS "mgmm4103"

extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");    
    ESP_ERROR_CHECK(wifi_init(WIFI_SSID, WIFI_PASS));
    ESP_ERROR_CHECK(sync_time());

    size_t free_heap_size = esp_get_free_heap_size();
    ESP_LOGI(MAIN, "Free heap size: %d bytes | %d megabytes", free_heap_size, free_heap_size / 1024);

    xTaskCreate(&socket_client_connect, "socket_client_connect", 1024*4, NULL, 5, NULL);
    xTaskCreate(&dummy_reader_esp, "dummy_reader_esp", 1024*4, NULL, 5, NULL);

    // clean up the memory allocated when isn't needed anymore
    // vPortFree(sock);
}