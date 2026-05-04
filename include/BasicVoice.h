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
#include <Music/Music.h>
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

//template <std::size_t OSCILLATOR_COUNT = 1>
class BasicVoice
{
  public:
    BasicVoice()
    : period(Music::MIN_PERIOD, Music::MAX_PERIOD, 4, 1, 1, "", false),
      waveForm(WAVEFORM_LABELS,
               ArrayLen(WAVEFORM_LABELS),
               daisysp::Oscillator::WAVE_TRI),
      volume(0.0f,
             100.0f,
             100.0f,
             daisy::MappedFloatValue::Mapping::lin,
             "%",
             0,
             false),
      fltFreq(0.0f,
              16000.0f,
              1000.0f,
              daisy::MappedFloatValue::Mapping::lin,
              "Hz",
              0,
              false),
      fltRes(0.0f,
             1.0f,
             0.0f,
             daisy::MappedFloatValue::Mapping::lin,
             "",
             2,
             false),
      fltEnvelope(0.1f, 0.5f),
      ampLevel(-1.0f,
               1.0f,
               0.0f,
               daisy::MappedFloatValue::Mapping::lin,
               "",
               2,
               true),
      ampEnvelope(0.1f, 0.2f, 0.60f, 0.5f)
    {
    }

    void Init(float sample_rate)
    {
        osc_.Init(sample_rate);
        ampAdsr_.Init(sample_rate);
    }

    float Process()
    {
        // For now...
        return osc_.Process();
    }

    void Update(uint32_t nowMS)
    {
        ampAdsr_.SetAttackTime(ampEnvelope.attack);
        ampAdsr_.SetDecayTime(ampEnvelope.decay);
        ampAdsr_.SetSustainLevel(ampEnvelope.sustain);
        ampAdsr_.SetReleaseTime(ampEnvelope.release);
    }

    MappedIntValue        period;
    MappedStringListValue waveForm;
    MappedFloatValue      volume;
    MappedFloatValue      fltFreq;
    MappedFloatValue      fltRes;
    ADEnvelope            fltEnvelope;
    MappedFloatValue      ampLevel;
    ADSREnvelope          ampEnvelope;

  private:
    daisysp::Oscillator osc_;
    daisysp::Adsr       ampAdsr_;
};
