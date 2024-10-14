/*
 * Copyright (c) 2024 Nordic Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/net/net_if.h>
#include <zephyr/net/net_mgmt.h>
#include <zephyr/net/wifi_mgmt.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(wifi_event_handler);

#if defined(CONFIG_NET_CONNECTION_MANAGER)

static K_SEM_DEFINE(network_connected, 0, 1);

static void wifi_event_handler(struct net_mgmt_event_callback *cb,
                                uint32_t mgmt_event,
                                struct net_if *iface) {
    if (mgmt_event == NET_EVENT_WIFI_CONNECT_RESULT) {
        LOG_INF("Connected to Wi-Fi network");
		k_sem_give(&network_connected);
    } else if (mgmt_event == NET_EVENT_WIFI_DISCONNECT_RESULT) {
        LOG_INF("Disconnected from Wi-Fi network");
    }
}

void wait_for_wifi(void)
{
	struct net_mgmt_event_callback wifi_cb;

    // Check if Wi-Fi is connected
    struct net_if *iface = net_if_get_default();
    if (iface) {
        if (net_if_is_up(iface)) {
            LOG_INF("Wi-Fi is connected");
        } else {
            LOG_INF("Wi-Fi is not connected");

            // Register the Wi-Fi event callback
            net_mgmt_init_event_callback(&wifi_cb, wifi_event_handler, 
                                        NET_EVENT_WIFI_CONNECT_RESULT | NET_EVENT_WIFI_DISCONNECT_RESULT);
            net_mgmt_add_event_callback(&wifi_cb);

            LOG_INF("Waiting for Wi-Fi connection");

            k_sem_take(&network_connected, K_FOREVER);
        }
    } else {
        LOG_ERR("No network interface found");
    }

}
#endif /* CONFIG_NET_CONNECTION_MANAGER */