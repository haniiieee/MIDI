// Copyright (c) Microsoft Corporation. All rights reserved.
#pragma once

class CMidi2KSMidiBiDi : 
    public Microsoft::WRL::RuntimeClass<
        Microsoft::WRL::RuntimeClassFlags<Microsoft::WRL::ClassicCom>,
        IMidiBiDi,
        IMidiCallback>
{
public:

    STDMETHOD(Initialize(_In_ LPCWSTR, _In_ DWORD *, _In_opt_ IMidiCallback *));
    STDMETHOD(SendMidiMessage(_In_ PVOID , _In_ UINT , _In_ LONGLONG));
    STDMETHOD(Callback)(_In_ PVOID, _In_ UINT, _In_ LONGLONG);
    STDMETHOD(Cleanup)();

private:

    std::unique_ptr<KSMidiInDevice> m_MidiInDevice;
    std::unique_ptr<KSMidiOutDevice> m_MidiOutDevice;

    wil::com_ptr_nothrow<IMidiCallback> m_MidiInCallback;
};

