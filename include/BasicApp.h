/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file BasicApp.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <BasicVoice.h>

template <std::size_t VOICE_COUNT>
struct BasicApp
{
    BasicApp()
    {

    }

    std::array<BasicVoice<ADSREnvelope>, VOICE_COUNT> voices{};
};