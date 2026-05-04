#pragma once

#include <BasicVoice.h>
#include <Pages/FullScreenVerticalMenu.h>

class VoicePage : public FullScreenVerticalMenu<>
{
  public:
    VoicePage() {}

    void Init(BasicVoice* voice)
    {
        voice_ = voice;

        menuConfig_[0].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[0].text = s_PERIOD;
        menuConfig_[0].asMappedValueItem.valueToModify = &voice->period;

        menuConfig_[1].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[1].text = s_SHAPE;
        menuConfig_[1].asMappedValueItem.valueToModify = &voice->waveForm;

        menuConfig_[2].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[2].text = s_VOLUME;
        menuConfig_[2].asMappedValueItem.valueToModify = &voice->volume;

        menuConfig_[3].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[3].text = s_FLT_FREQ;
        menuConfig_[3].asMappedValueItem.valueToModify = &voice->fltFreq;

        menuConfig_[4].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[4].text = s_FLT_RES;
        menuConfig_[4].asMappedValueItem.valueToModify = &voice->fltRes;

        menuConfig_[5].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[5].text = s_FLT_ATTACK;
        menuConfig_[5].asMappedValueItem.valueToModify
            = &voice->fltEnvelope.attack;

        menuConfig_[6].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[6].text = s_FLT_DECAY;
        menuConfig_[6].asMappedValueItem.valueToModify
            = &voice->fltEnvelope.decay;

        menuConfig_[7].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[7].text = s_AMP_LEVEL;
        menuConfig_[7].asMappedValueItem.valueToModify
            = &voice->ampLevel;

        menuConfig_[8].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[8].text = s_AMP_ATTACK;
        menuConfig_[8].asMappedValueItem.valueToModify
            = &voice->ampEnvelope.attack;

        menuConfig_[9].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[9].text = s_AMP_DECAY;
        menuConfig_[9].asMappedValueItem.valueToModify
            = &voice->ampEnvelope.decay;

        menuConfig_[10].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[10].text = s_AMP_SUSTAIN;
        menuConfig_[10].asMappedValueItem.valueToModify
            = &voice->ampEnvelope.sustain;

        menuConfig_[11].type = daisy::AbstractMenu::ItemType::valueItem;
        menuConfig_[11].text = s_AMP_RELEASE;
        menuConfig_[11].asMappedValueItem.valueToModify
            = &voice->ampEnvelope.release;

        FullScreenVerticalMenu<>::Init(
            menuConfig_.data(),
            menuConfig_.size(),
            daisy::AbstractMenu::Orientation::leftRightSelectUpDownModify,
            true);
    }

  private:
    BasicVoice* voice_;

    std::array<daisy::AbstractMenu::ItemConfig, 12> menuConfig_;

    static inline const char* s_PERIOD      = "PERIOD";
    static inline const char* s_SHAPE       = "SHAPE";
    static inline const char* s_VOLUME      = "VOLUME";
    static inline const char* s_FLT_FREQ    = "FLT FREQ";
    static inline const char* s_FLT_RES     = "FLT RES";
    static inline const char* s_FLT_ATTACK  = "FLT ATTACK";
    static inline const char* s_FLT_DECAY   = "FLT DECAY";
    static inline const char* s_AMP_LEVEL   = "AMP LEVEL";
    static inline const char* s_AMP_ATTACK  = "AMP ATTACK";
    static inline const char* s_AMP_DECAY   = "AMP DECAY";
    static inline const char* s_AMP_SUSTAIN = "AMP SUSTAIN";
    static inline const char* s_AMP_RELEASE = "AMP RELEASE";
};