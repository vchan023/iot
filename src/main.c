

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "uptime_counter.h"

#define RED_LIGHT_GPIO

static const char *TAG = "main";

void app_main()
{
    printf("Hello, Isabel!\n");

    uptime_counter_start();
}