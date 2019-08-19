#pragma once
class OrnoSmartLivingRemote
{
	const char* on1 = "122222222232132122222223132122222";
	const char*  off1 = "122222222232132122222223132212222";
	const char*  on2 = "122222222232132122222223132122231";
	const char*  off2 = "122222222232132122222223132212231";

	int _txPin;
public:
	OrnoSmartLivingRemote(int txPin);
	void Set(int deviceNumber, bool onOff);
	void TransmitCommand(const char* command);
	void TrDataBlock(const char *data);
	void TrStart();
	void TrDigit(int digit);
	void TrLow();
	void TrHigh();
};

