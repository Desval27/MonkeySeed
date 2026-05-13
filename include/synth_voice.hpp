/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file synth_voice.hpp
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <basic_voice.hpp>

const char* WAVEFORM_LABELS[] = {
  "SIN", "TRI", "SAW", "RAMP", "SQUARE", "POLY TRI", "POLY SAW", "POLY SQUARE",
};

const char* FILTER_MODE_LABELS[] = {
  "LOW", "HIGH", "BAND", "NOTCH", "PEAK",
};

enum FilterMode
{
  FILTER_MODE_LOW,
  FILTER_MODE_HIGH,
  FILTER_MODE_BAND,
  FILTER_MODE_NOTCH,
  FILTER_MODE_PEAK,
};

struct SynthVoiceConfig : public BasicVoiceConfig
{
  daisy::MappedIntValue period;
  daisy::MappedStringListValue wave_form;
  daisy::MappedFloatValue noise_level;
  daisy::MappedFloatValue flt_freq;
  daisy::MappedStringListValue flt_mode;
  bool flt_key_track;
  daisy::MappedFloatValue flt_res;
  ADSREnvelope flt_envelope;
  daisy::MappedFloatValue amp_level;
  ADSREnvelope amp_envelope;

  bool operator==(const SynthVoiceConfig& other) const
  {
    return BasicVoiceConfig::operator==(other) && other.period == period &&
           other.wave_form == wave_form && other.noise_level == noise_level &&
           other.flt_freq == flt_freq && other.flt_mode == flt_mode &&
           other.flt_key_track == flt_key_track && other.flt_res == flt_res &&
           other.flt_envelope == flt_envelope && other.amp_level == amp_level &&
           other.amp_envelope == amp_envelope;
  }

  bool operator!=(const SynthVoiceConfig& other) const
  {
    return !(*this == other);
  }

  SynthVoiceConfig()
    : BasicVoiceConfig()
    , period(music::MIN_PERIOD, music::MAX_PERIOD, 0, 1, 1, "", false)
    , wave_form(WAVEFORM_LABELS,
                array_len(WAVEFORM_LABELS),
                daisysp::Oscillator::WAVE_TRI)
    , noise_level(0.0f,
                  1.0f,
                  0.0f,
                  daisy::MappedFloatValue::Mapping::lin,
                  "",
                  2,
                  false)
    , flt_freq(30.0f,
               16000.0f,
               1000.0f,
               daisy::MappedFloatValue::Mapping::log,
               "Hz",
               0,
               false)
    , flt_mode(FILTER_MODE_LABELS,
               array_len(FILTER_MODE_LABELS),
               FILTER_MODE_LOW)
    , flt_key_track(false)
    , flt_res(0.0f,
              1.0f,
              0.0f,
              daisy::MappedFloatValue::Mapping::lin,
              "",
              2,
              false)
    , flt_envelope(0.2f, 0.5f, 0.60f, 0.5f)
    , amp_level(-1.0f,
                1.0f,
                0.0f,
                daisy::MappedFloatValue::Mapping::lin,
                "",
                2,
                true)
    , amp_envelope(0.1f, 0.2f, 0.60f, 0.5f)
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

    const float vib_val = vib_.Process();
    const float freq =
      TBasicVoice::get_base_frequency() * (1.0f + (vib_val * vib_depth));
    osc_.SetFreq(freq);

    const float amp_sig =
      get_amp_signal(aEnv_.Process(TBasicVoice::get_gate()));
    osc_.SetAmp(amp_sig);

    // Mix and Filter
    const float osc_sig = osc_.Process();
    const float nse_sig = nse_.Process() * amp_sig;
    const float nse_ratio = TBasicVoice::config_.noise_level;
    const float mix_sig = (osc_sig * (1.0 - nse_ratio)) + (nse_sig * nse_ratio);

    const float filter_env_value_ = fEnv_.Process(TBasicVoice::get_gate());
    update_filter_cutoff(filter_env_value_ *
                         TBasicVoice::config_.flt_envelope.amount);
    flt_.Process(mix_sig);

    return TBasicVoice::balance_signal(get_filter_output());
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param nowMS
  void update(uint32_t nowMS) override
  {
    TBasicVoice::update(nowMS);

    osc_.SetWaveform(TBasicVoice::config_.wave_form);

    // Vibrato Configuration
    vib_.SetWaveform(daisysp::Oscillator::WAVE_SIN);
    vib_.SetFreq(5.0F); // Typical?

    update_filter_cutoff(filter_env_value_ *
                         TBasicVoice::config_.flt_envelope.amount);
    flt_.SetRes(TBasicVoice::config_.flt_res);

    fEnv_.SetAttackTime(TBasicVoice::config_.flt_envelope.attack);
    fEnv_.SetDecayTime(TBasicVoice::config_.flt_envelope.decay);
    fEnv_.SetSustainLevel(TBasicVoice::config_.flt_envelope.sustain);
    fEnv_.SetReleaseTime(TBasicVoice::config_.flt_envelope.release);

    aEnv_.SetAttackTime(TBasicVoice::config_.amp_envelope.attack);
    aEnv_.SetDecayTime(TBasicVoice::config_.amp_envelope.decay);
    aEnv_.SetSustainLevel(TBasicVoice::config_.amp_envelope.sustain);
    aEnv_.SetReleaseTime(TBasicVoice::config_.amp_envelope.release);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param pulse
  /// @param note
  /// @param gate
  /// @param trigger
  void handle_pulse(int pulse,
                    const music::NoteEvent& note,
                    bool gate,
                    bool trigger) override
  {
    TBasicVoice::handle_pulse(pulse, note, gate, trigger);
    set_vibrato_on(note.value > music::NoteValue::Eighth);
    update_filter_cutoff(filter_env_value_ *
                         TBasicVoice::config_.flt_envelope.amount);
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
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  float get_filter_cutoff(float envAmount = 0.0f) const
  {
    float cutoff = TBasicVoice::config_.flt_freq;
    if (TBasicVoice::config_.flt_key_track)
      cutoff *= TBasicVoice::get_base_frequency() / music::C4FREQ;

    cutoff += (16000.0f - cutoff) * envAmount;

    if (cutoff < 30.0f)
      return 30.0f;
    if (cutoff > 16000.0f)
      return 16000.0f;
    return cutoff;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  void update_filter_cutoff(float envAmount = 0.0f)
  {
    flt_.SetFreq(get_filter_cutoff(envAmount));
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param envValue
  /// @return
  float get_amp_signal(float envValue) const
  {
    float amp = TBasicVoice::config_.amp_level +
                (envValue * TBasicVoice::config_.amp_envelope.amount);

    if (amp < -1.0f)
      return -1.0f;
    if (amp > 1.0f)
      return 1.0f;
    return amp;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @return
  float get_filter_output()
  {
    const int mode = TBasicVoice::config_.flt_mode;
    switch (mode) {
      case FILTER_MODE_HIGH:
        return flt_.High();
      case FILTER_MODE_BAND:
        return flt_.Band();
      case FILTER_MODE_NOTCH:
        return flt_.Notch();
      case FILTER_MODE_PEAK:
        return flt_.Peak();
      default:
      case FILTER_MODE_LOW:
        return flt_.Low();
    }
  }

  bool vibrato_on_{ false };
  float filter_env_value_{ 0.0f };

  daisysp::Oscillator osc_;
  daisysp::Oscillator vib_;
  daisysp::WhiteNoise nse_;
  daisysp::Svf flt_;
  daisysp::Adsr fEnv_;
  daisysp::Adsr aEnv_;
};
