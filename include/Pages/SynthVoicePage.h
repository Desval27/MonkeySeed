#pragma once

#include <Pages/FullScreenVerticalMenu.h>
#include <synth_voice.hpp>

///////////////////////////////////////////////////////////////////////////////
/// @brief
class SynthVoicePage : public FullScreenVerticalMenu<>
{
public:
  /////////////////////////////////////////////////////////////////////////////
  /// @brief
  SynthVoicePage() = default;

  /////////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param config
  void Bind(SynthVoiceConfig& config)
  {
    menuConfig_[0].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[0].text = s_VOLUME;
    menuConfig_[0].asMappedValueItem.valueToModify = &config.volume;

    menuConfig_[1].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[1].text = s_BALANCE;
    menuConfig_[1].asMappedValueItem.valueToModify = &config.balance;

    menuConfig_[2].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[2].text = s_PERIOD;
    menuConfig_[2].asMappedValueItem.valueToModify = &config.period;

    menuConfig_[3].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[3].text = s_SHAPE;
    menuConfig_[3].asMappedValueItem.valueToModify = &config.wave_form;

    menuConfig_[4].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[4].text = s_NOISE;
    menuConfig_[4].asMappedValueItem.valueToModify = &config.noise_level;

    menuConfig_[5].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[5].text = s_FLT_FREQ;
    menuConfig_[5].asMappedValueItem.valueToModify = &config.flt_freq;

    menuConfig_[6].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[6].text = s_FLT_RES;
    menuConfig_[6].asMappedValueItem.valueToModify = &config.flt_res;

    menuConfig_[7].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[7].text = s_FLT_MODE;
    menuConfig_[7].asMappedValueItem.valueToModify = &config.flt_mode;

    menuConfig_[8].type = daisy::AbstractMenu::ItemType::checkboxItem;
    menuConfig_[8].text = s_FLT_KEY_TRACK;
    menuConfig_[8].asCheckboxItem.valueToModify = &config.flt_key_track;

    menuConfig_[9].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[9].text = s_FLT_ATTACK;
    menuConfig_[9].asMappedValueItem.valueToModify = &config.flt_envelope.attack;

    menuConfig_[10].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[10].text = s_FLT_DECAY;
    menuConfig_[10].asMappedValueItem.valueToModify = &config.flt_envelope.decay;

    menuConfig_[11].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[11].text = s_FLT_SUSTAIN;
    menuConfig_[11].asMappedValueItem.valueToModify =
      &config.flt_envelope.sustain;

    menuConfig_[12].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[12].text = s_FLT_RELEASE;
    menuConfig_[12].asMappedValueItem.valueToModify =
      &config.flt_envelope.release;

    menuConfig_[13].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[13].text = s_FLT_AMOUNT;
    menuConfig_[13].asMappedValueItem.valueToModify =
      &config.flt_envelope.amount;

    menuConfig_[14].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[14].text = s_AMP_LEVEL;
    menuConfig_[14].asMappedValueItem.valueToModify = &config.amp_level;

    menuConfig_[15].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[15].text = s_AMP_ATTACK;
    menuConfig_[15].asMappedValueItem.valueToModify =
      &config.amp_envelope.attack;

    menuConfig_[16].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[16].text = s_AMP_DECAY;
    menuConfig_[16].asMappedValueItem.valueToModify = &config.amp_envelope.decay;

    menuConfig_[17].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[17].text = s_AMP_SUSTAIN;
    menuConfig_[17].asMappedValueItem.valueToModify =
      &config.amp_envelope.sustain;

    menuConfig_[18].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[18].text = s_AMP_RELEASE;
    menuConfig_[18].asMappedValueItem.valueToModify =
      &config.amp_envelope.release;

    menuConfig_[19].type = daisy::AbstractMenu::ItemType::valueItem;
    menuConfig_[19].text = s_AMP_AMOUNT;
    menuConfig_[19].asMappedValueItem.valueToModify =
      &config.amp_envelope.amount;

    FullScreenVerticalMenu<>::Init(
      menuConfig_.data(),
      menuConfig_.size(),
      daisy::AbstractMenu::Orientation::leftRightSelectUpDownModify,
      true);
  }

private:
  std::array<daisy::AbstractMenu::ItemConfig, 20> menuConfig_;

  static inline const char* s_PERIOD = "PERIOD";
  static inline const char* s_SHAPE = "SHAPE";
  static inline const char* s_VOLUME = "VOLUME";
  static inline const char* s_BALANCE = "BALANCE";
  static inline const char* s_NOISE = "NOISE";
  static inline const char* s_FLT_FREQ = "FLT FREQ";
  static inline const char* s_FLT_RES = "FLT RES";
  static inline const char* s_FLT_MODE = "FLT MODE";
  static inline const char* s_FLT_KEY_TRACK = "FLT KEYTRK";
  static inline const char* s_FLT_ATTACK = "FLT ATTACK";
  static inline const char* s_FLT_DECAY = "FLT DECAY";
  static inline const char* s_FLT_SUSTAIN = "FLT SUSTAIN";
  static inline const char* s_FLT_RELEASE = "FLT RELEASE";
  static inline const char* s_FLT_AMOUNT = "FLT AMT";
  static inline const char* s_AMP_LEVEL = "AMP LEVEL";
  static inline const char* s_AMP_ATTACK = "AMP ATTACK";
  static inline const char* s_AMP_DECAY = "AMP DECAY";
  static inline const char* s_AMP_SUSTAIN = "AMP SUSTAIN";
  static inline const char* s_AMP_RELEASE = "AMP RELEASE";
  static inline const char* s_AMP_AMOUNT = "AMP AMT";
};
