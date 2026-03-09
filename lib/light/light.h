#ifndef LIGHT_H
#define LIGHT_H

#include <stdbool.h>

#define NUM_LIGHTS 2

typedef enum
{
    LIGHT_ONE = 0,
    LIGHT_TWO = 1
} light_id_t;

void set_light(light_id_t light, bool on);
bool get_light(light_id_t light);
void light_task_init(void);

#endif // LIGHT_H