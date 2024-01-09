// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiChannelPressureMessage.g.h"


namespace winrt::MIDI_ROOT_NAMESPACE_CPP::implementation
{
    struct MidiChannelPressureMessage : MidiChannelPressureMessageT<MidiChannelPressureMessage>
    {
        MidiChannelPressureMessage() = default;

        MidiChannelPressureMessage(uint8_t channel, uint8_t pressure);
        uint8_t Channel();
        uint8_t Pressure();
        winrt::Windows::Foundation::TimeSpan Timestamp();
        winrt::MIDI_ROOT_NAMESPACE_CPP::MidiMessageType Type();
        winrt::Windows::Storage::Streams::IBuffer RawData();
    };
}
namespace winrt::MIDI_ROOT_NAMESPACE_CPP::factory_implementation
{
    struct MidiChannelPressureMessage : MidiChannelPressureMessageT<MidiChannelPressureMessage, implementation::MidiChannelPressureMessage>
    {
    };
}
