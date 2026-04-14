#include "zb_zcl_struct.h"
#include "zb_range_extender.h"

struct zb_device_ctx dev_ctx;

ZB_ZCL_DECLARE_BASIC_ATTRIB_LIST_EXT(
    basic_attr_list,
    &dev_ctx.basic_attr.zcl_version,
    &dev_ctx.basic_attr.app_version,
    &dev_ctx.basic_attr.stack_version,
    &dev_ctx.basic_attr.hw_version,
    dev_ctx.basic_attr.mf_name,
    dev_ctx.basic_attr.model_id,
    dev_ctx.basic_attr.date_code,
    &dev_ctx.basic_attr.power_source,
    dev_ctx.basic_attr.location_id,
    &dev_ctx.basic_attr.ph_env,
    dev_ctx.basic_attr.sw_ver);

ZB_ZCL_DECLARE_IDENTIFY_CLIENT_ATTRIB_LIST(
    identify_client_attr_list);

ZB_ZCL_DECLARE_IDENTIFY_SERVER_ATTRIB_LIST(
    identify_server_attr_list,
    &dev_ctx.identify_attr.identify_time);

#define ZB_ZCL_DECLARE_POWER_CONFIG_VOLTAGE_PERCENT_ATTRIB_LIST(attr_list, voltage, remaining)        \
    ZB_ZCL_START_DECLARE_ATTRIB_LIST_CLUSTER_REVISION(attr_list, ZB_ZCL_POWER_CONFIG)                 \
    ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_VOLTAGE_ID(voltage, ),                    \
        ZB_SET_ATTR_DESCR_WITH_ZB_ZCL_ATTR_POWER_CONFIG_BATTERY_PERCENTAGE_REMAINING_ID(remaining, ), \
        ZB_ZCL_FINISH_DECLARE_ATTRIB_LIST;

ZB_ZCL_DECLARE_POWER_CONFIG_VOLTAGE_PERCENT_ATTRIB_LIST(
    power_config_server_attr_list,
    &dev_ctx.power_attr.voltage,
    &dev_ctx.power_attr.percent_remaining);

ZB_ZCL_DECLARE_POLL_CONTROL_ATTRIB_LIST(
    poll_control_attrib_list,
    &dev_ctx.poll_control_attrs.checkin_interval,
    &dev_ctx.poll_control_attrs.long_poll_interval,
    &dev_ctx.poll_control_attrs.short_poll_interval,
    &dev_ctx.poll_control_attrs.fast_poll_timeout,
    &dev_ctx.poll_control_attrs.checkin_interval_min,
    &dev_ctx.poll_control_attrs.long_poll_interval_min,
    &dev_ctx.poll_control_attrs.fast_poll_timeout_max);

ZB_ZCL_DECLARE_TEMP_MEASUREMENT_ATTRIB_LIST(
    temp_measurement_attr_list,
    &dev_ctx.temp_measure_attrs.measure_value,
    &dev_ctx.temp_measure_attrs.min_measure_value,
    &dev_ctx.temp_measure_attrs.max_measure_value,
    &dev_ctx.temp_measure_attrs.tolerance);

ZB_ZCL_DECLARE_REL_HUMIDITY_MEASUREMENT_ATTRIB_LIST(
    humidity_measurement_attr_list,
    &dev_ctx.humidity_measure_attrs.measure_value,
    &dev_ctx.humidity_measure_attrs.min_measure_value,
    &dev_ctx.humidity_measure_attrs.max_measure_value);

ZB_ZCL_DECLARE_PRESSURE_MEASUREMENT_ATTRIB_LIST(
    pressure_measurement_attr_list,
    &dev_ctx.pressure_measure_attrs.measure_value,
    &dev_ctx.pressure_measure_attrs.min_measure_value,
    &dev_ctx.pressure_measure_attrs.max_measure_value,
    &dev_ctx.pressure_measure_attrs.tolerance_value);

ZB_DECLARE_NORDIC_DIY_CLUSTER_LIST(
    NORDIC_DIY_clusters,
    basic_attr_list,
    identify_server_attr_list,
    power_config_server_attr_list,
    temp_measurement_attr_list,
    humidity_measurement_attr_list,
    pressure_measurement_attr_list,
    identify_client_attr_list,
    poll_control_attrib_list);

ZB_DECLARE_NORDIC_DIY_EP(
    NORDIC_DIY_ep,
    ENDPOINT_NUM,
    NORDIC_DIY_clusters);

ZBOSS_DECLARE_DEVICE_CTX_1_EP(
    NORDIC_DIY_ctx,
    NORDIC_DIY_ep);

