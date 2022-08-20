// ------------------------------------------------------------------------------------------------
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See LICENSE in the GitHub project root for license information.
// ------------------------------------------------------------------------------------------------

#define WINDOWSMIDISERVICES_EXPORTS

#include "WindowsMidiServicesUtility.h"
#include "WindowsMidiServicesMessages.h"

namespace Microsoft::Windows::Midi::Messages
{
	const uint32_t Midi2ChannelPressureMessage::getData()
	{
		return 0;
	}

	void Midi2ChannelPressureMessage::setData(const uint32_t value)
	{
	}

	Midi2ChannelPressureMessage Midi2ChannelPressureMessage::FromValues(const uint8_t group, const uint8_t channel, const uint32_t data)
	{
		Midi2ChannelPressureMessage msg;
		return msg;

	}

}