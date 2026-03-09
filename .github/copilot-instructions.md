# Copilot Instructions for ESP32 FreeRTOS Project

## Project Overview

This project is an ESP32 application using PlatformIO and FreeRTOS. The main logic is in `src/main.c`, which now initializes modular FreeRTOS tasks for uptime, button, and light control.

## Key Files and Directories

- `src/main.c`: Main application logic and task initialization.
- `platformio.ini`: PlatformIO configuration for ESP32.
- `CMakeLists.txt`: CMake build configuration.
- `lib/uptime/uptime.c`, `lib/uptime/uptime.h`: Uptime task implementation and interface.
- `lib/button/button.c`, `lib/button/button.h`: Button task implementation and interface.
- `lib/light/light.c`, `lib/light/light.h`: Light control task implementation and interface.
- `graveyard/`: For deprecated or removed code kept for reference.

## Coding Guidelines for Copilot/AI Agents

- Use FreeRTOS APIs for multitasking and timing.
- Place new application logic or tasks in `app_main()` or as separate FreeRTOS tasks.
- Use `vTaskDelay()` for periodic actions.
- Keep code modular: add new features as separate libraries under `lib/` with their own `.c` and `.h` files.
- Initialize new tasks in `app_main()` using a `*_task_init()` convention (e.g., `uptime_task_init()`).
- Add comments for clarity.
- Use PlatformIO for building and uploading firmware.

## For Future AI Agents

- Summarize any new features, tasks, or libraries you add in this file.
- Update this file with any project-specific conventions or requirements.
- If you add new files or libraries, document their purpose here.

---

_Last updated: 2026-03-09_
