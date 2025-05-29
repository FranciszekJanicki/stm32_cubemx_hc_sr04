#include "main.h"
#include "gpio.h"
#include "hc_sr04.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_hal_tim.h"
#include "tim.h"
#include <stdint.h>
#include <stm32l4xx_hal_gpio.h>

enum {
    EVENT_ECHO_TRIG = 1 << 0,
    EVENT_SAMPLING_TIMER = 1 << 1,
};

static uint32_t volatile event_bits = {};

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == GPIO_PIN_0) {
        event_bits |= EVENT_ECHO_TRIG;
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef* htim)
{
    if (htim->Instance == TIM2) {
        event_bits |= EVENT_SAMPLING_TIMER;
    }
}

static hc_sr04_err_t hc_sr04_gpio_write(void* user, uint32_t pin, bool state)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    HAL_GPIO_WritePin((GPIO_TypeDef*)user, (uint16_t)pin, (GPIO_PinState)state);

    return HC_SR04_ERR_OK;
}

static hc_sr04_err_t hc_sr04_gpio_read(void* user, uint32_t pin, bool* state)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    *state = (bool)HAL_GPIO_ReadPin((GPIO_TypeDef*)user, (uint16_t)pin);

    return HC_SR04_ERR_OK;
}

static hc_sr04_err_t hc_sr04_timer_start(void* user)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    return HAL_TIM_Base_Start((TIM_HandleTypeDef*)user) ? HC_SR04_ERR_OK : HC_SR04_ERR_FAIL;
}

static hc_sr04_err_t hc_sr04_timer_stop(void* user)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    return HAL_TIM_Base_Stop((TIM_HandleTypeDef*)user) ? HC_SR04_ERR_OK : HC_SR04_ERR_FAIL;
}

static hc_sr04_err_t hc_sr04_timer_set_count(void* user, uint32_t count)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    ((TIM_HandleTypeDef*)user)->Instance->CNT = count;

    return HC_SR04_ERR_OK;
}

static hc_sr04_err_t hc_sr04_timer_get_count(void* user, uint32_t* count)
{
    if (!user) {
        return HC_SR04_ERR_NULL;
    }

    *count = ((TIM_HandleTypeDef*)user)->Instance->CNT;

    return HC_SR04_ERR_OK;
}

int main(void)
{
    HAL_Init();
    SystemClock_Config();

    MX_GPIO_Init();
    MX_TIM1_Init();
    MX_TIM3_Init();

    hc_sr04_t hc_sr04 = {};

    hc_sr04_config_t config = {.pin_echo = 1 << 5, .pin_trig = 1 << 4};

    hc_sr04_interface_t interface = {.gpio_user = GPIOB,
                                     .gpio_write = &hc_sr04_gpio_write,
                                     .gpio_read = &hc_sr04_gpio_read,
                                     .timer_user = &htim1,
                                     .timer_get_count = &hc_sr04_timer_get_count,
                                     .timer_set_count = &hc_sr04_timer_set_count,
                                     .timer_start = &hc_sr04_timer_start,
                                     .timer_stop = &hc_sr04_timer_stop};

    hc_sr04_initialize(&hc_sr04, &config, &interface);

    HAL_TIM_Base_Start_IT(&htim3);

    while (1) {
        if (event_bits & EVENT_ECHO_TRIG) {
            event_bits &= ~EVENT_ECHO_TRIG;
        }

        if (event_bits & EVENT_SAMPLING_TIMER) {
            event_bits &= ~EVENT_SAMPLING_TIMER;

            HAL_TIM_Base_Start_IT(&htim3);
        }
    }
}
