/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file DrumVoice.h
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
// #include <analogbassdrum.h>
#include <daisy.h>
#include <daisysp.h>

struct DrumVoiceConfig : public BasicVoiceConfig
{
  DrumVoiceConfig()
    : BasicVoiceConfig()
  {
  }

  daisy::MappedFloatValue freq{
    1.0F, 12000.0F, 56.0F, daisy::MappedFloatValue::Mapping::log, "Hz", 0, false
  };

  daisy::MappedFloatValue tone{
    0.0F, 1.0F, 0.5F, daisy::MappedFloatValue::Mapping::lin, "", 2, false
  };
};

template<typename T>
class DrumVoice : public BasicVoice<DrumVoiceConfig>
{
public:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param sample_rate
  void Init(float sample_rate) override
  {
    BasicVoice::Init(sample_rate);
    t_.Init(sample_rate);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  std::tuple<float, float> Process(bool trigger = false) override
  {
    // Don't get anything from base class.
    float sig = (t_.Process(trigger) * config_.volume);
    return balance_signal(sig);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param nowMS
  void Update(uint32_t nowMS) override
  {
    BasicVoice::Update(nowMS);
    // t_.SetFreq(config_.freq);
    // t_.SetTone(config_.tone);
  }

public:
  T t_;
};