/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file Envelopes.h
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

struct ADEnvelope
{
    ADEnvelope()
    : attack(0.0f,
             10.0f,
             0.1f,
             daisy::MappedFloatValue::Mapping::lin,
             "s",
             2,
             false),
      decay(0.0f,
            10.0f,
            0.1f,
            daisy::MappedFloatValue::Mapping::lin,
            "s",
            2,
            false)
    {
    }


    daisy::MappedFloatValue attack;
    daisy::MappedFloatValue decay;

    void Draw(const daisy::UiCanvasDescriptor &canvas) {}
};

struct ADSREnvelope
{
    ADSREnvelope()
    : attack(0.0f,
             10.0f,
             0.1f,
             daisy::MappedFloatValue::Mapping::lin,
             "s",
             2,
             false),
      decay(0.0f,
            10.0f,
            0.1f,
            daisy::MappedFloatValue::Mapping::lin,
            "s",
            2,
            false),
      sustain(0.0f,
              100.0f,
              50.0f,
              daisy::MappedFloatValue::Mapping::lin,
              "%",
              0,
              false),
      release(0.0f,
              10.0f,
              0.1f,
              daisy::MappedFloatValue::Mapping::lin,
              "s",
              2,
              false)
    {
    }


    daisy::MappedFloatValue attack;
    daisy::MappedFloatValue decay;
    daisy::MappedFloatValue sustain;
    daisy::MappedFloatValue release;

    void Draw(const daisy::UiCanvasDescriptor &canvas) {}
};
