#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uptime.h"
#include "esp_log.h"

#define TAG "uptime"

// Task to track uptime in seconds
static void uptime_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Uptime task initialized\n");
    uint16_t counter = 0;
    while (1)
    {
        ESP_LOGI(TAG, "Run Time: %d Seconds\n", counter++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uptime_task_init(void)
{
    xTaskCreate(uptime_task, "uptime_task", 2048, NULL, 5, NULL);
}