void app_clusters_attr_init(void)
{
    ZB_AF_REGISTER_DEVICE_CTX(&NORDIC_DIY_ctx);

    // Basic cluster attributes data.
    dev_ctx.basic_attr.zcl_version = ZB_ZCL_VERSION;
    dev_ctx.basic_attr.power_source = NORDIC_DIY_INIT_BASIC_POWER_SOURCE;
    dev_ctx.basic_attr.stack_version = NORDIC_DIY_INIT_BASIC_STACK_VERSION;
    dev_ctx.basic_attr.hw_version = NORDIC_DIY_INIT_BASIC_HW_VERSION;

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.mf_name,
                          NORDIC_DIY_INIT_BASIC_MANUF_NAME,
                          ZB_ZCL_STRING_CONST_SIZE(NORDIC_DIY_INIT_BASIC_MANUF_NAME));

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.model_id,
                          NORDIC_DIY_INIT_BASIC_MODEL_ID,
                          ZB_ZCL_STRING_CONST_SIZE(NORDIC_DIY_INIT_BASIC_MODEL_ID));

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.date_code,
                          NORDIC_DIY_INIT_BASIC_DATE_CODE,
                          ZB_ZCL_STRING_CONST_SIZE(NORDIC_DIY_INIT_BASIC_DATE_CODE));

    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.location_id,
                          NORDIC_DIY_INIT_BASIC_LOCATION_DESC,
                          ZB_ZCL_STRING_CONST_SIZE(NORDIC_DIY_INIT_BASIC_LOCATION_DESC));

    dev_ctx.basic_attr.ph_env = NORDIC_DIY_INIT_BASIC_PH_ENV;
    dev_ctx.basic_attr.app_version = NORDIC_DIY_INIT_BASIC_APP_VERSION;
    ZB_ZCL_SET_STRING_VAL(dev_ctx.basic_attr.sw_ver,
                          NORDIC_DIY_INIT_BASIC_SW_VERSION,
                          ZB_ZCL_STRING_CONST_SIZE(NORDIC_DIY_INIT_BASIC_SW_VERSION));

    // identify cluster attributes data.
    dev_ctx.identify_attr.identify_time = ZB_ZCL_IDENTIFY_IDENTIFY_TIME_DEFAULT_VALUE;

    // power cluster attributes data.
    dev_ctx.power_attr.voltage = ZB_ZCL_POWER_CONFIG_BATTERY_VOLTAGE_INVALID;
    dev_ctx.power_attr.percent_remaining = ZB_ZCL_POWER_CONFIG_BATTERY_REMAINING_UNKNOWN;

    //  temperature cluster attributes data.
    dev_ctx.temp_measure_attrs.measure_value = ZB_ZCL_ATTR_TEMP_MEASUREMENT_VALUE_UNKNOWN;
    dev_ctx.temp_measure_attrs.min_measure_value = ATTR_TEMP_MIN;
    dev_ctx.temp_measure_attrs.max_measure_value = ATTR_TEMP_MAX;
    dev_ctx.temp_measure_attrs.tolerance = ATTR_TEMP_TOLERANCE;

    // humidity cluster attributes data.
    dev_ctx.humidity_measure_attrs.measure_value = ZB_ZCL_ATTR_REL_HUMIDITY_MEASUREMENT_VALUE_UNKNOWN;
    dev_ctx.humidity_measure_attrs.min_measure_value = ATTR_HUM_MIN;
    dev_ctx.humidity_measure_attrs.max_measure_value = ATTR_HUM_MAX;

    // pressure cluster attributes data.
    dev_ctx.pressure_measure_attrs.measure_value = ZB_ZCL_ATTR_PRESSURE_MEASUREMENT_VALUE_UNKNOWN;
    dev_ctx.pressure_measure_attrs.min_measure_value = ATTR_PRESSURE_MIN;
    dev_ctx.pressure_measure_attrs.max_measure_value = ATTR_PRESSURE_MAX;
    dev_ctx.pressure_measure_attrs.tolerance_value = ATTR_PRESSURE_TOLERANCE;

    // poll_control cluster attributes data.
    dev_ctx.poll_control_attrs.checkin_interval =
        ZB_ZCL_POLL_CONTROL_CHECKIN_INTERVAL_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.long_poll_interval =
        ZB_ZCL_POLL_CONTROL_LONG_POLL_INTERVAL_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.short_poll_interval =
        ZB_ZCL_POLL_CONTROL_SHORT_POLL_INTERVAL_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.fast_poll_timeout =
        ZB_ZCL_POLL_CONTROL_FAST_POLL_TIMEOUT_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.checkin_interval_min =
        ZB_ZCL_POLL_CONTROL_CHECKIN_MIN_INTERVAL_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.long_poll_interval_min =
        ZB_ZCL_POLL_CONTROL_LONG_POLL_MIN_INTERVAL_DEFAULT_VALUE;
    dev_ctx.poll_control_attrs.fast_poll_timeout_max =
        ZB_ZCL_POLL_CONTROL_FAST_POLL_MAX_TIMEOUT_DEFAULT_VALUE;
}
