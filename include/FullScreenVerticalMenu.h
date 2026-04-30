#pragma once

#include <BasePage.h>
#include <daisy.h>

using namespace daisy;

class FullScreenVerticalMenu : public AbstractMenu {
public:
  void Init(const AbstractMenu::ItemConfig *items, uint16_t numItems,
            AbstractMenu::Orientation orientation =
                AbstractMenu::Orientation::leftRightSelectUpDownModify,
            bool allowEntering = true) {
    AbstractMenu::Init(items, numItems, orientation, allowEntering);
  }

  void SetOneBitGraphicsDisplayToDrawTo(uint16_t canvasId) {
    canvasIdToDrawTo_ = canvasId;
  }

  void Draw(const daisy::UiCanvasDescriptor &canvas) override {
    if (!ValidateCanvas(canvas))
      return;

    daisy::OneBitGraphicsDisplay &display =
        *(daisy::OneBitGraphicsDisplay *)(canvas.handle_);

    InternalDraw(display);
  }

protected:
  virtual void InternalDraw(daisy::OneBitGraphicsDisplay &display) {
    // no items or out of bounds??!
    if ((selectedItemIdx_ < 0) || (selectedItemIdx_ >= numItems_))
      return;

    // Experimenting for now
    FontDef fd = Font_6x8;
    Rectangle baseRect(
        display.GetBounds().WithHeight(fd.FontHeight + 2).WithTrimmedLeft(1));

    int16_t charH = baseRect.GetHeight();

    const int16_t line = 0; // I don't remember what this was for...
    const int16_t maxItems = std::min(static_cast<int16_t>(numItems_), static_cast<int16_t>(display.Height()/charH - line));

    int16_t startIdx;
    if (centerScrolling_)
      startIdx = std::max(
          0, std::min(selectedItemIdx_ - (maxItems / 2), numItems_ - maxItems));
    else
      startIdx = selectedItemIdx_ >= maxItems
                       ? std::max(0, selectedItemIdx_ - maxItems + 1)
                       : 0;

    int16_t yOfs = 0;
    for (int16_t itemIdx = startIdx; itemIdx < (startIdx + maxItems); itemIdx++, yOfs += charH)
    {
      Rectangle itemRect = baseRect.Translated(0, yOfs);
      bool isSelected = (itemIdx == selectedItemIdx_);

      FixedCapStr<20> itemText(items_[itemIdx].text);

      switch (items_[itemIdx].type) {
      case ItemType::closeMenuItem:
        itemText.Append(" X");
        break;
      case ItemType::callbackFunctionItem:
        break;
      case ItemType::checkboxItem:
        if (*items_[itemIdx].asCheckboxItem.valueToModify)
          itemText.Append(" Y");
        else
          itemText.Append(" N");
        break;
      case ItemType::valueItem:
        break;
      case ItemType::openUiPageItem:
        itemText.Append("...");
        break;
      case ItemType::customItem:
        break;
      }
      if (isSelected)
        display.DrawRect(itemRect, true, true);
      display.WriteStringAligned(itemText.Cstr(), fd, itemRect,
                                 Alignment::centered, !isSelected);
    }
  }

  bool ValidateCanvas(const daisy::UiCanvasDescriptor &canvas) const {
    // Find out if this canvas is one we should draw to.
    if (canvasIdToDrawTo_ == daisy::UI::invalidCanvasId) {
      // We're configured to use the UIs default canvas.
      auto *ui = GetParentUI();
      if (!ui)
        // No parent UI?! How are we supposed to find out what cannvas to draw
        // to?
        return false;

      if (ui->GetPrimaryOneBitGraphicsDisplayId() != canvas.id_)
        // This is not the default canvas! Don't draw here.
        return false;
    } else if (canvasIdToDrawTo_ != canvas.id_)
      // we're configured to draw to a specific canvas, but not this one.
      return false;
    return true;
  }

private:
  uint16_t canvasIdToDrawTo_ = daisy::UI::invalidCanvasId;
  bool centerScrolling_ = true;
};