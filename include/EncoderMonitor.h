/* SPDX-License-Identifier: CC0-1.0 */
/**
 * @file EncoderMonitor.h
 * @brief 
 * @author pfburdette <paul.f.burdette@gmail.com>
 *
 * @copyright This work is dedicated to the public domain under CC0 1.0.
 * To the extent possible under law, the author(s) have waived all copyright
 * and related or neighboring rights to this software.
 * See <http://creativecommons.org>
 */
#pragma once

#include <daisysp.h>

using namespace daisy;

template <typename BackendType, uint32_t numEncoders, uint16_t numSteps = 20U>
class EncoderMonitor
{
  public:
    EncoderMonitor()
    : queue_(nullptr),
      backend_(nullptr)
    {
    }

    void Init(UiEventQueue& queueToAddEventsTo,
              BackendType&  backend)
    {
        queue_              = &queueToAddEventsTo;
        backend_            = &backend;

        for(uint32_t i = 0; i < numEncoders; i++)
        {
            encoderActive_[i] = false;
            encoderIncrements_[i] = 0;
        }
    }

    /** Checks the value of each button and generates messages for the UIEventQueue.
     *  Call this at regular intervals, ideally from your main() idle loop.
     */
    void Process()
    {
        for(uint32_t i = 0; i < numEncoders; i++)
            ProcessEncoder(i, backend_->Increment(i));
    }

    /** Returns the BackendType that is used by the monitor. */
    BackendType& GetBackend() { return backend_; }

    /** Returns the number of encoders that are monitored by this class. */
    uint16_t GetNumEncodersMonitored() const { return numEncoders; }

  private:
    void ProcessEncoder(uint16_t id, int32_t inc)
    {
        if(inc != 0)
        {
            if(!encoderActive_[id])
            {
                queue_->AddEncoderActivityChanged(id, true);
                encoderActive_[id] = true;
                encoderIncrements_[id] = 0;
            }
            encoderIncrements_[id] = encoderIncrements_[id] + inc;
            queue_->AddEncoderTurned(id, encoderIncrements_[id], numSteps);
        }
        else
        {
            if(encoderActive_[id])
            {
                queue_->AddEncoderActivityChanged(id, false);
                encoderActive_[id] = false;
                encoderIncrements_[id] = 0;
            }
        }
    }

    EncoderMonitor(const EncoderMonitor&) = delete;
    EncoderMonitor& operator=(const EncoderMonitor&) = delete;

    UiEventQueue* queue_;
    BackendType*  backend_;
    int32_t       encoderIncrements_[numEncoders];
    bool          encoderActive_[numEncoders];
};