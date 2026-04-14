import * as m from 'zigbee-herdsman-converters/lib/modernExtend';

export default {
    zigbeeModel: ['nordic DIY PM'],
    model: 'nordic DIY',
    vendor: 'Nordic',
    description: 'Nordic DIY sensor',
    extend: [
        m.temperature({
            reporting: {
                min: 60,
                max: 300,
                change: 20,
            },
        }),
        m.humidity({
            reporting: {
                min: 180,
                max: 300,
                change: 100,
            },
        }),
        m.pressure({
            reporting: {
                min: 120,
                max: 300,
                change: 5,
            },
            unit: 'hPa',
        }),
        m.battery({
            voltage: true,
            voltageReporting: false,
            voltageReportingConfig: {
                min: 600,
                max: 3600,
                change: 1,
            },
            percentageReportingConfig: {
                min: 600,
                max: 3600,
                change: 1,
            },
        }),
    ],
};