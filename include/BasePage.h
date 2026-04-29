#pragma once

#include <daisy_seed.h>
#include <daisysp.h>

class BasePage : public daisy::UiPage
{
public:
    BasePage() {}

    void Draw(const daisy::UiCanvasDescriptor &canvas) override
    {
        daisy::OneBitGraphicsDisplay& display = *(daisy::OneBitGraphicsDisplay*)(canvas.handle_);
        display.Fill(false); // Clear the display before drawing
        daisy::Rectangle bounds = display.GetBounds().WithTrimmedRight(1).WithTrimmedBottom(1);
        InternalDraw(display, bounds);
        display.Update();
    }

protected:
    virtual void InternalDraw(daisy::OneBitGraphicsDisplay& display, const daisy::Rectangle bounds) = 0;
};