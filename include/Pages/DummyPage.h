#pragma once

#include <Pages/BasePage.h>

class DummyPage : public BasePage<true>
{
protected:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param display
  /// @param nowMS
  void InternalDraw(daisy::OneBitGraphicsDisplay& display, uint32_t nowMS) {}
};