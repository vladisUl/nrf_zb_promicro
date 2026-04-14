#include "zephyr_drivers.h"
#include <zephyr/logging/log.h>
#include <zephyr/pm/device.h>

const struct device *leds = DEVICE_DT_GET(DT_NODELABEL(leds));

LOG_MODULE_REGISTER(zephyr_drivers, LOG_LEVEL_INF);

const struct device *const dev_bme280 = DEVICE_DT_GET_ANY(bosch_bme280);

SENSOR_DT_READ_IODEV(iodev, DT_COMPAT_GET_ANY_STATUS_OKAY(bosch_bme280),
                     {SENSOR_CHAN_AMBIENT_TEMP, 0},
                     {SENSOR_CHAN_HUMIDITY, 0},
                     {SENSOR_CHAN_PRESS, 0});

RTIO_DEFINE(ctx, 1, 1);

#define VDD_CH_NODE DT_NODELABEL(vdd_channel)

const struct device *adc_dev = DEVICE_DT_GET(DT_PARENT(VDD_CH_NODE));
static const struct adc_channel_cfg vdd_ch_cfg = ADC_CHANNEL_CFG_DT(VDD_CH_NODE);

void set_led_off(const struct gpio_dt_spec *led)
{
    gpio_pin_set_dt(led, 0);
}

void set_led_on(const struct gpio_dt_spec *led)
{
    gpio_pin_set_dt(led, 1);
}

void led_timer_handler(struct k_timer *timer)
{
    ARG_UNUSED(timer);

    static int led_on_state;
    gpio_pin_set_dt(&led, !((++led_on_state) % 100));
    k_timer_start(&led_timer, K_MSEC(10), K_NO_WAIT);
}

typedef struct
{
    uint16_t voltage;
    uint8_t capacity;
} voltage_capacity;

static voltage_capacity volt_cap[] =
    {{3000, 200}, {2900, 160}, {2800, 120}, {2700, 80}, {2600, 60}, {2500, 40}, {2400, 20}, {2000, 0}};

static int bme280_suspend_if_needed(void)
{
    enum pm_device_state state;
    int err;

    if (dev_bme280 == NULL)
    {
        return -ENODEV;
    }

    err = pm_device_state_get(dev_bme280, &state);
    if (err)
    {
        return err;
    }

    if (state == PM_DEVICE_STATE_SUSPENDED)
    {
        return 0;
    }

    if (state == PM_DEVICE_STATE_SUSPENDING)
    {
        return -EBUSY;
    }

    return pm_device_action_run(dev_bme280, PM_DEVICE_ACTION_SUSPEND);
}

static int bme280_resume_if_needed(void)
{
    enum pm_device_state state;
    int err;

    if (dev_bme280 == NULL)
    {
        return -ENODEV;
    }

    err = pm_device_state_get(dev_bme280, &state);
    if (err)
    {
        return err;
    }

    if (state == PM_DEVICE_STATE_ACTIVE)
    {
        return 0;
    }

    return pm_device_action_run(dev_bme280, PM_DEVICE_ACTION_RESUME);
}

const struct device *check_bme280_device(void)
{
    int err;
    if (dev_bme280 == NULL)
    {
        LOG_INF("Error: no device found");
        return NULL;
    }

    if (!device_is_ready(dev_bme280))
    {
        LOG_INF("Error: Device \"%s\" is not ready; "
                "check the driver initialization logs for errors",
                dev_bme280->name);
        return NULL;
    }

    err = bme280_suspend_if_needed();
    if (err && err != -EALREADY)
    {
        LOG_INF("BME280 initial suspend failed: %d", err);
        return NULL;
    }

    return dev_bme280;
}

int16_t read_bme280(int16_t *temperature, int16_t *pressure, int16_t *humidity)
{
    struct sensor_value val;
    int res;
    int pm_res;
    bool resumed = false;

    if (dev_bme280 == NULL)
    {
        LOG_ERR("bme280 device not ready");
        return -ENODEV;
    }

    pm_res = bme280_resume_if_needed();
    if (pm_res)
    {
        LOG_ERR("BME280 resume failed: %d", pm_res);
        return pm_res;
    }
    resumed = true;

    res = sensor_sample_fetch(dev_bme280);
    if (res != 0)
    {
        LOG_INF("sample_fetch() failed: %d", res);
        goto out;
    }

    res = sensor_channel_get(dev_bme280, SENSOR_CHAN_AMBIENT_TEMP, &val);
    if (res != 0)
    {
        LOG_INF("temp channel_get() failed: %d", res);
        goto out;
    }
    *temperature = val.val1 * 100 + val.val2 / 10000;

    res = sensor_channel_get(dev_bme280, SENSOR_CHAN_PRESS, &val);
    if (res != 0)
    {
        LOG_INF("press channel_get() failed: %d", res);
        goto out;
    }
    *pressure = val.val1 * 100 + val.val2 / 10000;

    res = sensor_channel_get(dev_bme280, SENSOR_CHAN_HUMIDITY, &val);
    if (res != 0)
    {
        LOG_INF("hum channel_get() failed: %d", res);
        goto out;
    }
    *humidity = val.val1 * 100 + val.val2 / 10000;

    res = 0;

out:
    if (resumed)
    {
        pm_res = bme280_suspend_if_needed();
        if (pm_res && pm_res != -EALREADY)
        {
            LOG_ERR("BME280 suspend failed: %d", pm_res);
            if (res == 0)
            {
                res = pm_res;
            }
        }
    }

    return res;
}

int init_vdd(void)
{
    int err;

    if (!device_is_ready(adc_dev))
    {
        LOG_ERR("ADC device not ready");
        return -ENODEV;
    }

    err = adc_channel_setup(adc_dev, &vdd_ch_cfg);
    if (err)
    {
        LOG_ERR("adc_channel_setup failed: %d", err);
        return err;
    }

    return 0;
}

int read_vdd_mv(int16_t *mv)
{
    int err;
    uint16_t raw;

    struct adc_sequence seq = {
        .channels = BIT(vdd_ch_cfg.channel_id),
        .buffer = &raw,
        .buffer_size = sizeof(raw),
        .resolution = 12,
    };

    err = adc_read(adc_dev, &seq);
    if (err)
    {
        LOG_ERR("adc_read failed: %d", err);
        return err;
    }

    /* gain 1/6 + internal ref -> full scale about 3.6V */
    *mv = (int32_t)raw * 3600 / 4095;
    return 0;
}

uint8_t cr2032_CalculateLevel(uint16_t voltage)
{
    uint32_t res = 0;
    uint8_t i;

    for (i = 0; i < (sizeof(volt_cap) / sizeof(voltage_capacity)); i++)
    {
        if (voltage > volt_cap[i].voltage)
        {
            if (i == 0)
            {
                return volt_cap[0].capacity;
            }
            else
            {
                res = (voltage - volt_cap[i].voltage) * (volt_cap[i - 1].capacity - volt_cap[i].capacity) / (volt_cap[i - 1].voltage - volt_cap[i].voltage);
                res += volt_cap[i].capacity;
                return (uint8_t)res;
            }
        }
    }
    // Below the minimum voltage in the table.
    return volt_cap[sizeof(volt_cap) / sizeof(voltage_capacity) - 1].capacity;
}