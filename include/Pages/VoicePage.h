#pragma once

#include <BasicVoice.h>
#include <Pages/FullScreenVerticalMenu.h>

class VoicePage : public FullScreenVerticalMenu<>
{
  public:
    VoicePage() {}

    void Init(BasicVoice<ADSREnvelope> *voice)
    {
      voice_ = voice;
      menuConfig_[0].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[0].text = s_SHAPE;
      menuConfig_[0].asMappedValueItem.valueToModify = &voice->waveForm;

      menuConfig_[1].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[1].text = s_VOLUME;
      menuConfig_[1].asMappedValueItem.valueToModify = &voice->volume;

      menuConfig_[2].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[2].text = s_ATTACK;
      menuConfig_[2].asMappedValueItem.valueToModify = &voice->envelope.attack;

      menuConfig_[3].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[3].text = s_DECAY;
      menuConfig_[3].asMappedValueItem.valueToModify = &voice->envelope.decay;

      menuConfig_[4].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[4].text = s_SUSTAIN;
      menuConfig_[4].asMappedValueItem.valueToModify = &voice->envelope.sustain;

      menuConfig_[5].type = daisy::AbstractMenu::ItemType::valueItem;
      menuConfig_[5].text = s_RELEASE;
      menuConfig_[5].asMappedValueItem.valueToModify = &voice->envelope.release;

      FullScreenVerticalMenu<>::Init(
          menuConfig_.data(),
          menuConfig_.size(),
          daisy::AbstractMenu::Orientation::leftRightSelectUpDownModify,
          true);
    }

  private:
    BasicVoice<ADSREnvelope> *voice_;
    std::array<daisy::AbstractMenu::ItemConfig, 6> menuConfig_;

    static inline const char* s_SHAPE   = "SHAPE";
    static inline const char* s_VOLUME  = "VOLUME";
    static inline const char* s_ATTACK  = "ATTACK";
    static inline const char* s_DECAY   = "DECAY";
    static inline const char* s_SUSTAIN = "SUSTAIN";
    static inline const char* s_RELEASE = "RELEASE";
};