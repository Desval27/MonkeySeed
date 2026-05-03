/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file ShapeItem.h
 * @brief
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#include <daisy.h>

class ShapeItem : public daisy::AbstractMenu::CustomItem {
public:
  void Draw(daisy::OneBitGraphicsDisplay &display, int currentIndex, int numItemsTotal,
            daisy::Rectangle boundsToDrawIn, bool isEditing) override {
                display.DrawRect(boundsToDrawIn, true, false);
            }
};