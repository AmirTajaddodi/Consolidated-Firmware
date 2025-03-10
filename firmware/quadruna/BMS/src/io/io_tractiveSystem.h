#pragma once

#ifdef TARGET_EMBEDDED
#include "hw_adc.h"

typedef struct
{
    const AdcChannel *ts_vsense_channel_P;
    const AdcChannel *ts_vsense_channel_N;
    const AdcChannel *ts_isense_high_res_channel;
    const AdcChannel *ts_isense_low_res_channel;
} TractiveSystemConfig;
#else
#include "app_utils.h"
EMPTY_STRUCT(TractiveSystemConfig);
#endif

/**
 * Initialize the tractive system driver.
 * @param ts_config Config struct.
 */
void io_tractiveSystem_init(const TractiveSystemConfig *ts_config);

/**
 * Convert the given ADC voltage to tractive system voltage
 * @return The tractive system voltage in V
 */
float io_tractiveSystem_getVoltage(void);

/**
 * Convert the given ADC voltage to high resolution main current
 * @note This correspond to output 1 of DHAB S/124 (+/- 75A):
 * @return Main current from high resolution sensor.
 */
float io_tractiveSystem_getCurrentHighResolution(void);

/**
 * Convert the given ADC voltage to low resolution main current
 * @note This corresponds to output 2 of DHAB S/124 (+/- 500A):
 * @return Main current from low resolution sensor.
 */
float io_tractiveSystem_getCurrentLowResolution(void);
