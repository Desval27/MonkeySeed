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

/// @brief
template <MenuFontSize FONT_SIZE = MenuFontSize::FONT_NORMAL>
class FullScreenVerticalMenu : public AbstractMenu
{
  public:
    /// @brief
    /// @param items
    /// @param numItems
    /// @param orientation
    /// @param allowEntering
    void Init(const AbstractMenu::ItemConfig *items,
              uint16_t                        numItems,
              AbstractMenu::Orientation       orientation
              = AbstractMenu::Orientation::leftRightSelectUpDownModify,
              bool allowEntering = true)
    { AbstractMenu::Init(items, numItems, orientation, allowEntering); }

    /// @brief
    /// @param canvasId
    void SetOneBitGraphicsDisplayToDrawTo(uint16_t canvasId)
    { canvasIdToDrawTo_ = canvasId; }

    /// @brief
    /// @param canvas
    void Draw(const daisy::UiCanvasDescriptor &canvas) override
    {
        if(!ValidateCanvas(canvas))
            return;

        daisy::OneBitGraphicsDisplay &display
            = *(daisy::OneBitGraphicsDisplay *)(canvas.handle_);

        InternalDraw(display);
    }

  protected:
    /// @brief
    /// @param display
    virtual void InternalDraw(daisy::OneBitGraphicsDisplay &display)
    {
        // no items or out of bounds??!
        if((selectedItemIdx_ < 0) || (selectedItemIdx_ >= numItems_))
            return;

        // Experimenting for now
        FontDef fd = GetFontDef(FONT_SIZE);

        Rectangle baseRect(display.GetBounds()
                               .WithHeight(fd.FontHeight + 2)
                               .WithTrimmedLeft(1));

        int16_t charH = baseRect.GetHeight();

        const int16_t line = 0; // I don't remember what this was for...
        const int16_t maxItems
            = std::min(static_cast<int16_t>(numItems_),
                       static_cast<int16_t>(display.Height() / charH - line));

        int16_t startIdx;
        if(centerScrolling_)
            startIdx = std::max(0,
                                std::min(selectedItemIdx_ - (maxItems / 2),
                                         numItems_ - maxItems));
        else
            startIdx = selectedItemIdx_ >= maxItems
                           ? std::max(0, selectedItemIdx_ - maxItems + 1)
                           : 0;

        int16_t yOfs = 0;
        for(int16_t itemIdx = startIdx; itemIdx < (startIdx + maxItems);
            itemIdx++, yOfs += charH)
        {
            Rectangle itemRect   = baseRect.Translated(0, yOfs);
            bool      isSelected = (itemIdx == selectedItemIdx_);

            FixedCapStr<20> itemText;

            // This is just a WIP.  For now
            // based on the item append some 
            // sort of indicator text if possible
            itemText.Append(items_[itemIdx].text);

            switch(items_[itemIdx].type)
            {
                // Mostly just being sort of cheap for now.
                case ItemType::checkboxItem:
                    if(*items_[itemIdx].asCheckboxItem.valueToModify)
                        itemText.Append("=YES");
                    else
                        itemText.Append("=NO");
                    break;
                case ItemType::openUiPageItem:
                    itemText.Append("...");
                    break;
                case ItemType::closeMenuItem:
                    itemText.Append(" X");
                    break;
                case ItemType::valueItem:
                    itemText.Append(' ');
                    (*items_[itemIdx].asMappedValueItem.valueToModify).AppentToString(itemText);
                    break;
                default:
                case ItemType::customItem:
                case ItemType::callbackFunctionItem:
                    break;
            }
            if(isSelected)
                display.DrawRect(itemRect, true, true);
            display.WriteStringAligned(itemText.Cstr(),
                                       fd,
                                       itemRect,
                                       Alignment::centered,
                                       !isSelected);
        }
    }

    /// @brief
    /// @param canvas
    /// @return
    bool ValidateCanvas(const daisy::UiCanvasDescriptor &canvas) const
    {
        // Find out if this canvas is one we should draw to.
        if(canvasIdToDrawTo_ == daisy::UI::invalidCanvasId)
        {
            // We're configured to use the UIs default canvas.
            auto *ui = GetParentUI();
            if(!ui)
                // No parent UI?! How are we supposed to find out what cannvas to draw
                // to?
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

    /// @brief
    /// @param size
    /// @return
    const FontDef &GetFontDef(MenuFontSize size)
    {
        switch(size)
        {
            case MenuFontSize::FONT_TOOBIG: return Font_16x26;
            case MenuFontSize::FONT_BIG: return Font_11x18;
            case MenuFontSize::FONT_NORMAL: return Font_7x10;
            default:
            case MenuFontSize::FONT_SMALL: return Font_6x8;
        }
    }

  private:
    uint16_t canvasIdToDrawTo_ = daisy::UI::invalidCanvasId;
    bool     centerScrolling_  = true;
};