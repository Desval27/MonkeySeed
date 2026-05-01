#include <daisy.h>

class ShapeItem : public daisy::AbstractMenu::CustomItem {
public:
  void Draw(daisy::OneBitGraphicsDisplay &display, int currentIndex, int numItemsTotal,
            daisy::Rectangle boundsToDrawIn, bool isEditing) override {
                display.DrawRect(boundsToDrawIn, true, false);
            }
};