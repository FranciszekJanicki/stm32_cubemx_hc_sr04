#ifndef HC_SR04_HPP
#define HC_SR04_HPP

#include "gpio.hpp"
#include "interrupts.hpp"
#include "utility.hpp"

namespace HC_SR04 {

    struct HC_SR04 {
        using TIMHandle = Utility::TIMHandle;
        using GPIO = Utility::GPIO;

        static std::uint32_t tof_ms_to_distance_mm(std::uint32_t const tof_ms) noexcept;
        static void delay_us(TIMHandle const timer, std::uint32_t const time_us) noexcept;

        void transmit_trig_signal() noexcept;
        void receive_echo_signal() noexcept;
        std::uint32_t get_distance_mm() const noexcept;

        TIMHandle tof_timer{nullptr};
        GPIO trig_pin{};
        GPIO echo_pin{};

        std::uint32_t (*count_to_time_ms)(std::uint32_t const);

        std::uint32_t distance_mm{};
        std::uint32_t echo_count{};
    };

}; // namespace HC_SR04

#endif // HC_SR04_HPP