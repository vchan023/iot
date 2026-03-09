#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "light.h"
#include "driver/gpio.h"
#include <stdbool.h>

static const gpio_num_t light_gpios[NUM_LIGHTS] = {GPIO_NUM_4, GPIO_NUM_5};
#define TAG "light"

static bool light_states[NUM_LIGHTS] = {false};
static SemaphoreHandle_t light_mutexes[NUM_LIGHTS];

void set_light(light_id_t light, bool on)
{
    if (light < 0 || light >= NUM_LIGHTS)
        return;
    xSemaphoreTake(light_mutexes[light], portMAX_DELAY);
    light_states[light] = on;
    xSemaphoreGive(light_mutexes[light]);
}

bool get_light(light_id_t light)
{
    if (light < 0 || light >= NUM_LIGHTS)
        return false;
    xSemaphoreTake(light_mutexes[light], portMAX_DELAY);
    bool state = light_states[light];
    xSemaphoreGive(light_mutexes[light]);
    return state;
}

// Task to control light behavior for all lights
static void light_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Light task initialized\n");
    for (int i = 0; i < NUM_LIGHTS; ++i)
    {
        gpio_set_direction(light_gpios[i], GPIO_MODE_OUTPUT);
        light_mutexes[i] = xSemaphoreCreateMutex();
    }

    while (1)
    {
        for (int i = 0; i < NUM_LIGHTS; ++i)
        {
            int level = get_light(i) ? 1 : 0;
            gpio_set_level(light_gpios[i], level);
        }
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

void light_task_init(void)
{
    xTaskCreate(light_task, "light_task", 2048, NULL, 5, NULL);
}
