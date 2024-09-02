#include <esp_task.h>
#include <esp_log.h>
#include <socket_esp.h>
#include <time_esp.h>
#include <freertos/task.h>
#include <esp_buffer.h>
#include <tags_log.h>

void dummy_reader_esp(void *pvParameters) {
    
    while (1) {

        //change the delay value to the card read interaction
        vTaskDelay(15000 / portTICK_PERIOD_MS);

        char *now = get_format_time();
        ESP_LOGI(READER, "Current time: %s", now);

        if (!send_message(now)) {
            //if the message wasn't sent, save it on a buffer
            ESP_LOGI(READER, "Saving message on buffer");
            save_data_on_buffer("dummy_uuid");            
        }
    }
}