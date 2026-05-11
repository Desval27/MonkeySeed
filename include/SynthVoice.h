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
  daisy::MappedIntValue period;
  daisy::MappedStringListValue waveForm;
  daisy::MappedFloatValue noiseLevel;
  daisy::MappedFloatValue fltFreq;
  daisy::MappedFloatValue fltRes;
  ADEnvelope fltEnvelope;
  daisy::MappedFloatValue ampLevel;
  ADSREnvelope ampEnvelope;

  bool operator==(const SynthVoiceConfig& other) const
  {
    return BasicVoiceConfig::operator==(other) && other.period == period &&
           other.waveForm == waveForm && other.noiseLevel == noiseLevel &&
           other.fltFreq == fltFreq && other.fltRes == fltRes &&
           other.fltEnvelope == fltEnvelope && other.ampLevel == ampLevel &&
           other.ampEnvelope == ampEnvelope;
  }

  bool operator!=(const SynthVoiceConfig& other) const
  {
    return !(*this == other);
  }

  SynthVoiceConfig()
    : BasicVoiceConfig()
    , period(music::MIN_PERIOD, music::MAX_PERIOD, 0, 1, 1, "", false)
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
    , ampLevel(-1.0f,
               1.0f,
               0.0f,
               daisy::MappedFloatValue::Mapping::lin,
               "",
               2,
               true)
    , ampEnvelope(0.1f, 0.2f, 0.60f, 0.5f)
  {
  }
};

template<std::size_t MAX_DEGREES = music::DEF_MAX_DEGREES,
         std::size_t SCALE_DEGREES = music::DEF_SCALE_DEGREES,
         typename VOICE_CONFIG = SynthVoiceConfig>
class SynthVoice : public BasicVoice<MAX_DEGREES, SCALE_DEGREES, VOICE_CONFIG>
{
  using TSetup = music::Setup<MAX_DEGREES, SCALE_DEGREES>;
  using TBasicVoice = BasicVoice<MAX_DEGREES, SCALE_DEGREES, VOICE_CONFIG>;

public:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param sample_rate
  void init(const TSetup& setup, int periodOffset, float sample_rate) override
  {
    TBasicVoice::init(setup, periodOffset, sample_rate);

    osc_.Init(sample_rate);
    vib_.Init(sample_rate);
    nse_.Init();
    flt_.Init(sample_rate);
    fEnv_.Init(sample_rate);
    aEnv_.Init(sample_rate);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  std::tuple<float, float> process(bool trigger = false) override
  {
    // get_frequency & Vibrato
    float vib_depth = 0.0F;
    if (get_vibrato_on())
      vib_depth = 0.01f; // ~50 cents pitch multipler

    float vib_val = vib_.Process();
    float freq =
      TBasicVoice::get_base_frequency() * (1.0f + (vib_val * vib_depth));
    osc_.SetFreq(freq);

    // Volume Processing
    float sigA = aEnv_.Process(TBasicVoice::get_gate());
    osc_.SetAmp(sigA); // Take into consideration amp level

    // Mix and Filter
    float oscSig = osc_.Process();
    float nseSig = nse_.Process() * sigA;
    float nseRatio = TBasicVoice::config_.noiseLevel;
    flt_.Process((oscSig * (1.0 - nseRatio)) + (nseSig * nseRatio));
    float sig = flt_.Low();

    return TBasicVoice::balance_signal(sig);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param nowMS
  void update(uint32_t nowMS) override
  {
    TBasicVoice::update(nowMS);

    osc_.SetWaveform(TBasicVoice::config_.waveForm);

    // Vibrato Configuration
    vib_.SetWaveform(daisysp::Oscillator::WAVE_SIN);
    vib_.SetFreq(5.0F); // Typical?

    flt_.SetFreq(TBasicVoice::config_.fltFreq);
    flt_.SetRes(TBasicVoice::config_.fltRes);

    fEnv_.SetTime(daisysp::ADENV_SEG_ATTACK,
                  TBasicVoice::config_.fltEnvelope.attack);
    fEnv_.SetTime(daisysp::ADENV_SEG_DECAY,
                  TBasicVoice::config_.fltEnvelope.decay);
    // fEnv_.SetAttackTime(config_.fltEnvelope.attack);
    // fEnv_.SetDecayTime(config_.fltEnvelope.decay);
    // fEnv_.SetSustainLevel(config_.fltEnvelope.sustain);
    // fEnv_.SetReleaseTime(config_.fltEnvelope.release);

    aEnv_.SetAttackTime(TBasicVoice::config_.ampEnvelope.attack);
    aEnv_.SetDecayTime(TBasicVoice::config_.ampEnvelope.decay);
    aEnv_.SetSustainLevel(TBasicVoice::config_.ampEnvelope.sustain);
    aEnv_.SetReleaseTime(TBasicVoice::config_.ampEnvelope.release);
  }

  void handle_pulse(int pulse,
                    const music::NoteEvent& note,
                    bool gate,
                    bool trigger)
  {
    set_vibrato_on(note.value > music::NoteValue::Eighth);

    TBasicVoice::set_base_frequency(
      TBasicVoice::get_frequency_for_note(note.note, note.period));
    TBasicVoice::set_gate(gate);
    TBasicVoice::set_trigger(trigger);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  bool get_vibrato_on() const { return vibrato_on_; }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param value
  void set_vibrato_on(bool value) { vibrato_on_ = value; }

private:
  bool vibrato_on_{ false };

  daisysp::Oscillator osc_;
  daisysp::Oscillator vib_;
  daisysp::WhiteNoise nse_;
  daisysp::Svf flt_;
  daisysp::AdEnv fEnv_;
  daisysp::Adsr aEnv_;
};
