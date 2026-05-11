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
#include <daisy_helpers.h>
#include <daisysp.h>

struct ADEnvelope
{
  daisy::MappedFloatValue attack;
  daisy::MappedFloatValue decay;
  daisy::MappedFloatValue amount;

  ADEnvelope(float defAttack, float defDecay, float defAmount = 1.0f)
    : attack(0.0f,
             10.0f,
             defAttack,
             daisy::MappedFloatValue::Mapping::lin,
             "s",
             2,
             false)
    , decay(0.0f,
            10.0f,
            defDecay,
            daisy::MappedFloatValue::Mapping::lin,
            "s",
            2,
            false)
    , amount(0.0f,
             1.0f,
             defAmount,
             daisy::MappedFloatValue::Mapping::lin,
             "",
             2,
             false)

  {
  }

  bool operator==(const ADEnvelope& other) const
  {
    return other.attack == attack && other.decay == decay &&
           other.amount == amount;
  }

  bool operator!=(const ADEnvelope& other) const { return !(*this == other); }

  void Draw(const daisy::UiCanvasDescriptor& canvas) {}
};

struct ADSREnvelope
{
  daisy::MappedFloatValue attack;
  daisy::MappedFloatValue decay;
  daisy::MappedFloatValue sustain;
  daisy::MappedFloatValue release;
  daisy::MappedFloatValue amount;

  ADSREnvelope(float defAttack,
               float defDecay,
               float defSustain,
               float defRelease,
               float defAmount = 1.0f)
    : attack(0.0f,
             10.0f,
             defAttack,
             daisy::MappedFloatValue::Mapping::lin,
             "s",
             2,
             false)
    , decay(0.0f,
            10.0f,
            defDecay,
            daisy::MappedFloatValue::Mapping::lin,
            "s",
            2,
            false)
    , sustain(0.0f,
              1.0f,
              defSustain,
              daisy::MappedFloatValue::Mapping::lin,
              "",
              2,
              false)
    , release(0.0f,
              10.0f,
              defRelease,
              daisy::MappedFloatValue::Mapping::lin,
              "s",
              2,
              false)
    , amount(0.0f,
             1.0f,
             defAmount,
             daisy::MappedFloatValue::Mapping::lin,
             "",
             2,
             false)
  {
  }

  bool operator==(const ADSREnvelope& other) const
  {
    return other.attack == attack && other.decay == decay &&
           other.sustain == sustain && other.release == release &&
           other.amount == amount;
  }

  bool operator!=(const ADSREnvelope& other) const { return !(*this == other); }

  void Draw(const daisy::UiCanvasDescriptor& canvas) {}
};
