#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "light.h"
#include "driver/gpio.h"

#define GPIO_LIGHT_ONE GPIO_NUM_17
#define TAG "light"

static bool light_one_on = false;
static SemaphoreHandle_t light_one_mutex;

void set_light_one(bool on)
{
    xSemaphoreTake(light_one_mutex, portMAX_DELAY);
    light_one_on = on;
    xSemaphoreGive(light_one_mutex);
}

bool get_light_one()
{
    xSemaphoreTake(light_one_mutex, portMAX_DELAY);
    bool state = light_one_on;
    xSemaphoreGive(light_one_mutex);
    return state;
}

// Task to control light behavior
static void light_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Light task initialized\n");
    gpio_set_direction(GPIO_LIGHT_ONE, GPIO_MODE_OUTPUT);
    light_one_mutex = xSemaphoreCreateMutex();

    while (1)
    {
        if (get_light_one())
        {
            gpio_set_level(GPIO_LIGHT_ONE, 1);
        }
        else
        {
            gpio_set_level(GPIO_LIGHT_ONE, 0);
        }

        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void light_task_init(void)
{
    xTaskCreate(light_task, "light_task", 2048, NULL, 5, NULL);
}
