/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file SynthVoice.h
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

const char* WAVEFORM_LABELS[] = {
  "SIN", "TRI", "SAW", "RAMP", "SQUARE", "POLY TRI", "POLY SAW", "POLY SQUARE",
};

struct SynthVoiceConfig : public BasicVoiceConfig
{
  SynthVoiceConfig()
    : BasicVoiceConfig()
    , period(Music::MIN_PERIOD, Music::MAX_PERIOD, 0, 1, 1, "", false)
    , waveForm(WAVEFORM_LABELS,
               ArrayLen(WAVEFORM_LABELS),
               daisysp::Oscillator::WAVE_TRI)
    , noiseLevel(0.0f,
                 1.0f,
                 0.0f,
                 daisy::MappedFloatValue::Mapping::lin,
                 "",
                 2,
                 false)
    , fltFreq(30.0f,
              16000.0f,
              1000.0f,
              daisy::MappedFloatValue::Mapping::log,
              "Hz",
              0,
              false)
    , fltRes(0.0f,
             1.0f,
             0.0f,
             daisy::MappedFloatValue::Mapping::lin,
             "",
             2,
             false)
    , fltEnvelope(0.2f, 0.5f)
    , ampLevel(0.0f,
               1.0f,
               1.0f,
               daisy::MappedFloatValue::Mapping::lin,
               "",
               2,
               true)
    , ampEnvelope(0.1f, 0.2f, 0.60f, 0.5f)
  {
  }

  daisy::MappedIntValue period;
  daisy::MappedStringListValue waveForm;
  daisy::MappedFloatValue noiseLevel;
  daisy::MappedFloatValue fltFreq;
  daisy::MappedFloatValue fltRes;
  ADEnvelope fltEnvelope;
  daisy::MappedFloatValue ampLevel;
  ADSREnvelope ampEnvelope;
};

class SynthVoice : public BasicVoice<SynthVoiceConfig>
{
public:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param sample_rate
  void Init(float sample_rate) override
  {
    BasicVoice::Init(sample_rate);

    osc_.Init(sample_rate);
    nse_.Init();
    flt_.Init(sample_rate);
    fEnv_.Init(sample_rate);
    aEnv_.Init(sample_rate);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  std::tuple<float, float> Process(bool trigger = false) override
  {
    // Don't get anything from base class.

    // float sigF = fEnv_.Process(GetGate());
    // float sigA = aEnv_.Process(GetGate());

    osc_.SetAmp(config_.ampLevel);

    float oscSig = osc_.Process();
    float nseSig = nse_.Process();
    float nseRatio = config_.noiseLevel;

    flt_.Process((oscSig * (1.0 - nseRatio)) + (nseSig * nseRatio));
    float sig = flt_.Low();

    return balance_signal(sig);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param nowMS
  void Update(uint32_t nowMS) override
  {
    BasicVoice::Update(nowMS);

    osc_.SetWaveform(config_.waveForm);

    flt_.SetFreq(config_.fltFreq);
    flt_.SetRes(config_.fltRes);

    fEnv_.SetTime(daisysp::ADENV_SEG_ATTACK, config_.fltEnvelope.attack);
    fEnv_.SetTime(daisysp::ADENV_SEG_DECAY, config_.fltEnvelope.decay);
    // fEnv_.SetAttackTime(config_.fltEnvelope.attack);
    // fEnv_.SetDecayTime(config_.fltEnvelope.decay);
    // fEnv_.SetSustainLevel(config_.fltEnvelope.sustain);
    // fEnv_.SetReleaseTime(config_.fltEnvelope.release);

    aEnv_.SetAttackTime(config_.ampEnvelope.attack);
    aEnv_.SetDecayTime(config_.ampEnvelope.decay);
    aEnv_.SetSustainLevel(config_.ampEnvelope.sustain);
    aEnv_.SetReleaseTime(config_.ampEnvelope.release);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param value
  void SetFreq(float value)
  {
    freq_ = value;
    osc_.SetFreq(freq_);
  }

private:
  daisysp::Oscillator osc_;
  daisysp::WhiteNoise nse_;
  daisysp::Svf flt_;
  daisysp::AdEnv fEnv_;
  daisysp::Adsr aEnv_;

  float freq_;
};