/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0)); 

int main(void)
{
	struct sensor_value value = {0};
	
	while(1)
	{
		value.val1++;
		sensor_attr_set(dev, SENSOR_CHAN_ALL, SENSOR_ATTR_RESOLUTION, &value);
		k_msleep(1000);
	}

	return 0;
}
