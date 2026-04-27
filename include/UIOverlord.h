/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file UIOverlord.h
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

#include <daisy_seed.h>
#include <daisysp.h>

#include <EncoderMonitor.h>

constexpr uint32_t UPDATE_RATE_MS          = 30;
constexpr uint32_t SCREEN_SAVER_TIMEOUT_MS = 5000;


template <typename DisplayDriver,
          size_t ENCODER_COUNT = 4,
          size_t BUTTON_COUNT  = 4,
          size_t POT_COUNT     = 4>
class UIOverlord
{
  public:
    struct EncoderConfig
    {
        Pin a;
        Pin b;
        Pin click;
    };

    struct ButtonConfig
    {
        Pin pin;
    };

    struct PotConfig
    {
        Pin pin;
    };

    void Init(float               sample_rate,
              daisy::AdcHandle*   adc,
              const EncoderConfig encoderConfig[],
              const ButtonConfig  buttonConfig[],
              const PotConfig     potConfig[])
    {
        //
        // Display Initialization.  Clear on startup.
        //
        typename daisy::OledDisplay<DisplayDriver>::Config disp_cfg;
        display_.Init(disp_cfg);
        display_.Fill(false);
        display_.Update();

        //
        // Init Encoders
        //
        if(ENCODER_COUNT > 0)
        {
            for(size_t i = 0; i < ENCODER_COUNT; i++)
            {
                encoders_[i].Init(encoderConfig[i].a,
                                  encoderConfig[i].b,
                                  encoderConfig[i].click);
            }
            encoderBackend_.encoders = encoders_.data();
            encoderMonitor_.Init(eventQueue_, encoderBackend_);
        }

        //
        // Init Buttons
        //
        if(BUTTON_COUNT > 0)
        {
            for(size_t i = 0; i < BUTTON_COUNT; i++)
            {
                buttons_[i].Init(buttonConfig[i].pin);
            }
            buttonBackend_.buttons = buttons_.data();
            buttonMonitor_.Init(eventQueue_, buttonBackend_);
        }

        //
        // Init Pots
        //
        if(POT_COUNT > 0)
        {
            AdcChannelConfig adcConfig[POT_COUNT];
            for(size_t i = 0; i < POT_COUNT; i++)
            {
                adcConfig[i].InitSingle(potConfig[i].pin);
            }
            adc->Init(adcConfig, POT_COUNT);
            adc->Start();
            potBackend_.adc = adc;
            potMonitor_.Init(eventQueue_, potBackend_);
        }


        UI::SpecialControlIds specialControls;
        if(ENCODER_COUNT > 0)
            specialControls.menuEncoderId = ENCODER_1;
        if(BUTTON_COUNT > 0)
            specialControls.okBttnId = BUTTON_1;

        UiCanvasDescriptor oledCanvas;
        oledCanvas.id_                = 0;
        oledCanvas.handle_            = &display_;
        oledCanvas.updateRateMs_      = UPDATE_RATE_MS;
        oledCanvas.screenSaverTimeOut = SCREEN_SAVER_TIMEOUT_MS;
        oledCanvas.clearFunction_     = ClearCanvas;
        oledCanvas.flushFunction_     = FlushCanvas;

        ui_.Init(eventQueue_, specialControls, {oledCanvas}, 0);
    }

    void Process()
    {
        // Debouce all controls
        for(size_t i = 0; i < ENCODER_COUNT; i++)
            encoders_[i].Debounce();
        for(size_t i = 0; i < BUTTON_COUNT; i++)
            buttons_[i].Debounce();

        // Process events
        if (ENCODER_COUNT > 0)
            encoderMonitor_.Process();
        if (BUTTON_COUNT > 0)
            buttonMonitor_.Process();
        if (POT_COUNT > 0)
            potMonitor_.Process();
            
        ui_.Process();
    }

    enum EncoderId
    {
        ENCODER_1 = 0,
        ENCODER_2 = 1,
        ENCODER_3 = 2,
        ENCODER_4 = 3,
    };

    enum ButtonId
    {
        BUTTON_1 = 0,
        BUTTON_2 = 1,
        BUTTON_3 = 2,
        BUTTON_4 = 3,
    };

    enum PotId
    {
        POT_1 = 0,
        POT_2 = 1,
        POT_3 = 2,
        POT_4 = 3,
    };

  protected:
    struct EncoderBackend
    {
        Encoder* encoders;
        int32_t  Increment(uint16_t encoderID)
        { return encoders[encoderID].Increment(); }
    };

    struct ButtonBackend
    {
        Switch* buttons;
        bool    IsButtonPressed(uint16_t buttonID)
        { return buttons[buttonID].Pressed(); }
    };

    struct PotBackend
    {
        AdcHandle* adc;
        // Used by PotMonitor
        float GetPotValue(uint16_t potID) { return adc->GetFloat(potID); }
    };

    static void ClearCanvas(const UiCanvasDescriptor& canvas)
    {
        auto* d
            = static_cast<daisy::OledDisplay<DisplayDriver>*>(canvas.handle_);
        d->Fill(false);
    }

    static void FlushCanvas(const UiCanvasDescriptor& canvas)
    {
        auto* d
            = static_cast<daisy::OledDisplay<DisplayDriver>*>(canvas.handle_);
        d->Update();
    }

  private:
    UI           ui_;
    UiEventQueue eventQueue_;

    daisy::OledDisplay<DisplayDriver> display_;

    // Control Backends
    EncoderBackend encoderBackend_;
    ButtonBackend  buttonBackend_;
    PotBackend     potBackend_;

    // Control Monitors
    EncoderMonitor<EncoderBackend, ENCODER_COUNT> encoderMonitor_;
    ButtonMonitor<ButtonBackend, BUTTON_COUNT>    buttonMonitor_;
    PotMonitor<PotBackend, POT_COUNT>             potMonitor_;

    // Control Instances
    std::array<Encoder, ENCODER_COUNT> encoders_;
    std::array<Switch, BUTTON_COUNT>   buttons_;
};
