#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "light.h"

#define NUM_BUTTONS 2
static const gpio_num_t button_gpios[NUM_BUTTONS] = {GPIO_NUM_7, GPIO_NUM_16};
static const char *TAG = "button";

// Task to handle pull-up button inputs
void button_task(void *pvParameters)
{
    ESP_LOGI(TAG, "Button task initialized\n");
    uint8_t last_state[NUM_BUTTONS];

    for (int i = 0; i < NUM_BUTTONS; ++i)
    {
        gpio_set_direction(button_gpios[i], GPIO_MODE_INPUT);
        gpio_set_pull_mode(button_gpios[i], GPIO_PULLUP_ONLY); // Enable internal pull-up
        last_state[i] = gpio_get_level(button_gpios[i]);       // Initialize from hardware
    }

    while (1)
    {
        for (int i = 0; i < NUM_BUTTONS; ++i)
        {
            uint8_t current_state = gpio_get_level(button_gpios[i]);
            if (current_state != last_state[i])
            {
                vTaskDelay(10 / portTICK_PERIOD_MS); // Debounce delay
                current_state = gpio_get_level(button_gpios[i]);
                if (current_state != last_state[i]) // Confirm state change
                {
                    if (current_state == 0)
                    {
                        // Button pressed
                        ESP_LOGI(TAG, "Button %d Pressed!", i + 1);
                    }
                    else
                    {
                        // Button released
                        ESP_LOGI(TAG, "Button %d Released!", i + 1);
                        set_light(i, !get_light(i)); // Toggle corresponding light on button release
                    }
                    last_state[i] = current_state;
                }
            }
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void button_task_init(void)
{
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
}