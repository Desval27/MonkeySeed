/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file BasePage.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <daisy_seed.h>
#include <daisysp.h>

template <bool CONSOLIDATE_EVENTS> class BasePage : public daisy::UiPage {
public:
  BasePage() {}

  void Draw(const daisy::UiCanvasDescriptor &canvas) override {
    if (!ValidateCanvas(canvas))
        return;

    daisy::OneBitGraphicsDisplay &display =
        *(daisy::OneBitGraphicsDisplay *)(canvas.handle_);

    InternalDraw(display, daisy::System::GetNow());
  }

  void SetOneBitGraphicsDisplayToDrawTo(uint16_t canvasId) {
    canvasIdToDrawTo_ = canvasId;
  }

  // I really don't like having separate methods based on the control type since
  // context can change based on the page involved.  Instead, we're going to
  // optionally  funnel them all through the more generic handlers.
  bool OnCancelButton(uint8_t numberOfPresses, bool isRetriggering) override {
    if (CONSOLIDATE_EVENTS)
      return OnButton(GetParentUI()->GetSpecialControlIds().cancelBttnId,
                      numberOfPresses, isRetriggering);
    return daisy::UiPage::OnCancelButton(numberOfPresses, isRetriggering);
  }

  bool OnOkayButton(uint8_t numberOfPresses, bool isRetriggering) override {
    if (CONSOLIDATE_EVENTS)
      return OnButton(GetParentUI()->GetSpecialControlIds().okBttnId,
                      numberOfPresses, isRetriggering);
    return daisy::UiPage::OnOkayButton(numberOfPresses, isRetriggering);
  }

  bool OnArrowButton(daisy::ArrowButtonType arrowType, uint8_t numberOfPresses,
                     bool isRetriggering) override {
    if (CONSOLIDATE_EVENTS)
      return OnButton(GetButtonIdForArrowType(arrowType), numberOfPresses,
                      isRetriggering);
    return daisy::UiPage::OnArrowButton(arrowType, numberOfPresses,
                                        isRetriggering);
  }

  bool OnFunctionButton(uint8_t numberOfPresses, bool isRetriggering) override {
    if (CONSOLIDATE_EVENTS)
      return OnButton(GetParentUI()->GetSpecialControlIds().funcBttnId,
                      numberOfPresses, isRetriggering);
    return daisy::UiPage::OnFunctionButton(numberOfPresses, isRetriggering);
  }

  bool OnMenuEncoderTurned(int16_t turns,
                           uint16_t stepsPerRevolution) override {
    if (CONSOLIDATE_EVENTS)
      return OnEncoderTurned(
          GetParentUI()->GetSpecialControlIds().menuEncoderId, turns,
          stepsPerRevolution);
    return daisy::UiPage::OnMenuEncoderTurned(turns, stepsPerRevolution);
  }

  bool OnValueEncoderTurned(int16_t turns,
                            uint16_t stepsPerRevolution) override {
    if (CONSOLIDATE_EVENTS)
      return OnEncoderTurned(
          GetParentUI()->GetSpecialControlIds().valueEncoderId, turns,
          stepsPerRevolution);
    return daisy::UiPage::OnValueEncoderTurned(turns, stepsPerRevolution);
  }

  bool OnMenuEncoderActivityChanged(bool isCurrentlyActive) override {
    if (CONSOLIDATE_EVENTS)
      return OnEncoderActivityChanged(
          GetParentUI()->GetSpecialControlIds().menuEncoderId,
          isCurrentlyActive);
    return daisy::UiPage::OnMenuEncoderActivityChanged(isCurrentlyActive);
  }

  bool OnValueEncoderActivityChanged(bool isCurrentlyActive) override {
    if (CONSOLIDATE_EVENTS)
      return OnEncoderActivityChanged(
          GetParentUI()->GetSpecialControlIds().valueEncoderId,
          isCurrentlyActive);
    return daisy::UiPage::OnValueEncoderActivityChanged(isCurrentlyActive);
  }

  bool OnValuePotMoved(float newPosition) override {
    if (CONSOLIDATE_EVENTS)
      return OnPotMoved(GetParentUI()->GetSpecialControlIds().valuePotId,
                        newPosition);
    return daisy::UiPage::OnValuePotMoved(newPosition);
  }

  bool OnValuePotActivityChanged(bool isCurrentlyActive) override {
    if (CONSOLIDATE_EVENTS)
      return OnPotActivityChanged(
          GetParentUI()->GetSpecialControlIds().valuePotId, isCurrentlyActive);
    return daisy::UiPage::OnValuePotActivityChanged(isCurrentlyActive);
  }

protected:
  uint16_t GetButtonIdForArrowType(daisy::ArrowButtonType arrowType) {
    switch (arrowType) {
    case daisy::ArrowButtonType::left:
      return GetParentUI()->GetSpecialControlIds().leftBttnId;
    case daisy::ArrowButtonType::right:
      return GetParentUI()->GetSpecialControlIds().rightBttnId;
    case daisy::ArrowButtonType::up:
      return GetParentUI()->GetSpecialControlIds().upBttnId;
    case daisy::ArrowButtonType::down:
      return GetParentUI()->GetSpecialControlIds().downBttnId;
    default:
      return daisy::UiEventQueue::invalidButtonId;
    }
  }

  bool ValidateCanvas(const daisy::UiCanvasDescriptor &canvas) const  
  {
    // Find out if this canvas is one we should draw to.
    if(canvasIdToDrawTo_ == daisy::UI::invalidCanvasId)
    {
        // We're configured to use the UIs default canvas.
        auto* ui = GetParentUI();
        if(!ui)
            // No parent UI?! How are we supposed to find out what cannvas to draw to?
            return false;

        if(ui->GetPrimaryOneBitGraphicsDisplayId() != canvas.id_)
            // This is not the default canvas! Don't draw here.
            return false;
    }
    else if(canvasIdToDrawTo_ != canvas.id_)
        // we're configured to draw to a specific canvas, but not this one.
        return false;
    return true;
  }

  virtual void InternalDraw(daisy::OneBitGraphicsDisplay &display, uint32_t nowMS) = 0;

private:
  uint16_t canvasIdToDrawTo_ = daisy::UI::invalidCanvasId;

};
