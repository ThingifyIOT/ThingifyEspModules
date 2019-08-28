#pragma once
#include <Arduino.h>

class OrnoGb405Remote
{
	void TrDataBlock(const char *data);
	void TrDigit(int digit);
	void TransmitCommandSegment(char *code, int switchNumber, const char *command);
	void TrLow();
	void TrHigh();
	int _txPin;
	char _codeBlock[13];

public:
	void SetCode(const char*code);
	OrnoGb405Remote(int txPin);
	void TurnOn(int outputNumber);
	void TurnOff(int outputNumber);
	void Set(int ouputNumber, bool value);
};

