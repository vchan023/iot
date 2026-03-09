#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "button.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define BUTTON_GPIO GPIO_NUM_16
static const char *TAG = "button";

// Task to handle pull-up button input
void button_task(void *pvParameters)
{
    gpio_set_direction(GPIO_NUM_16, GPIO_MODE_INPUT);
    uint8_t last_state = 1;

    while (1)
    {
        // Button handling logic goes here
        uint8_t current_state = gpio_get_level(BUTTON_GPIO);

        if (current_state != last_state)
        {
            if (current_state == 0)
            {
                // Button pressed
                ESP_LOGI(TAG, "Button Pressed!");
            }
            else
            {
                // Button released
                ESP_LOGI(TAG, "Button Released!");
            }
            last_state = current_state;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void button_task_init(void)
{
    xTaskCreate(button_task, "button_task", 2048, NULL, 5, NULL);
}