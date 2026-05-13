/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file FullScreenVerticalMenu.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <Pages/BasePage.h>
#include <daisy.h>

using namespace daisy;

enum class MenuFontSize : uint8_t
{
  FONT_SMALL,
  FONT_NORMAL,
  FONT_BIG,
  FONT_TOOBIG,
};

///////////////////////////////////////////////////////////////////////////////
/// @brief
template<MenuFontSize FONT_SIZE = MenuFontSize::FONT_NORMAL>
class FullScreenVerticalMenu : public AbstractMenu
{
public:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param items
  /// @param numItems
  /// @param orientation
  /// @param allowEntering
  void Init(const AbstractMenu::ItemConfig* items,
            uint16_t numItems,
            AbstractMenu::Orientation orientation =
              AbstractMenu::Orientation::leftRightSelectUpDownModify,
            bool allowEntering = true)
  {
    AbstractMenu::Init(items, numItems, orientation, allowEntering);
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param canvasId
  void SetOneBitGraphicsDisplayToDrawTo(uint16_t canvasId)
  {
    canvas_id_2draw2_ = canvasId;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param canvas
  void Draw(const daisy::UiCanvasDescriptor& canvas) override
  {
    if (!ValidateCanvas(canvas))
      return;

    daisy::OneBitGraphicsDisplay& display =
      *(daisy::OneBitGraphicsDisplay*)(canvas.handle_);

    InternalDraw(display);
  }

protected:
  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param display
  virtual void InternalDraw(daisy::OneBitGraphicsDisplay& display)
  {
    // no items or out of bounds??!
    if ((selectedItemIdx_ < 0) || (selectedItemIdx_ >= numItems_))
      return;

    // Experimenting for now
    FontDef fd = GetFontDef(FONT_SIZE);

    Rectangle base_rect(
      display.GetBounds().WithHeight(fd.FontHeight + 2).WithTrimmedLeft(1));

    int16_t char_height = base_rect.GetHeight();

    const int16_t line = 0; // I don't remember what this was for...
    const int16_t max_items =
      std::min(static_cast<int16_t>(numItems_),
               static_cast<int16_t>(display.Height() / char_height - line));

    int16_t start_index;
    if (center_scrolling_)
      start_index = std::max(
        0, std::min(selectedItemIdx_ - (max_items / 2), numItems_ - max_items));
    else
      start_index = selectedItemIdx_ >= max_items
                      ? std::max(0, selectedItemIdx_ - max_items + 1)
                      : 0;

    int16_t y_offset = 0;
    for (int16_t item_index = start_index;
         item_index < (start_index + max_items);
         item_index++, y_offset += char_height) {
      Rectangle item_rect = base_rect.Translated(0, y_offset);

      FixedCapStr<20> item_text;

      bool is_selected = (item_index == selectedItemIdx_);
      if (is_selected && isEditing_)
        item_text.Append(">");

      // This is just a WIP.  For now
      // based on the item append some
      // sort of indicator text if possible
      item_text.Append(items_[item_index].text);

      switch (items_[item_index].type) {
        // Mostly just being sort of cheap for now.
        case ItemType::checkboxItem:
          if (*items_[item_index].asCheckboxItem.valueToModify)
            item_text.Append("=YES");
          else
            item_text.Append("=NO");
          break;
        case ItemType::openUiPageItem:
          item_text.Append("...");
          break;
        case ItemType::closeMenuItem:
          item_text.Append(" X");
          break;
        case ItemType::valueItem:
          item_text.Append(' ');
          (*items_[item_index].asMappedValueItem.valueToModify)
            .AppentToString(item_text);
          break;
        default:
        case ItemType::customItem:
        case ItemType::callbackFunctionItem:
          break;
      }
      // Draw a inverse rectangle around the selected item.
      if (is_selected)
        display.DrawRect(item_rect, true, true);

      display.WriteStringAligned(
        item_text.Cstr(), fd, item_rect, Alignment::centered, !is_selected);
    }
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param canvas
  /// @return
  bool ValidateCanvas(const daisy::UiCanvasDescriptor& canvas) const
  {
    // Find out if this canvas is one we should draw to.
    if (canvas_id_2draw2_ == daisy::UI::invalidCanvasId) {
      // We're configured to use the UIs default canvas.
      auto* ui = GetParentUI();
      if (!ui)
        // No parent UI?! How are we supposed to find out what cannvas to draw
        // to?
        return false;

      if (ui->GetPrimaryOneBitGraphicsDisplayId() != canvas.id_)
        // This is not the default canvas! Don't draw here.
        return false;
    } else if (canvas_id_2draw2_ != canvas.id_)
      // we're configured to draw to a specific canvas, but not this one.
      return false;
    return true;
  }

  ///////////////////////////////////////////////////////////////////////////
  /// @brief
  /// @param size
  /// @return
  const FontDef& GetFontDef(MenuFontSize size)
  {
    switch (size) {
      case MenuFontSize::FONT_TOOBIG:
        return Font_16x26;
      case MenuFontSize::FONT_BIG:
        return Font_11x18;
      case MenuFontSize::FONT_NORMAL:
        return Font_7x10;
      default:
      case MenuFontSize::FONT_SMALL:
        return Font_6x8;
    }
  }

private:
  uint16_t canvas_id_2draw2_ = daisy::UI::invalidCanvasId;
  bool center_scrolling_ = true;
};