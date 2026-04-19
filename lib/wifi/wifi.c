#include <stdio.h>
#include <string.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/event_groups.h"
// #include "freertos/queue.h"
#include "esp_log.h"
// #include "esp_wifi.h"
// #include "esp_netif.h"
// #include "esp_event.h"
// #include "nvs_flash.h"
// #include "wifi.h"

#define TAG "wifi"

// // AP Configuration for setup mode
// #define AP_SSID "ESP32-Setup"
// #define AP_PASS "123456789"
// #define AP_CHANNEL 1
// #define AP_MAX_CONN 4

// // Storage keys for NVS
// #define WIFI_SSID_KEY "wifi_ssid"
// #define WIFI_PASS_KEY "wifi_pass"

// // Maximum number of connection attempts
// #define MAX_CONNECT_ATTEMPTS 5

// // Event group for WiFi events
// static EventGroupHandle_t wifi_event_group;
// #define WIFI_CONNECTED_BIT BIT0
// #define WIFI_FAILED_BIT BIT1
// #define WIFI_AP_ACTIVE_BIT BIT2

// // Connection attempt counter
// static int connect_attempts = 0;

// // NVS handle for WiFi credentials
// static nvs_handle_t nvs_wifi_handle;

// // Queue for credential updates
// static QueueHandle_t wifi_cred_queue;

// // Struct for passing credentials
// typedef struct {
//     char ssid[32];
//     char password[64];
// } wifi_credentials_t;

// // WiFi event handler
// static void wifi_event_handler(void *arg, esp_event_base_t event_base,
//                                 int32_t event_id, void *event_data)
// {
//     if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
//     {
//         ESP_LOGI(TAG, "WiFi station started");
//     }
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
//     {
//         ESP_LOGI(TAG, "WiFi disconnected");
//         xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT);
//         xEventGroupClearBits(wifi_event_group, WIFI_AP_ACTIVE_BIT);
//     }
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_START)
//     {
//         ESP_LOGI(TAG, "WiFi AP started - SSID: %s", AP_SSID);
//         xEventGroupSetBits(wifi_event_group, WIFI_AP_ACTIVE_BIT);
//     }
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STACONNECTED)
//     {
//         wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
//         ESP_LOGI(TAG, "Station connected to AP, MAC: " MACSTR, MAC2STR(event->mac));
//     }
//     else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_AP_STADISCONNECTED)
//     {
//         wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
//         ESP_LOGI(TAG, "Station disconnected from AP, MAC: " MACSTR, MAC2STR(event->mac));
//     }
//     else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
//     {
//         ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
//         ESP_LOGI(TAG, "Got IP address: " IPSTR, IP2STR(&event->ip_info.ip));
//         xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
//     }
// }

// // Task 1: WiFi Setup - Initialize WiFi stack and configuration
// static void wifi_setup_task(void *pvParameters)
// {
//     ESP_LOGI(TAG, "WiFi setup task started");

//     // Initialize NVS (Non-Volatile Storage)
//     esp_err_t ret = nvs_flash_init();
//     if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
//     {
//         ESP_ERROR_CHECK(nvs_flash_erase());
//         ret = nvs_flash_init();
//     }
//     ESP_ERROR_CHECK(ret);

//     // Open NVS namespace for WiFi credentials
//     ESP_ERROR_CHECK(nvs_open("wifi", NVS_READWRITE, &nvs_wifi_handle));

//     // Initialize TCP/IP stack
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());

//     // Create WiFi station and AP interfaces
//     esp_netif_create_default_wifi_sta();
//     esp_netif_create_default_wifi_ap();

//     // Initialize WiFi with default config
//     wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
//     ESP_ERROR_CHECK(esp_wifi_init(&cfg));

//     // Register event handlers
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID,
//                                                 &wifi_event_handler, NULL));
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP,
//                                                 &wifi_event_handler, NULL));

//     ESP_LOGI(TAG, "WiFi setup complete");
//     vTaskDelete(NULL);
// }

// // Task 2: WiFi Connect - Attempt to connect to WiFi
// static void wifi_connect_task(void *pvParameters)
// {
//     ESP_LOGI(TAG, "WiFi connect task started");

//     wifi_credentials_t creds;
//     char stored_ssid[32] = {0};
//     char stored_pass[64] = {0};
//     size_t ssid_len = sizeof(stored_ssid);
//     size_t pass_len = sizeof(stored_pass);

//     // Try to load stored credentials from NVS
//     esp_err_t ret_ssid = nvs_get_str(nvs_wifi_handle, WIFI_SSID_KEY, stored_ssid, &ssid_len);
//     esp_err_t ret_pass = nvs_get_str(nvs_wifi_handle, WIFI_PASS_KEY, stored_pass, &pass_len);

//     if (ret_ssid == ESP_OK && ret_pass == ESP_OK)
//     {
//         ESP_LOGI(TAG, "Found stored credentials for SSID: %s", stored_ssid);
//         strncpy(creds.ssid, stored_ssid, sizeof(creds.ssid) - 1);
//         strncpy(creds.password, stored_pass, sizeof(creds.password) - 1);
//     }
//     else
//     {
//         ESP_LOGW(TAG, "No stored credentials found, waiting for credentials from queue...");
//         // Wait for credentials to be sent via queue
//         if (xQueueReceive(wifi_cred_queue, &creds, portMAX_DELAY))
//         {
//             ESP_LOGI(TAG, "Received credentials for SSID: %s", creds.ssid);
//             // Store credentials in NVS
//             nvs_set_str(nvs_wifi_handle, WIFI_SSID_KEY, creds.ssid);
//             nvs_set_str(nvs_wifi_handle, WIFI_PASS_KEY, creds.password);
//             nvs_commit(nvs_wifi_handle);
//             ESP_LOGI(TAG, "Credentials stored in NVS");
//         }
//     }

