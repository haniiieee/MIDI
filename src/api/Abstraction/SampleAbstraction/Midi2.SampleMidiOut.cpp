// Copyright (c) Microsoft Corporation. All rights reserved.

#include "pch.h"
#include "midi2.sampleabstraction.h"

_Use_decl_annotations_
HRESULT
CMidi2SampleMidiOut::Initialize(
    LPCWSTR,
    PABSTRACTIONCREATIONPARAMS,
    DWORD *,
    GUID /* SessionId */

)
{
    TraceLoggingWrite(
        MidiSampleAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
        );

    return E_NOTIMPL;
}

HRESULT
CMidi2SampleMidiOut::Cleanup()
{
    TraceLoggingWrite(
        MidiSampleAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
        );

    return E_NOTIMPL;
}

_Use_decl_annotations_
HRESULT
CMidi2SampleMidiOut::SendMidiMessage(
    PVOID,
    UINT,
    LONGLONG
)
{
    return E_NOTIMPL;
}

