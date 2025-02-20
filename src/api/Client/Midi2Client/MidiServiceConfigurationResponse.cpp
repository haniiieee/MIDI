// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiServiceConfigurationResponse.h"
#include "MidiServiceConfigurationResponse.g.cpp"

namespace winrt::Windows::Devices::Midi2::implementation
{
    winrt::Windows::Devices::Midi2::MidiServiceConfigurationResponseStatus MidiServiceConfigurationResponse::Status()
    {
        throw hresult_not_implemented();
    }
    hstring MidiServiceConfigurationResponse::ResponseJson()
    {
        throw hresult_not_implemented();
    }
}
