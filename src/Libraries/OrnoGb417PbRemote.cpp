#include "OrnoGb417PbRemote.h"

OrnoGb417PbRemote::OrnoGb417PbRemote(int txPin)
{
	_txPin = txPin;
}

void OrnoGb417PbRemote::Init()
{
	_rcSwitch.enableTransmit(_txPin);

	_rcSwitch.setPulseLength(400);

	_rcSwitch.setProtocol(4);

	_rcSwitch.setRepeatTransmit(6);
}

void OrnoGb417PbRemote::TurnOn(int outputNumber)
{
	if(outputNumber > 4)
	{
		return;
	}
	Serial.printf("On %s\n", _onCodes[outputNumber]);
	_rcSwitch.sendTriState(_onCodes[outputNumber]);
}

void OrnoGb417PbRemote::TurnOff(int outputNumber)
{
	if (outputNumber > 4)
	{
		return;
	}
	Serial.printf("Off %s\n", _onCodes[outputNumber]);

	_rcSwitch.sendTriState(_offCodes[outputNumber]);
}

void OrnoGb417PbRemote::Set(int ouputNumber, bool value)
{
	if(value)
	{
		TurnOn(ouputNumber);
	}
	else
	{
		TurnOff(ouputNumber);
	}
}


