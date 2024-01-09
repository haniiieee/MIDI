// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once
#include "MidiSynthesizer.g.h"


namespace winrt::MIDI_ROOT_NAMESPACE_CPP::implementation
{
    struct MidiSynthesizer : MidiSynthesizerT<MidiSynthesizer>
    {
        MidiSynthesizer() = default;

        hstring DeviceId();
        void SendMessage(winrt::MIDI_ROOT_NAMESPACE_CPP::IMidiMessage const& midiMessage);
        void SendBuffer(winrt::Windows::Storage::Streams::IBuffer const& midiData);
    };
}
