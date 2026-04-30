/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file TurnOnlyEncoder.h
 * @brief Quadrature encoder reader without an integrated push switch.
 */
#pragma once

#include <daisy_seed.h>
#include <per/gpio.h>

namespace daisy {
class TurnOnlyEncoder {
public:
  TurnOnlyEncoder()
      : last_update_(0), updated_(false), hw_a_(), hw_b_(), a_(0xff), b_(0xff),
        inc_(0) {}

  void Init(Pin a, Pin b) {
    last_update_ = System::GetNow();
    updated_ = false;

    hw_a_.pin = a;
    hw_a_.mode = DSY_GPIO_MODE_INPUT;
    hw_a_.pull = DSY_GPIO_PULLUP;
    hw_b_.pin = b;
    hw_b_.mode = DSY_GPIO_MODE_INPUT;
    hw_b_.pull = DSY_GPIO_PULLUP;
    dsy_gpio_init(&hw_a_);
    dsy_gpio_init(&hw_b_);

    inc_ = 0;
    a_ = b_ = 0xff;
  }

  void Debounce() {
    uint32_t now = System::GetNow();
    updated_ = false;

    if (now - last_update_ >= 1) {
      last_update_ = now;
      updated_ = true;

      a_ = (a_ << 1) | dsy_gpio_read(&hw_a_);
      b_ = (b_ << 1) | dsy_gpio_read(&hw_b_);

      inc_ = 0;
      if ((a_ & 0x03) == 0x02 && (b_ & 0x03) == 0x00) {
        inc_ = 1;
      } else if ((b_ & 0x03) == 0x02 && (a_ & 0x03) == 0x00) {
        inc_ = -1;
      }
    }
  }

  int32_t Increment() const { return updated_ ? inc_ : 0; }

private:
  uint32_t last_update_;
  bool updated_;
  dsy_gpio hw_a_;
  dsy_gpio hw_b_;
  uint8_t a_;
  uint8_t b_;
  int32_t inc_;
};
} // namespace daisy
