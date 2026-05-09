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

#include <array>
#include <utility>

#include <Music/EventSetManager.h>
#include <Music/Gnome.h>

template<std::size_t MAX_DEGREES = Music::DEF_MAX_DEGREES,
         std::size_t SCALE_DEGREES = Music::DEF_SCALE_DEGREES>
///////////////////////////////////////////////////////////////////////////////
/// @brief
class BasicApp
{
  using MySetup = Music::Setup<MAX_DEGREES, SCALE_DEGREES>;

private:
  const uint32_t COMPONENT_REFRESH_MS = 100;

public:
  MySetup setup;
  Music::Gnome gnome;

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Initializes app infrastructure including all voices based on the
  /// given sample rate
  /// @param sample_rate
  virtual void Init(float sample_rate) {}

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Processes the audio stream for all voices mixed into separate left
  /// & right values.
  /// @return left & right floating point values.
  virtual std::tuple<float, float> Process(bool trigger = false)
  {
    return { 0.0F, 0.0F };
  }

  /////////////////////////////////////////////////////////////////////////////
  /// @brief Updates the status of all components including voices based on
  /// possible configuration or setup changes.
  /// @param nowMS
  void Update(uint32_t nowMS)
  {
    uint32_t delta = nowMS - lastRefreshMS_;
    if (delta >= COMPONENT_REFRESH_MS) {
      lastRefreshMS_ = nowMS - (delta % COMPONENT_REFRESH_MS);
      InternalUpdate(nowMS);
    }
  }

protected:
  /////////////////////////////////////////////////////////////////////////////
  /// @brief
  BasicApp()
    : setup(4,
            Music::NoteValue::Quarter,
            Music::TWELVE_TONE,
            Music::OCTAVE_DOUBLE)
    , gnome(setup.timeSignature, setup.bars)
    , lastRefreshMS_(0UL)
  {
  }

  virtual void InternalUpdate(uint32_t nowMS)
  {
    // Do Nothing Stub
  }

private:
  uint32_t lastRefreshMS_{ 0 };
};
