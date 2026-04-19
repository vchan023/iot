#ifndef WIFI_H
#define WIFI_H

/**
 * Initialize WiFi manager with three tasks:
 * - wifi_setup_task: Initializes WiFi stack and NVS
 * - wifi_connect_task: Attempts to connect to configured network
 * - wifi_monitor_task: Monitors connection status
 */
void wifi_task_init(void);

// /**
//  * Enable AP mode for credential setup
//  * Creates an open Access Point where credentials can be received
//  */
// void wifi_enable_ap_setup(void);

// /**
//  * Set WiFi credentials via queue
//  * Can be called from other components to provide SSID and password
//  *
//  * @param ssid SSID of the network to connect to (max 31 chars)
//  * @param password Password for the network (max 63 chars)
//  */
// void wifi_set_credentials(const char *ssid, const char *password);

#endif // WIFI_H
