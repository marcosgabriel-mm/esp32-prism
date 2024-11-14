#include <nvs_flash.h>
#include <nvs.h>

void setup_eeprom() {
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}

nvs_handle_t eeprom_handler() {
    nvs_handle_t my_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READWRITE, &my_handle);
    if (err != ESP_OK) {
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    } 

    return my_handle;
}

bool has_wifi_configured() {
    nvs_handle_t handler = eeprom_handler();
    uint8_t has_data = 0;

    esp_err_t err = nvs_get_u8(handler, "has_data", &has_data);
    nvs_close(handler);

    if (err != ESP_OK) {
        printf("Error (%s) reading!\n", esp_err_to_name(err));
    }

    return has_data;
}