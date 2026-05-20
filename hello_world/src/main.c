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
#include <zephyr/shell/shell.h>

LOG_MODULE_REGISTER(main, LOG_LEVEL_DBG);

const struct device *dev = DEVICE_DT_GET(DT_NODELABEL(led_driver0)); 

static int cmd_version(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Version is 1.0.0");
	return 0;
}

static int cmd_fetch(const struct shell *sh, size_t argc, char **argv)
{
	return sensor_sample_fetch(dev);
}

static int cmd_read(const struct shell *sh, size_t argc, char **argv)
{
    struct sensor_value value = {0};
	int ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &value);
	shell_print(sh, "Value = %d", value.val1);
	return ret;
}	

static int cmd_info(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Device name: %s", dev->name);
	shell_print(sh, "Device ready: %s", dev->state->initialized ? "Yes" : "No");
	return 0;
}

static int sub_cmd_set(const struct shell *sh, size_t argc, char **argv)
{
	if (argc != 2)
	{
		shell_error(sh, "Invalid number of subcommand arguments, expecting 1");
		return -EINVAL;
	}

	struct sensor_value value = {0};
	value.val1 = strtol(argv[1], NULL, 10);

	if (value.val1 == 0)
	{
		shell_error(sh, "Invalid value argument format");
		return -EINVAL;
	}

	return sensor_attr_set(dev, SENSOR_CHAN_ALL, SENSOR_ATTR_RESOLUTION, &value);
}

SHELL_CMD_REGISTER(version, NULL, "Print application version", cmd_version);
SHELL_CMD_REGISTER(fetch, NULL, "Fetch sensor sample", cmd_fetch);
SHELL_CMD_REGISTER(read, NULL, "Read sensor channel value", cmd_read);
SHELL_CMD_REGISTER(info, NULL, "Print device information and ready state", cmd_info);

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmds, 
	SHELL_CMD(set, NULL, "Set sensor attribute value", sub_cmd_set),
	SHELL_SUBCMD_SET_END
);
SHELL_CMD_ARG_REGISTER(sensor, &sensor_subcmds, "Sensor commands", NULL, 2, 0);

int main(void)
{
	while(1)
	{
		k_sleep(K_FOREVER);
	}

	return 0;
}
