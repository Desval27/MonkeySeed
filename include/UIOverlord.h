/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file UIOverlord.h
 * @brief 
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <daisy_seed.h>
#include <daisysp.h>

template <size_t ENCODER_COUNT = 4>
class UIOverlord
{
    enum EncoderId
    {
        ENCODER_1,
        ENCODER_2,
        ENCODER_3,
        ENCODER_4,
    };

  private:
    Encoder encoders[ENCODER_COUNT];
};