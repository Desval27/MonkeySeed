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

#include <Envelopes.h>
#include <monkey.hpp>
#include <music/music.hpp>
#include <music/music_config.hpp>

///////////////////////////////////////////////////////////////////////////
/// @brief
struct BasicVoiceConfig
{
  BasicVoiceConfig()
    : volume(0.0f,
             1.0f,
             1.0f,
             daisy::MappedFloatValue::Mapping::lin,
             "",
             2,
             false)
    , balance(-1.0f,
              1.0f,
              0.0f,
              daisy::MappedFloatValue::Mapping::lin,
              "",
              4,
              true)
  {
  }

  daisy::MappedFloatValue volume;
  daisy::MappedFloatValue balance;
};

///////////////////////////////////////////////////////////////////////////////
/// @brief
// template <std::size_t OSCILLATOR_COUNT = 1>
template<std::size_t MAX_DEGREES = music::DEF_MAX_DEGREES,
         std::size_t SCALE_DEGREES = music::DEF_SCALE_DEGREES,
         typename VOICE_CONFIG = BasicVoiceConfig>
class BasicVoice
{
  using MySetup = music::Setup<MAX_DEGREES, SCALE_DEGREES>;
  using MyPitchEngine = music::PitchEngine<MAX_DEGREES, SCALE_DEGREES>;

public:
  VOICE_CONFIG config_;

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  BasicVoice()
    : base_frequency_(music::BASE_HZ)
    , gate_(false)
    , trigger_(false)
  {
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param sample_rate
  virtual void init(const MySetup& setup, int periodOffset, float sample_rate)
  {
    const float voiceRootHertz =
      music::C4FREQ * setup.temperament.period_multiplier(periodOffset);

    pitch_engine_.set_temperament(&setup.temperament);
    pitch_engine_.set_scale_map(&setup.scaleMap);
    pitch_engine_.set_root_hz(voiceRootHertz); // C4
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  virtual std::tuple<float, float> process(bool trigger = false)
  {
    return { 0.0F, 0.0F };
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param nowMS
  virtual void update(uint32_t nowMS) {}

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  float get_base_frequency() const { return base_frequency_; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param value
  void set_base_frequency(float value) { base_frequency_ = value; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  bool get_gate() const { return gate_; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param value
  void set_gate(bool value) { gate_ = value; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  bool get_trigger() const { return trigger_; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param value
  void set_trigger(bool value) { trigger_ = value; }

protected:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param sig
  /// @return
  std::tuple<float, float> balance_signal(float sig)
  {
    const float rCoeff = (config_.balance.Get() + 1.0F) / 2.0F;
    const float lCoeff = 1.0F - rCoeff;
    return { sig * lCoeff, sig * rCoeff };
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param n
  /// @param p
  /// @param fc
  /// @return
  float get_frequency_for_note(music::Note n,
                               music::Period p,
                               float fc = 0.0f) const
  {
    return pitch_engine_.Frequency(music::TemperedPitch(n, p, fc));
  }

private:
  MyPitchEngine pitch_engine_;
  float base_frequency_;
  bool gate_;
  bool trigger_;
};
