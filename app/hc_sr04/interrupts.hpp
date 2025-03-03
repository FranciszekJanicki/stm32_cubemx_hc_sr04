#ifndef INTERRUPTS_HPP
#define INTERRUPTS_HPP

#include "common.hpp"
#include "gpio.hpp"

enum Event : std::uint32_t {
    ECHO_TRIG = 1U << 0U,
    SAMPLING_TIMER = 1U << 1U,
};

namespace {

    volatile std::uint32_t event_bits = 0UL;

};

inline void reset_event_bits(std::uint32_t const event_mask) noexcept
{
    event_bits &= ~event_mask;
}

inline void set_event_bits(std::uint32_t const event_mask) noexcept
{
    event_bits |= event_mask;
}

[[nodiscard]] inline std::uint32_t get_event_bits(std::uint32_t const event_mask,
                                                  bool const clear_on_exit = true) noexcept
{
    auto bits = event_bits;
    if (clear_on_exit) {
        reset_event_bits(event_mask);
    }
    return bits;
}

#endif // INTERRUPTS_HPP