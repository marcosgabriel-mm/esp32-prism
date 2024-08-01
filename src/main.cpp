#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"

#include <tags_log.h>
#include <wifi_esp.h>
#include <http_esp.h>



extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");
    ESP_ERROR_CHECK(wifi_init());

    // xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    xTaskCreate(&http_get_task, "http_get_task", 8192, NULL, 5, NULL);
}