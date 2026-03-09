

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include <uptime.h>
#include <button.h>
#include <light.h>
// #include <wifi.h>

static const char *TAG = "main";

void app_main()
{
    ESP_LOGI(TAG, "Starting application...");

    uptime_task_init();
    button_task_init();
    light_task_init();
    // wifi_task_init();
}