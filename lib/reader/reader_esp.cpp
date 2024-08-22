#include <esp_task.h>
#include <esp_log.h>

#include <socket_esp.h>

void dummy_reader_esp(void *pvParameters) {
    
    char *TAG = "03J3DF4S";
    UBaseType_t uxHighWaterMark;

    int *sock = (int *)pvParameters;
    printf("Socket: %d\n", *sock);

    while (1) {

        ESP_LOGI("READER", "Reading card...");
        ESP_ERROR_CHECK(send_message(*sock, TAG));

        uxHighWaterMark = uxTaskGetStackHighWaterMark(NULL);
        ESP_LOGI("READER", "High water mark: %d", uxHighWaterMark);
        vTaskDelay(10000 / portTICK_PERIOD_MS);
    }

}