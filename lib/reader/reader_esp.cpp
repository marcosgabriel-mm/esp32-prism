#include <esp_task.h>
#include <esp_log.h>
#include <socket_esp.h>
#include <time_esp.h>

void dummy_reader_esp(void *pvParameters) {
    
    int *sock = (int *)pvParameters;
    while (1) {

        char *now = get_format_time();
        ESP_LOGI("READER", "Current time: %s", now);

        if (!send_message(*sock, now)) {
            //if the message wasn't sent, save it on a buffer
            ESP_LOGI("READER", "Saving message on buffer");
        }

        //change the delay value to the card read interaction
        vTaskDelay(20000 / portTICK_PERIOD_MS);
    }

}