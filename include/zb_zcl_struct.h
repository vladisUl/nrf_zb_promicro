#ifndef ZB_ZCL_STRUCT_H
#define ZB_ZCL_STRUCT_H

#include <zboss_api.h>
#include <zboss_api_addons.h>

#define NORDIC_DIY_INIT_BASIC_APP_VERSION 01
#define NORDIC_DIY_INIT_BASIC_STACK_VERSION 10
#define NORDIC_DIY_INIT_BASIC_HW_VERSION 11
#define NORDIC_DIY_INIT_BASIC_SW_VERSION "build-3"
#define NORDIC_DIY_INIT_BASIC_MANUF_NAME "Nordic"
#define NORDIC_DIY_INIT_BASIC_MODEL_ID "nordic DIY PM"
#define NORDIC_DIY_INIT_BASIC_DATE_CODE "041426"
#define NORDIC_DIY_INIT_BASIC_POWER_SOURCE ZB_ZCL_BASIC_POWER_SOURCE_BATTERY
#define NORDIC_DIY_INIT_BASIC_LOCATION_DESC ""
#define NORDIC_DIY_INIT_BASIC_PH_ENV ZB_ZCL_BASIC_ENV_UNSPECIFIED

#define ATTR_TEMP_MIN (-4000)
#define ATTR_TEMP_MAX (8500)
#define ATTR_TEMP_TOLERANCE (100)

#define ATTR_HUM_MIN (0)
#define ATTR_HUM_MAX (10000)

#define ATTR_PRESSURE_MIN (3000)
#define ATTR_PRESSURE_MAX (11000)
#define ATTR_PRESSURE_TOLERANCE (1)

#define ENDPOINT_NUM 1
#define ERASE_PERSISTENT_CONFIG ZB_FALSE

#define READ_DATA_INITIAL_DELAY K_SECONDS(30)
#define READ_DATA_TIMER_PERIOD K_SECONDS(60)
#define LONG_POLL_INTERVAL 600000


typedef struct zb_zcl_power_attrs zb_zcl_power_attrs_t;

struct zb_zcl_power_attrs
{
    zb_uint8_t voltage;
    zb_uint8_t percent_remaining;
};

typedef struct
{
    zb_int16_t measure_value;
    zb_int16_t min_measure_value;
    zb_int16_t max_measure_value;
} zb_zcl_humidity_measurement_attrs_t;

typedef struct
{
    zb_int16_t measure_value;
    zb_int16_t min_measure_value;
    zb_int16_t max_measure_value;
    zb_int16_t tolerance_value;
} zb_zcl_pressure_measurement_attrs_t;

typedef struct
{
    zb_uint32_t checkin_interval;
    zb_uint32_t long_poll_interval;
    zb_uint16_t short_poll_interval;
    zb_uint16_t fast_poll_timeout;
    zb_uint32_t checkin_interval_min;
    zb_uint32_t long_poll_interval_min;
    zb_uint16_t fast_poll_timeout_max;
} zb_zcl_poll_control_attrs_t;

struct zb_device_ctx
{
    zb_zcl_basic_attrs_ext_t basic_attr;
    zb_zcl_identify_attrs_t identify_attr;
    zb_zcl_temp_measurement_attrs_t temp_measure_attrs;
    zb_zcl_humidity_measurement_attrs_t humidity_measure_attrs;
    zb_zcl_pressure_measurement_attrs_t pressure_measure_attrs;
    zb_zcl_power_attrs_t power_attr;
    zb_zcl_poll_control_attrs_t poll_control_attrs;
};



extern struct zb_device_ctx dev_ctx;

extern zb_af_device_ctx_t NORDIC_DIY_ctx;
extern zb_af_endpoint_desc_t NORDIC_DIY_ep;

void app_clusters_attr_init(void);   

#endif /* ZB_ZCL_STRUCT_H */