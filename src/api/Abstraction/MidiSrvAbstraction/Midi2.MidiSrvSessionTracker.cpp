// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"


HRESULT
CMidi2MidiSrvSessionTracker::Initialize()
{
    return S_OK;
}

_Use_decl_annotations_
HRESULT
CMidi2MidiSrvSessionTracker::AddClientSession(
    GUID SessionId,
    LPCWSTR SessionName,
    DWORD ClientProcessId,
    LPCWSTR ClientProcessName
)
{
    TraceLoggingWrite(
        MidiSrvAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
    );

    wil::unique_rpc_binding bindingHandle;

    RETURN_IF_FAILED(GetMidiSrvBindingHandle(&bindingHandle));
    RETURN_HR_IF_NULL(E_INVALIDARG, ClientProcessName);
    RETURN_HR_IF_NULL(E_INVALIDARG, SessionName);

    RETURN_IF_FAILED([&]()
        {
            // RPC calls are placed in a lambda to work around compiler error C2712, limiting use of try/except blocks
            // with structured exception handling.
            RpcTryExcept RETURN_IF_FAILED(MidiSrvRegisterSession(bindingHandle.get(), SessionId, SessionName, ClientProcessId, ClientProcessName));
            RpcExcept(I_RpcExceptionFilter(RpcExceptionCode())) RETURN_IF_FAILED(HRESULT_FROM_WIN32(RpcExceptionCode()));
            RpcEndExcept
            
            return S_OK;
        }());

    return S_OK;
}

_Use_decl_annotations_
HRESULT
CMidi2MidiSrvSessionTracker::RemoveClientSession(
    GUID SessionId
)
{
    TraceLoggingWrite(
        MidiSrvAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
    );

    wil::unique_rpc_binding bindingHandle;

    RETURN_IF_FAILED(GetMidiSrvBindingHandle(&bindingHandle));

    RETURN_IF_FAILED([&]()
        {
            // RPC calls are placed in a lambda to work around compiler error C2712, limiting use of try/except blocks
            // with structured exception handling.
            RpcTryExcept RETURN_IF_FAILED(MidiSrvDeregisterSession(bindingHandle.get(), SessionId));
            RpcExcept(I_RpcExceptionFilter(RpcExceptionCode())) RETURN_IF_FAILED(HRESULT_FROM_WIN32(RpcExceptionCode()));
            RpcEndExcept

            return S_OK;
        }());

    return S_OK;
}

//_Use_decl_annotations_
//HRESULT
//CMidi2MidiSrvSessionTracker::AddClientEndpointConnection(
//    GUID SessionId,
//    LPCWSTR ConnectionEndpointInterfaceId
//)
//{
//    UNREFERENCED_PARAMETER(SessionId);
//    UNREFERENCED_PARAMETER(ConnectionEndpointInterfaceId);
//
//    return S_OK;
//}
//
//_Use_decl_annotations_
//HRESULT
//CMidi2MidiSrvSessionTracker::RemoveClientEndpointConnection(
//    GUID SessionId,
//    LPCWSTR ConnectionEndpointInterfaceId
//)
//{
//    UNREFERENCED_PARAMETER(SessionId);
//    UNREFERENCED_PARAMETER(ConnectionEndpointInterfaceId);
//
//    return S_OK;
//}

_Use_decl_annotations_
HRESULT
CMidi2MidiSrvSessionTracker::GetSessionListJson(
    BSTR* SessionList
)
{
    TraceLoggingWrite(
        MidiSrvAbstractionTelemetryProvider::Provider(),
        __FUNCTION__,
        TraceLoggingLevel(WINEVENT_LEVEL_INFO),
        TraceLoggingPointer(this, "this")
    );

    wil::unique_rpc_binding bindingHandle;

    RETURN_IF_FAILED(GetMidiSrvBindingHandle(&bindingHandle));
    RETURN_HR_IF_NULL(E_INVALIDARG, SessionList);

    RETURN_IF_FAILED([&]()
        {
            // RPC calls are placed in a lambda to work around compiler error C2712, limiting use of try/except blocks
            // with structured exception handling.
            RpcTryExcept RETURN_IF_FAILED(MidiSrvGetSessionList(bindingHandle.get(), SessionList));
            RpcExcept(I_RpcExceptionFilter(RpcExceptionCode())) RETURN_IF_FAILED(HRESULT_FROM_WIN32(RpcExceptionCode()));
            RpcEndExcept
                return S_OK;
        }());

    return S_OK;
}

HRESULT
CMidi2MidiSrvSessionTracker::Cleanup()
{
    return S_OK;
}