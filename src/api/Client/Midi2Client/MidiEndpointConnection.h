// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#pragma once

#include <pch.h>


#include "MidiEndpointConnection.g.h"

#include "midi_service_interface.h"

#include "MidiMessageReceivedEventArgs.h"


namespace winrt::Windows::Devices::Midi2::implementation
{
    struct MidiEndpointConnection : MidiEndpointConnectionT<MidiEndpointConnection, IMidiCallback> 
    {
        MidiEndpointConnection() = default;
        ~MidiEndpointConnection();

        static hstring GetDeviceSelector() noexcept { return L"System.Devices.InterfaceClassGuid:=\"{E7CCE071-3C03-423f-88D3-F1045D02552B}\" AND System.Devices.InterfaceEnabled:=System.StructuredQueryType.Boolean#True"; }


        winrt::guid ConnectionId() const noexcept { return m_connectionId; }
        winrt::hstring EndpointDeviceId() const noexcept { return m_endpointDeviceId; }


        bool IsOpen() const noexcept { return m_isOpen; }
        midi2::IMidiEndpointDefinedConnectionSettings Settings() const noexcept { return m_settings; }

        foundation::IInspectable Tag() const noexcept { return m_tag; }
        void Tag(_In_ foundation::IInspectable value) noexcept { m_tag = value; }


        _Success_(return == true)
        bool InternalInitialize(
            _In_ winrt::com_ptr<IMidiAbstraction> serviceAbstraction,
            _In_ winrt::guid const connectionId,
            _In_ winrt::hstring const endpointDeviceId,
            _In_ midi2::MidiEndpointConnectionOptions options);


        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessagePacket(
                _In_ midi2::IMidiUniversalPacket const& ump);


        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageStruct(
                _In_ internal::MidiTimestamp timestamp,
                _In_ midi2::MidiMessageStruct const& message,
                _In_ uint8_t wordCount);


        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageWords(
                _In_ internal::MidiTimestamp const timestamp,
                _In_ uint32_t const word0);

        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageWords(
                _In_ internal::MidiTimestamp const timestamp,
                _In_ uint32_t const word0,
                _In_ uint32_t const word1);

        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageWords(
                _In_ internal::MidiTimestamp const timestamp,
                _In_ uint32_t const word0,
                _In_ uint32_t const word1,
                _In_ uint32_t const word2);

        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageWords(
                _In_ internal::MidiTimestamp const timestamp,
                _In_ uint32_t const word0,
                _In_ uint32_t const word1,
                _In_ uint32_t const word2,
                _In_ uint32_t const word3);


        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageWordArray(
                _In_ internal::MidiTimestamp const timestamp,
                _In_ winrt::array_view<uint32_t const> words,
                _In_ uint32_t const startIndex,
                _In_ uint8_t const wordCount);



        _Success_(return == true)
            midi2::MidiSendMessageResult SendMessageBuffer(
                _In_ internal::MidiTimestamp timestamp,
                _In_ foundation::IMemoryBuffer const& buffer,
                _In_ uint32_t byteOffset,
                _In_ uint8_t byteLength);













        _Success_(return == true)
        bool Open();

        // IClosable
        void Close();

        STDMETHOD(Callback)(_In_ PVOID data, _In_ UINT size, _In_ LONGLONG timestamp) override;

        winrt::event_token MessageReceived(_In_ foundation::TypedEventHandler<foundation::IInspectable, midi2::MidiMessageReceivedEventArgs> const& handler)
        {
            return m_messageReceivedEvent.add(handler);
        }

        void MessageReceived(_In_ winrt::event_token const& token) noexcept
        {
            if (m_messageReceivedEvent) m_messageReceivedEvent.remove(token);
        }


        winrt::Windows::Foundation::Collections::IVector<midi2::IMidiEndpointMessageProcessingPlugin> MessageProcessingPlugins() const noexcept
        {
            return m_messageProcessingPlugins;
        }

    private:
        winrt::guid m_connectionId{};
        winrt::hstring m_endpointDeviceId{};
        winrt::Windows::Foundation::IInspectable m_tag{ nullptr };

        bool m_isOpen{ false };
        bool m_closeHasBeenCalled{ false };

        winrt::com_ptr<IMidiAbstraction> m_serviceAbstraction{ nullptr };
        winrt::com_ptr<IMidiBiDi> m_endpointAbstraction{ nullptr };


        winrt::event<foundation::TypedEventHandler<foundation::IInspectable, midi2::MidiMessageReceivedEventArgs>> m_messageReceivedEvent;

        midi2::MidiEndpointConnectionOptions m_options;

        foundation::Collections::IVector<midi2::IMidiEndpointMessageProcessingPlugin>
            m_messageProcessingPlugins{ winrt::single_threaded_vector<midi2::IMidiEndpointMessageProcessingPlugin>() };


        midi2::IMidiEndpointDefinedConnectionSettings m_settings{ nullptr };


        bool ValidateUmp(_In_ uint32_t word0, _In_ uint8_t wordCount) noexcept;


        _Success_(return == true)
        midi2::MidiSendMessageResult SendUmpInternal(
            _In_ winrt::com_ptr<IMidiBiDi> endpoint,
            _In_ midi2::IMidiUniversalPacket const& ump);

        _Success_(return == true)
        midi2::MidiSendMessageResult SendMessageRaw(
            _In_ winrt::com_ptr<IMidiBiDi> endpoint,
            _In_ void* data,
            _In_ uint8_t sizeInBytes,
            _In_ internal::MidiTimestamp timestamp);

        _Success_(return != nullptr)
        void* GetUmpDataPointer(
            _In_ midi2::IMidiUniversalPacket const& ump,
            _Out_ uint8_t & dataSizeOut);

        _Success_(return == true)
        bool ActivateMidiStream(
            _In_ winrt::com_ptr<IMidiAbstraction> serviceAbstraction,
            _In_ const IID & iid,
            _Out_ void** iface) noexcept;

        void InitializePlugins() noexcept;
        void CallOnConnectionOpenedOnPlugins() noexcept;
        void CleanupPlugins() noexcept;

    };
}
namespace winrt::Windows::Devices::Midi2::factory_implementation
{
    struct MidiEndpointConnection : MidiEndpointConnectionT<MidiEndpointConnection, implementation::MidiEndpointConnection>
    {
    };
}