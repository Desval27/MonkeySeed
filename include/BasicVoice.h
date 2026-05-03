/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file BasicVoice.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <daisy.h>
#include <daisysp.h>

#include <Monkey.h>

#include <Envelopes.h>

const char* WAVEFORM_LABELS[] = {
    "SIN",
    "TRI",
    "SAW",
    "RAMP",
    "SQUARE",
    "POLY TRI",
    "POLY SAW",
    "POLY SQUARE",
};

template <typename Envelope, std::size_t OSCILLATOR_COUNT = 1>
struct BasicVoice
{
    BasicVoice()
    : waveForm(WAVEFORM_LABELS,
               ArrayLen(WAVEFORM_LABELS),
               daisysp::Oscillator::WAVE_TRI),
      volume(0.0f, 100.0f, 100.0f, daisy::MappedFloatValue::Mapping::lin, "%", 0, false),
      envelope()
    {
    }

    MappedStringListValue waveForm;
    MappedFloatValue      volume;
    Envelope              envelope;

    std::array<daisysp::Oscillator, OSCILLATOR_COUNT> oscillators;
};
