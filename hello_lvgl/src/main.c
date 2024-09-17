/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/display.h>
#include <zephyr/drivers/gpio.h>

#include <lvgl.h>
#include "lvgl_sample.h"

#define LED0_NODE DT_ALIAS(led0) 
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define LOG_LEVEL CONFIG_LOG_DEFAULT_LEVEL
#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(app);

int main(void)
{
	printf("Hello World! %s\n", CONFIG_BOARD_TARGET);

	const struct device *display_dev;

	display_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_display));
	if (!device_is_ready(display_dev)) {
		LOG_ERR("Device not ready, aborting test");
		return 0;
	}

	lv_sample();

	gpio_pin_configure_dt(&led, GPIO_OUTPUT);
	gpio_pin_set(led.port, led.pin, 1);
	display_blanking_off(display_dev);

	while (1) {
		k_sleep(K_MSEC(10));
		lv_task_handler();
	}

	return 0;
}
