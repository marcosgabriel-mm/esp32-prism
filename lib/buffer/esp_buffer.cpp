#include <stdio.h>
#include <string.h>
#include "cJSON.h"
#include "esp_log.h"
#include "time_esp.h"

#define MAX_OBJECTS 50

cJSON *json_array = NULL;
int object_count = 0;

void save_data_on_buffer(const char *uuid) {

    cJSON *json = cJSON_CreateObject();
    if (json == NULL) {
        ESP_LOGE("save_data_on_buffer", "Failed to create JSON object");
        return;
    }

    cJSON_AddStringToObject(json, "uuid", uuid);
    cJSON_AddStringToObject(json, "hour", get_format_time());

    if (json_array == NULL) {
        json_array = cJSON_CreateArray();
        if (json_array == NULL) {
            ESP_LOGE("save_data_on_buffer", "Failed to create JSON array");
            cJSON_Delete(json);
            return;
        }
    }

    cJSON_AddItemToArray(json_array, json);
    object_count++;

    if (object_count >= MAX_OBJECTS) {

        // save_buffer_to_sd();
    }
}