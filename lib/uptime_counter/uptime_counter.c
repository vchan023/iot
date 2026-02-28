#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uptime_counter.h"

static void uptime_counter_task(void *pvParameters)
{
    uint16_t counter = 0;
    while (1)
    {
        printf("Run Time: %d Seconds\n", counter++);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void uptime_counter_start(void)
{
    xTaskCreate(uptime_counter_task, "UptimeCounter", 2048, NULL, 5, NULL);
}
