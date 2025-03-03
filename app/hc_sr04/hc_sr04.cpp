#include "hc_sr04.hpp"
#include "common.hpp"
#include "utility.hpp"

namespace HC_SR04 {

    std::uint32_t HC_SR04::tof_ms_to_distance_mm(std::uint32_t const tof_ms) noexcept
    {
        return (1000U * 340U) * (tof_ms / 1000U) / 2U;
    }

    void HC_SR04::delay_us(TIMHandle const timer, std::uint32_t const time_us) noexcept
    {
        auto time = 0UL;
        auto const count_period_ms = 0UL;
        auto const start_count = __HAL_TIM_GetCounter(timer);
        auto const end_count =
            start_count + Utility::time_ms_to_count(time_us / 1000UL, timer->Init.Prescaler, 80000000UL);
        while (__HAL_TIM_GetCounter(timer) < end_count) {
            ;
            ;
        }
    }

    void HC_SR04::transmit_trig_signal() noexcept
    {
        gpio_reset_pin(this->trig_pin);
        gpio_set_pin(this->trig_pin);
        delay_us(this->tof_timer, 10UL);
        gpio_reset_pin(this->trig_pin);

        __HAL_TIM_SetCounter(this->tof_timer, 0UL);
        HAL_TIM_Base_Start(this->tof_timer);
    }

    void HC_SR04::receive_echo_signal() noexcept
    {
        if (gpio_read_pin(this->echo_pin)) {
            ++this->echo_count;
        }

        if (this->echo_count > 8U) {
            HAL_TIM_Base_Stop(this->tof_timer);
            __HAL_TIM_SetCounter(this->tof_timer, 0UL);
            this->distance_mm = tof_ms_to_distance_mm(this->count_to_time_ms(__HAL_TIM_GetCounter(this->tof_timer)));
            this->echo_count = 0U;
        }
    }

    std::uint32_t HC_SR04::get_distance_mm() const noexcept
    {
        return this->distance_mm;
    }

}; // namespace HC_SR04