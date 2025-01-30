#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_log.h"
#include "esp_err.h"
#include "esp_netif.h"

#include <tags_log.h>
#include <wifi_esp.h>
#include <http_esp.h>
#include <eeprom_esp.h>
#include <wifi_credentials_page/wifi_credentials.h>

#include <socket_esp.h>
#include <reader_esp.h>
#include <time_esp.h>

extern "C" void app_main() {
    
    ESP_LOGI(MAIN, "Starting ESP32 application...");    
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    setup_eeprom();

    if (!has_wifi_configured()) {
        start_wifi_ap();
        start_webserver();

        while (!wifi_credentials().connected) {
            vTaskDelay(1000 / portTICK_PERIOD_MS);
            ESP_LOGI(MAIN, "Waiting for the user to configure the wifi...");
        }

        wifi_credentials_t credentials = wifi_credentials();
        ESP_LOGI(MAIN, "SSID: %s | Password: %s", credentials.ssid, credentials.password);
        
        save_wifi_credentials(credentials.ssid, credentials.password);
        set_wifi_configured(0);
        
        esp_restart();
    }

    wifi_credentials_t credentials = load_wifi_credentials();
    ESP_ERROR_CHECK(wifi_init(credentials.ssid, credentials.password));
    ESP_ERROR_CHECK(sync_time());

    size_t free_heap_size = esp_get_free_heap_size();
    ESP_LOGI(MAIN, "Free heap size: %d bytes | %d megabytes", free_heap_size, free_heap_size / 1024);


    // create task to get updated users from the server, in 6:6 hours
    // create task to when the user hold the boot button for 3 seconds, the device erase the wifi credentials
    // fucntion to reset EEPROM: reset_wifi_credentials();
    // xTaskCreate(&dummy_reader_esp, "dummy_reader_esp", 1024*4, NULL, 5, NULL); // task to simulate the RFID reader
}