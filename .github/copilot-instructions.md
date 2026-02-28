# Copilot Instructions for ESP32 FreeRTOS Project

## Project Overview
This project is an ESP32 application using PlatformIO and FreeRTOS. The main logic is in `src/main.c`, where a runtime counter is printed every second using a FreeRTOS task loop.

## Key Files
- `src/main.c`: Main application logic.
- `platformio.ini`: PlatformIO configuration for ESP32.
- `CMakeLists.txt`: CMake build configuration.

## Coding Guidelines for Copilot/AI Agents
- Use FreeRTOS APIs for multitasking and timing.
- Place new application logic or tasks in `app_main()` or as separate FreeRTOS tasks.
- Use `vTaskDelay()` for periodic actions.
- Keep code modular and add comments for clarity.
- Use PlatformIO for building and uploading firmware.

## Example Task Loop
```
uint16_t counter = 0;
while (1) {
    printf("Run Time: %d Seconds\n", counter++);
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}
```

## For Future AI Agents
- Summarize any new features or tasks you add in this file.
- Update this file with any project-specific conventions or requirements.
- If you add new files or libraries, document their purpose here.

---
_Last updated: 2026-02-28_