#ifndef HTTP_ESP_H
#define HTTP_ESP_H

#include <esp_http_client.h>

esp_err_t _http_event_handler(esp_http_client_event_t *evt);
void http_get_task(void* pvParameters);

#endif