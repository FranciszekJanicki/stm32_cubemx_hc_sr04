#ifndef HC_SR04_HC_SR04_H
#define HC_SR04_HC_SR04_H

#include "hc_sr04_config.h"

typedef struct {
    hc_sr04_config_t config;
    hc_sr04_interface_t interface;

    uint32_t distance_mm;
    uint32_t echo_count;
} hc_sr04_t;

hc_sr04_err_t
hc_sr04_initialize(hc_sr04_t* hc_sr04, hc_sr04_config_t const* config, hc_sr04_interface_t const* interface);
hc_sr04_err_t hc_sr04_deinitialize(hc_sr04_t* hc_sr04);

hc_sr04_err_t hc_sr04_transmit_trig_signal(hc_sr04_t const* hc_sr04);
hc_sr04_err_t hc_sr04_receive_echo_signal(hc_sr04_t* hc_sr04);
uint32_t hc_sr04_get_distance_mm(hc_sr04_t* hc_sr04);

#endif // HC_SR04_HC_SR04_H