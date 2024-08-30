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

void cardPass(void *param) {

    int sock = *(int *)param;
    const char *message = "Card detected : U3JD56S";
    
    if (send_message(sock, (char *)message)) {
        // if the message was sent successfully, print a message
        ESP_LOGI(MAIN, "Message sent successfully");
    } else {
        // if the message was not sent successfully, storage the message to send it later
        ESP_LOGE(MAIN, "Error sending message");
    }
    
    vTaskDelay(6000 / portTICK_PERIOD_MS);
}

extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");    
    ESP_ERROR_CHECK(wifi_init());

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    ESP_ERROR_CHECK(socket_client_connect(sock));

    // if a card is detected, call the function to send the message to the server
    // xTaskCreate(&cardPass, "cardPass", 4096, &sock, 5, NULL);

    // xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT, pdFALSE, pdTRUE, portMAX_DELAY);
    // xTaskCreate(&http_get_task, "http_get_task", 8192, NULL, 5, NULL);
    // xTaskCreate(&socket_client_task, "tcp_client_task", 4096, NULL, 5, NULL);
}