//     // Set WiFi to station mode
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
//     ESP_ERROR_CHECK(esp_wifi_start());

//     // Configure and connect
//     wifi_config_t wifi_config = {0};
//     strncpy((char *)wifi_config.sta.ssid, creds.ssid, sizeof(wifi_config.sta.ssid) - 1);
//     strncpy((char *)wifi_config.sta.password, creds.password, sizeof(wifi_config.sta.password) - 1);

//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

//     // Try to connect with retries
//     connect_attempts = 0;
//     while (connect_attempts < MAX_CONNECT_ATTEMPTS)
//     {
//         ESP_LOGI(TAG, "Connection attempt %d/%d", connect_attempts + 1, MAX_CONNECT_ATTEMPTS);
//         esp_wifi_connect();

//         // Wait up to 10 seconds for connection
//         EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
//                                                WIFI_CONNECTED_BIT | WIFI_FAILED_BIT,
//                                                pdFALSE, pdFALSE, 10000 / portTICK_PERIOD_MS);

//         if (bits & WIFI_CONNECTED_BIT)
//         {
//             ESP_LOGI(TAG, "WiFi connected successfully");
//             break;
//         }

//         connect_attempts++;
//         if (connect_attempts < MAX_CONNECT_ATTEMPTS)
//         {
//             ESP_LOGW(TAG, "Connection failed, retrying in 5 seconds...");
//             vTaskDelay(5000 / portTICK_PERIOD_MS);
//         }
//     }

//     if (connect_attempts >= MAX_CONNECT_ATTEMPTS)
//     {
//         ESP_LOGE(TAG, "Failed to connect after %d attempts", MAX_CONNECT_ATTEMPTS);
//     }

//     vTaskDelete(NULL);
// }

// // Task 3: WiFi Monitor - Monitor connection status
// static void wifi_monitor_task(void *pvParameters)
// {
//     ESP_LOGI(TAG, "WiFi monitor task started");

//     while (1)
//     {
//         EventBits_t bits = xEventGroupGetBits(wifi_event_group);

//         if (bits & WIFI_CONNECTED_BIT)
//         {
//             // Check connection every 30 seconds when connected
//             vTaskDelay(30000 / portTICK_PERIOD_MS);
//         }
//         else
//         {
//             // Check more frequently when disconnected
//             vTaskDelay(5000 / portTICK_PERIOD_MS);
//             ESP_LOGW(TAG, "WiFi not connected, attempting reconnection...");
//             esp_wifi_connect();
//         }
//     }
// }

// // AP Setup Task - Starts AP for credential configuration
// static void wifi_ap_setup_task(void *pvParameters)
// {
//     ESP_LOGI(TAG, "WiFi AP setup task started");

//     // Configure AP
//     wifi_config_t ap_config = {
//         .ap = {
//             .ssid = AP_SSID,
//             .ssid_len = strlen(AP_SSID),
//             .password = AP_PASS,
//             .channel = AP_CHANNEL,
//             .authmode = WIFI_AUTH_WPA_WPA2_PSK,
//             .max_connection = AP_MAX_CONN,
//             .beacon_interval = 100,
//         },
//     };

//     // Set to AP mode
//     ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
//     ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &ap_config));
//     ESP_ERROR_CHECK(esp_wifi_start());

//     ESP_LOGI(TAG, "AP mode enabled - SSID: %s (password: %s)", AP_SSID, AP_PASS);
//     ESP_LOGI(TAG, "Ready to receive credentials...");

//     vTaskDelete(NULL);
// }

// Initialize all WiFi tasks
void wifi_task_init(void)
{
    // Create event group for WiFi events
    // wifi_event_group = xEventGroupCreate();

    // // Create queue for credential transfer
    // wifi_cred_queue = xQueueCreate(1, sizeof(wifi_credentials_t));

    // // Create and start the setup task
    // xTaskCreate(wifi_setup_task, "wifi_setup_task", 4096, NULL, 5, NULL);
    // vTaskDelay(1000 / portTICK_PERIOD_MS);

    // // Create and start the connect task
    // xTaskCreate(wifi_connect_task, "wifi_connect_task", 4096, NULL, 4, NULL);
    // vTaskDelay(500 / portTICK_PERIOD_MS);

    // // Create and start the monitor task
    // xTaskCreate(wifi_monitor_task, "wifi_monitor_task", 2048, NULL, 3, NULL);

    ESP_LOGI(TAG, "WiFi tasks initialized");
}

// // Public function to enable AP for credential setup
// void wifi_enable_ap_setup(void)
// {
//     ESP_LOGI(TAG, "Enabling AP setup mode...");
//     xTaskCreate(wifi_ap_setup_task, "wifi_ap_setup_task", 4096, NULL, 5, NULL);
// }

// // Public function to send credentials via queue
// void wifi_set_credentials(const char *ssid, const char *password)
// {
//     if (ssid == NULL || password == NULL)
//     {
//         ESP_LOGE(TAG, "Invalid credentials provided");
//         return;
//     }

//     wifi_credentials_t creds;
//     strncpy(creds.ssid, ssid, sizeof(creds.ssid) - 1);
//     creds.ssid[sizeof(creds.ssid) - 1] = '\0';
//     strncpy(creds.password, password, sizeof(creds.password) - 1);
//     creds.password[sizeof(creds.password) - 1] = '\0';

//     if (xQueueSend(wifi_cred_queue, &creds, 0) == pdPASS)
//     {
//         ESP_LOGI(TAG, "Credentials queued for SSID: %s", ssid);
//     }
//     else
//     {
//         ESP_LOGE(TAG, "Failed to queue credentials");
//     }
// }
