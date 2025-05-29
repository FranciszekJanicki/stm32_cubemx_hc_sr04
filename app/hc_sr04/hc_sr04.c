#include "hc_sr04.h"
#include <assert.h>
#include <string.h>

static hc_sr04_err_t hc_sr04_gpio_init(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.gpio_init) {
        return hc_sr04->interface.gpio_init(hc_sr04->interface.gpio_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_gpio_deinit(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.gpio_deinit) {
        return hc_sr04->interface.gpio_deinit(hc_sr04->interface.gpio_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_gpio_write(hc_sr04_t const* hc_sr04, uint32_t pin, bool state)
{
    assert(hc_sr04);

    if (hc_sr04->interface.gpio_write) {
        return hc_sr04->interface.gpio_write(hc_sr04->interface.gpio_user, pin, state);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_gpio_read(hc_sr04_t const* hc_sr04, uint32_t pin, bool* state)
{
    assert(hc_sr04);

    if (hc_sr04->interface.gpio_read) {
        return hc_sr04->interface.gpio_read(hc_sr04->interface.gpio_user, pin, state);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_init(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_init) {
        return hc_sr04->interface.timer_init(hc_sr04->interface.timer_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_deinit(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_deinit) {
        return hc_sr04->interface.timer_deinit(hc_sr04->interface.timer_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_start(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_start) {
        return hc_sr04->interface.timer_start(hc_sr04->interface.timer_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_stop(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_stop) {
        return hc_sr04->interface.timer_stop(hc_sr04->interface.timer_user);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_set_count(hc_sr04_t const* hc_sr04, uint32_t count)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_set_count) {
        return hc_sr04->interface.timer_set_count(hc_sr04->interface.timer_user, count);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_get_count(hc_sr04_t const* hc_sr04, uint32_t* count)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_get_count) {
        return hc_sr04->interface.timer_get_count(hc_sr04->interface.timer_user, count);
    }

    return HC_SR04_ERR_NULL;
}

static hc_sr04_err_t hc_sr04_timer_delay_us(hc_sr04_t const* hc_sr04, uint32_t delay_us)
{
    assert(hc_sr04);

    if (hc_sr04->interface.timer_delay_us) {
        return hc_sr04->interface.timer_delay_us(hc_sr04->interface.timer_user, delay_us);
    }

    return HC_SR04_ERR_NULL;
}

static uint32_t tof_ms_to_distance_mm(uint32_t tof_ms)
{
    return (uint32_t)((1000.0F * 340.0F) * ((float)tof_ms / 1000.0F) / 2.0F);
}

static uint32_t count_to_time_ms(uint32_t count)
{
    return count;
}

hc_sr04_err_t
hc_sr04_initialize(hc_sr04_t* hc_sr04, hc_sr04_config_t const* config, hc_sr04_interface_t const* interface)
{
    assert(hc_sr04 && config && interface);

    memset(hc_sr04, 0, sizeof(*hc_sr04));
    memcpy(&hc_sr04->config, config, sizeof(*config));
    memcpy(&hc_sr04->interface, interface, sizeof(*interface));

    hc_sr04_err_t err = hc_sr04_gpio_init(hc_sr04);
    err |= hc_sr04_timer_init(hc_sr04);

    return err;
}

hc_sr04_err_t hc_sr04_deinitialize(hc_sr04_t* hc_sr04)
{
    assert(hc_sr04);

    hc_sr04_err_t err = hc_sr04_gpio_deinit(hc_sr04);
    err |= hc_sr04_timer_deinit(hc_sr04);

    memset(hc_sr04, 0, sizeof(*hc_sr04));

    return err;
}

hc_sr04_err_t hc_sr04_transmit_trig_signal(hc_sr04_t const* hc_sr04)
{
    assert(hc_sr04);

    hc_sr04_err_t err = hc_sr04_gpio_write(hc_sr04, hc_sr04->config.pin_trig, false);
    err |= hc_sr04_gpio_write(hc_sr04, hc_sr04->config.pin_trig, true);
    err |= hc_sr04_timer_delay_us(hc_sr04, 10UL);
    err |= hc_sr04_gpio_write(hc_sr04, hc_sr04->config.pin_trig, false);

    err |= hc_sr04_timer_set_count(hc_sr04, 0UL);
    err |= hc_sr04_timer_start(hc_sr04);

    return err;
}

hc_sr04_err_t hc_sr04_receive_echo_signal(hc_sr04_t* hc_sr04)
{
    assert(hc_sr04);

    bool echo_state;
    hc_sr04_err_t err = hc_sr04_gpio_read(hc_sr04, hc_sr04->config.pin_echo, &echo_state);

    if (echo_state) {
        ++hc_sr04->echo_count;
    }

    if (hc_sr04->echo_count > 8U) {
        uint32_t tof_count;

        err |= hc_sr04_timer_stop(hc_sr04);
        err |= hc_sr04_timer_get_count(hc_sr04, &tof_count);
        err |= hc_sr04_timer_set_count(hc_sr04, 0UL);

        uint32_t tof_ms = count_to_time_ms(tof_count);
        hc_sr04->distance_mm = tof_ms_to_distance_mm(tof_ms);
        hc_sr04->echo_count = 0U;
    }

    return err;
}

uint32_t hc_sr04_get_distance_mm(hc_sr04_t* hc_sr04)
{
    assert(hc_sr04);

    return hc_sr04->distance_mm;
}
