#pragma once
/* This file has been generated by VisualGDB.
   DO NOT EDIT MANUALLY. THE FILE WILL BE OVERWRITTEN.
   Use VisualGDB Project Properties window to edit these settings instead. */

#pragma once
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_adc.h"
#include "r_adc_api.h"

FSP_HEADER
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc0;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc0_ctrl;
extern const adc_cfg_t g_adc0_cfg;
extern const adc_channel_cfg_t g_adc0_channel_cfg;

#ifndef NULL
void NULL(adc_callback_args_t * p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  0
#endif

void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER