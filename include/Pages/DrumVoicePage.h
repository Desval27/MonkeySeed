/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file DrumVoicePage.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <DrumVoice.h>
#include <Pages/FullScreenVerticalMenu.h>

///////////////////////////////////////////////////////////////////////////////
/// @brief
class DrumVoicePage : public FullScreenVerticalMenu<>
{
public:
  /////////////////////////////////////////////////////////////////////////////
  /// @brief
  DrumVoicePage() = default;

  /////////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param config
  void Bind(DrumVoiceConfig& config)
  {
    menuConfig_[0].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[0].text = s_VOLUME;
    menuConfig_[0].asMappedValueItem.valueToModify = &config.volume;

    menuConfig_[1].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[1].text = s_BALANCE;
    menuConfig_[1].asMappedValueItem.valueToModify = &config.balance;

    menuConfig_[2].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[2].text = s_FREQ;
    menuConfig_[2].asMappedValueItem.valueToModify = &config.freq;

    menuConfig_[3].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[3].text = s_TONE;
    menuConfig_[3].asMappedValueItem.valueToModify = &config.tone;

    FullScreenVerticalMenu<>::Init(
      menuConfig_.data(),
      menuConfig_.size(),
      daisy::AbstractMenu::Orientation::leftRightSelectUpDownModify,
      true);
  }

  // /////////////////////////////////////////////////////////////////////////////
  // /// @brief
  // /// @param voice
  // template<typename T>
  // void Init(DrumVoice<T>& voice)
  // {
  //   Bind(voice.config_);
  // }

private:
  std::array<daisy::AbstractMenu::ItemConfig, 4> menuConfig_;

  static inline const char* s_VOLUME = "VOLUME";
  static inline const char* s_BALANCE = "BALANCE";
  static inline const char* s_FREQ = "FREQ";
  static inline const char* s_TONE = "TONE";
};
