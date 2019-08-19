#pragma once

#include <vector>
#include <Arduino.h>
#include <inttypes.h>
#include <alloca.h>

inline void onInterrupt();

class ClapSensor
{
	std::vector<ulong> readings;
	int _microphonePin;
public:
	ClapSensor(int microphonePin);
	void Handleinterrupt();
	void Init();
	bool IsEnabled;

	bool CheckClaps(int &clapCount);
};

