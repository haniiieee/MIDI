// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License
// ============================================================================
// This is part of the Windows MIDI Services App API and should be used
// in your Windows application via an official binary distribution.
// Further information: https://github.com/microsoft/MIDI/
// ============================================================================

#include "pch.h"
#include "MidiBidirectionalEndpointConnection.h"
#include "MidiBidirectionalEndpointConnection.g.cpp"


namespace winrt::Windows::Devices::Midi2::implementation
{
    // Callback handler from the Midi Service endpoint abstraction

    IFACEMETHODIMP MidiBidirectionalEndpointConnection::Callback(_In_ PVOID Data, _In_ UINT Size, _In_ LONGLONG Timestamp)
    {
        try
        {
            bool skipMainMessageReceivedEvent = false;
            bool skipFurtherListeners = false;

            // If any listeners are hooked up, use them

            if (m_messageListeners && m_messageListeners.Size() > 0)
            {
                // loop through listeners

                for (const auto& listener : m_messageListeners)
                {
                    auto listenerArgs = winrt::make<MidiMessageReceivedEventArgs>(Data, Size, Timestamp);

                    // TODO: this is synchronous by design, but that requires the client to not block
                    listener.ProcessIncomingMessage((IMidiInputConnection)*this, listenerArgs, skipFurtherListeners, skipMainMessageReceivedEvent);

                    // if the listener has told us to skip further listeners, then do so
                    if (skipFurtherListeners) break;
                }
            }

            // if the main message received event is hooked up, use it

            if (m_messageReceivedEvent)
            {
                auto args = winrt::make_self<MidiMessageReceivedEventArgs>(Data, Size, Timestamp);

                if (!skipMainMessageReceivedEvent)
                {
                    if (args != nullptr)
                    {
                        m_messageReceivedEvent(*this, *args);
                    }
                    else
                    {
                        return E_FAIL;
                    }
                }
            }

            return S_OK;
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L"hresult exception calling message handlers", ex);

            return false;
        }
    }


    bool MidiBidirectionalEndpointConnection::SendUmpBuffer(internal::MidiTimestamp timestamp, winrt::Windows::Foundation::IMemoryBuffer const& buffer, uint32_t byteOffset, uint32_t byteLength)
    {
        try
        {
            // make sure we're sending only a single UMP
            uint32_t sizeInWords = byteLength / sizeof(uint32_t);

            if (!internal::IsValidSingleUmpWordCount(sizeInWords))
            {
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for a single UMP", sizeInWords, timestamp);

                //throw hresult_invalid_argument();
                return false;
            }

            Windows::Foundation::IMemoryBufferReference bufferReference = buffer.CreateReference();

            auto interop = bufferReference.as<IMemoryBufferByteAccess>();

            uint8_t* dataPointer;
            uint32_t dataSize;
            winrt::check_hresult(interop->GetBuffer(&dataPointer, &dataSize));

            // make sure we're not going to spin past the end of the buffer
            if (byteOffset + byteLength > bufferReference.Capacity())
            {
                // TODO: Log

                internal::LogGeneralError(__FUNCTION__, L"Buffer smaller than provided offset + byteLength");

                return false;
            }


            // send the ump

            return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)(dataPointer + byteOffset), byteLength, timestamp);
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__,  L"hresult exception sending message", ex);

            return false;
        }
    }


    // sends a single UMP's worth of words
    bool MidiBidirectionalEndpointConnection::SendUmpWordArray(internal::MidiTimestamp timestamp, array_view<uint32_t const> words, uint32_t wordCount)
    {
        try
        {
            if (!internal::IsValidSingleUmpWordCount(wordCount))
            {
                //throw hresult_invalid_argument();
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for a single UMP", wordCount, timestamp);

                return false;
            }

            if (internal::GetUmpLengthInMidiWordsFromFirstWord(words[0]) != wordCount)
            {
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for messageType", wordCount, timestamp);

                return false;
            }


            if (m_endpointInterface)
            {
                auto umpDataSize = (uint32_t)(sizeof(uint32_t) * wordCount);

                // if the service goes down, this will fail

                return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)words.data(), umpDataSize, timestamp);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception sending message", ex);

            return false;
        }
    }


    bool MidiBidirectionalEndpointConnection::SendUmpWords(internal::MidiTimestamp timestamp, uint32_t word0)
    {
        try
        {
            if (internal::GetUmpLengthInMidiWordsFromFirstWord(word0) != UMP32_WORD_COUNT)
            {
                // mismatch between the message type and the number of words
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for messageType", UMP32_WORD_COUNT, timestamp);
                return false;
            }


            if (m_endpointInterface)
            {
                auto umpByteCount = (uint32_t)(sizeof(internal::PackedUmp32));

                // if the service goes down, this will fail

                return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)&word0, umpByteCount, timestamp);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception sending message", ex);

            return false;
        }
    }

    bool MidiBidirectionalEndpointConnection::SendUmpWords(internal::MidiTimestamp timestamp, uint32_t word0, uint32_t word1)
    {
        try
        {
            if (internal::GetUmpLengthInMidiWordsFromFirstWord(word0) != UMP64_WORD_COUNT)
            {
                // mismatch between the message type and the number of words
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for messageType", UMP64_WORD_COUNT, timestamp);
                return false;
            }


            if (m_endpointInterface)
            {
                auto umpByteCount = (uint32_t)(sizeof(internal::PackedUmp64));
                internal::PackedUmp64 ump{};

                ump.word0 = word0;
                ump.word1 = word1;

                // if the service goes down, this will fail

                return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)&ump, umpByteCount, timestamp);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception sending message", ex);

            return false;
        }
    }

    bool MidiBidirectionalEndpointConnection::SendUmpWords(internal::MidiTimestamp timestamp, uint32_t word0, uint32_t word1, uint32_t word2)
    {
        try
        {
            if (internal::GetUmpLengthInMidiWordsFromFirstWord(word0) != UMP96_WORD_COUNT)
            {
                // mismatch between the message type and the number of words
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for messageType", UMP96_WORD_COUNT, timestamp);
                return false;
            }


            if (m_endpointInterface)
            {
                auto umpByteCount = (uint32_t)(sizeof(internal::PackedUmp96));
                internal::PackedUmp96 ump{};

                ump.word0 = word0;
                ump.word1 = word1;
                ump.word2 = word2;

                // if the service goes down, this will fail

                return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)&ump, umpByteCount, timestamp);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception sending message", ex);

            return false;
        }
    }

    bool MidiBidirectionalEndpointConnection::SendUmpWords(internal::MidiTimestamp timestamp, uint32_t word0, uint32_t word1, uint32_t word2, uint32_t word3)
    {
        try
        {
            if (internal::GetUmpLengthInMidiWordsFromFirstWord(word0) != UMP128_WORD_COUNT)
            {
                internal::LogUmpSizeValidationError(__FUNCTION__, L"Word count is incorrect for messageType", UMP128_WORD_COUNT, timestamp);
                return false;
            }


            if (m_endpointInterface)
            {
                auto umpByteCount = (uint32_t)(sizeof(internal::PackedUmp128));
                internal::PackedUmp128 ump{};

                ump.word0 = word0;
                ump.word1 = word1;
                ump.word2 = word2;
                ump.word3 = word3;

                // if the service goes down, this will fail

                return m_messageSenderHelper.SendMessageRaw(m_endpointInterface, (void*)&ump, umpByteCount, timestamp);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception sending message", ex);

            return false;
        }
    }



    bool MidiBidirectionalEndpointConnection::SendUmp(winrt::Windows::Devices::Midi2::IMidiUmp const& ump)
    {
        try
        {
            if (m_endpointInterface)
            {
                return m_messageSenderHelper.SendUmp(m_endpointInterface, ump);
            }
            else
            {
                internal::LogGeneralError(__FUNCTION__, L"Endpoint is nullptr");

                return false;
            }
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L"hresult exception sending message. Service may be unavailable", ex);

            return false;
        }
    }


    bool MidiBidirectionalEndpointConnection::ActivateMidiStream(com_ptr<IMidiAbstraction> serviceAbstraction, const IID& iid, void** iface)
    {
        try
        {
            winrt::check_hresult(serviceAbstraction->Activate(iid, iface));

            return true;
        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L"hresult exception activating stream. Service may be unavailable", ex);

            return false;
        }
    }
    
    // internal method to start listening for incoming messages, enable processing outgoing messages, etc.
    // TODO: Change signature of this
    bool MidiBidirectionalEndpointConnection::InternalStart(winrt::com_ptr<IMidiAbstraction> serviceAbstraction)
    {
        WINRT_ASSERT(!DeviceId().empty());
        WINRT_ASSERT(serviceAbstraction != nullptr);

        DWORD mmcssTaskId;  // TODO: Does this need to be session-wise? Probably, but it can be modified by the endpoint init, so maybe should be endpoint-local

        // Activate the endpoint for this device. Will fail if the device is not a BiDi device
        if (!ActivateMidiStream(serviceAbstraction, __uuidof(IMidiBiDi), (void**)&m_endpointInterface))
        {
            internal::LogGeneralError(__FUNCTION__, L"Could not activate MIDI Stream");

            return false;
        }

        try
        {
            // TODO: Need to handle the output only case which has no callback
            winrt::check_hresult(m_endpointInterface->Initialize(
                (LPCWSTR)(DeviceId().c_str()),
                &mmcssTaskId,
                (IMidiCallback*)(this)
            ));

            m_isConnected = true;

            return true;

        }
        catch (winrt::hresult_error const& ex)
        {
            internal::LogHresultError(__FUNCTION__, L" hresult exception initializing endpoint interface. Service may be unavailable.", ex);

            m_endpointInterface = nullptr;

            return false;
        }

    }

    MidiBidirectionalEndpointConnection::~MidiBidirectionalEndpointConnection()
    {
        if (m_endpointInterface != nullptr)
        {
            m_endpointInterface->Cleanup();
        }

        m_isConnected = false;

        // TODO: any event cleanup?
    }

}