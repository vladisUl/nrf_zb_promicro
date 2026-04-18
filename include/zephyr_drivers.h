#ifndef ZEPHYR_DRIVERS_H
#define ZEPHYR_DRIVERS_H

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/sensor_data_types.h>
#include <zephyr/drivers/adc.h>

#define SW0_NODE DT_ALIAS(sw0)
#define MY_BUTTON_MASK BIT(0)
#define LED0_NODE DT_ALIAS(led0)
#define MY_LED_MASK BIT(0)

extern const struct gpio_dt_spec button;
extern const struct gpio_dt_spec led;
extern struct k_timer led_timer;

#define LED_BLINK(start_delay, period) \
    k_timer_start(&led_timer, start_delay, period)

#define LED_BLINK_STOP() \
    k_timer_stop(&led_timer)

void set_led_on(const struct gpio_dt_spec *led);
void set_led_off(const struct gpio_dt_spec *led);
void led_timer_handler(struct k_timer *timer);
const struct device *check_bme280_device(void);
int16_t read_bme280(int16_t *temperature, int16_t *pressure, int16_t *humidity);
int init_vdd(void);
int read_vdd_mv(int16_t *mv);
uint8_t cr2032_CalculateLevel(uint16_t voltage);

#endif /* ZEPHYR_DRIVERS_H */