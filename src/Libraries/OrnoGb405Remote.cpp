#include "OrnoGb405Remote.h"

#define SAMPLINGRATE 3200000
#define SDELAY(X) delayMicroseconds((1000000.0 / (float)SAMPLINGRATE) * X)

char _on1[] =  "100010101";


OrnoGb405Remote::OrnoGb405Remote(int txPin)
{
	_txPin = txPin;
}

void OrnoGb405Remote::SetCode(const char* code)
{
	strcpy_P(_codeBlock, PSTR("11111111110"));
	for (int i = 0; i < 5; i ++)
	{
		_codeBlock[i * 2] = code[i];
	}
}

void OrnoGb405Remote::TurnOn(int outputNumber)
{
	for (int i = 0; i < 7; i++)
	{
		if (i > 0)
		{
			SDELAY(13585);
		}
		TransmitCommandSegment(_codeBlock, outputNumber, "01000");
	}
	SDELAY(13585);
	TransmitCommandSegment(_codeBlock, outputNumber, "11110");
}

void OrnoGb405Remote::TurnOff(int outputNumber)
{
	for (int i = 0; i < 7; i++)
	{
		if (i > 0)
		{
			SDELAY(13585);
		}
		TransmitCommandSegment(_codeBlock, outputNumber, "00010");
	}
	SDELAY(13585);
	TransmitCommandSegment(_codeBlock, outputNumber, "11110");
}

void OrnoGb405Remote::Set(int ouputNumber, bool value)
{
	if (value)
	{
		TurnOn(ouputNumber);
	}
	else
	{
		TurnOff(ouputNumber);
	}
}

void OrnoGb405Remote::TransmitCommandSegment(char *code, int switchNumber, const char *command)
{
	char switchId[10];	
	strcpy_P(switchId,PSTR("101010101"));
	switchId[switchNumber*2] = '0';

	TrDataBlock(code);
	TrDataBlock(switchId);
	TrDataBlock(command);
}


void OrnoGb405Remote::TrDataBlock(const char *data)
{
	for (int i = 0; i < strlen(data); i++)
	{
		int digit = data[i] - '0';
		TrDigit(digit);
	}
}

void OrnoGb405Remote::TrDigit(int digit)
{
	switch (digit)
	{
	case 0:
		TrHigh();
		SDELAY(472);
		TrLow();
		SDELAY(1469);
		break;
	case 1:
		TrHigh();
		SDELAY(1440);
		TrLow();
		SDELAY(501);
		break;
	default:
		Serial.println("Invalid digit in code");
	}
}


void OrnoGb405Remote::TrLow()
{
	digitalWrite(_txPin, LOW);
}
void OrnoGb405Remote::TrHigh()
{
	digitalWrite(_txPin, HIGH);
}

