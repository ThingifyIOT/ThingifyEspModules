#include <Arduino.h>

#include "OrnoSmartLivingRemote.h"


#define SAMPLINGRATE 3200000
#define SDELAY(X) delayMicroseconds((1000000.0 / (float)SAMPLINGRATE) * X)

OrnoSmartLivingRemote::OrnoSmartLivingRemote(int txPin)
{
	_txPin = txPin;
	
}

void OrnoSmartLivingRemote::Set(int deviceNumber, bool onOff)
{
	if (deviceNumber == 0)
	{
		if (onOff)
		{
			TransmitCommand(on1);
		}
		else
		{
			TransmitCommand(off1);
		}
	}
	if (deviceNumber == 1)
	{
		if (onOff)
		{
			TransmitCommand(on2);
		}
		else
		{
			TransmitCommand(off2);
		}
	}
}


void OrnoSmartLivingRemote::TransmitCommand(const char* command)
{
	TrStart();
	TrDataBlock(command);
	SDELAY(31000);
	TrDataBlock(command);
	SDELAY(31000);
	TrDataBlock(command);
	SDELAY(31000);
	TrDataBlock("122222221");
}
void OrnoSmartLivingRemote::TrDataBlock(const char *data)
{
	for (int i = 0; i < strlen(data); i++)
	{
		if (i > 0)
		{
			SDELAY(4000);
		}
		int digit = data[i] - '0';
		TrDigit(digit);
	}
}


void OrnoSmartLivingRemote::TrStart()
{
	TrHigh();
	SDELAY(785);
	TrLow();
	SDELAY(7928);
}

void OrnoSmartLivingRemote::TrDigit(int digit)
{
	for (int i = 0; i < digit; i++)
	{
		if (i > 0)
		{
			SDELAY(845);
		}
		TrHigh();
		SDELAY(785);
		TrLow();
	}
}

void OrnoSmartLivingRemote::TrLow()
{
	digitalWrite(_txPin, LOW);
}
void OrnoSmartLivingRemote::TrHigh()
{
	digitalWrite(_txPin, HIGH);
}
