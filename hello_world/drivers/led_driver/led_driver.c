#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/logging/log.h>
#include <zephyr/device.h>

#define DT_DRV_COMPAT adam_led_driver

LOG_MODULE_REGISTER(led_driver, LOG_LEVEL_DBG);

struct led_driver_data
{
	struct gpio_dt_spec led;
};

static int channel_get_impl(const struct device *dev,
                              enum sensor_channel chan,
                              struct sensor_value *val)
{
	LOG_INF("sensor_channel_get called");

    struct led_driver_data *data = dev->data;
	gpio_pin_set_dt(&data->led, 1);

	return 0;
}

static int sample_fetch_impl(const struct device *dev)
{
	LOG_INF("sensor_sample_fetch called");

    struct led_driver_data *data = dev->data;
	gpio_pin_set_dt(&data->led, 0);

	return 0;
}

static DEVICE_API(sensor, api_adam_custom) =
{
	.channel_get  = channel_get_impl,
	.sample_fetch = sample_fetch_impl,
};

static int init(const struct device *dev)
{
	struct led_driver_data *data = dev->data;
	int ret = 0;

	if (!gpio_is_ready_dt(&data->led))
	{
		LOG_ERR("LED GPIO not ready");
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&data->led, GPIO_OUTPUT_INACTIVE);
	if (ret < 0)
	{
		LOG_ERR("Failed to configure LED GPIO: %d", ret);
		return ret;
	}

	LOG_INF("Initialized");
	return 0;
}

#define LED_DRIVER_DEFINE(inst)                                   \
	static struct led_driver_data led_driver_data_##inst =        \
	{                                                             \
		.led   = GPIO_DT_SPEC_INST_GET(inst, gpios),              \
	};                                                            \
	DEVICE_DT_INST_DEFINE(inst, init, NULL,                       \
	                      &led_driver_data_##inst, NULL,          \
	                      POST_KERNEL, 100, &api_adam_custom);

DT_INST_FOREACH_STATUS_OKAY(LED_DRIVER_DEFINE)
