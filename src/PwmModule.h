#pragma once

#include "Libraries\Adafruit_PWMServoDriver.h"
#include <Conti.h>
#include <IModule.h>
#include <map>
#include <Wire.h>

class PwmModule: public IModule
{
private:
	Conti &_device;
	std::map<uint16_t, int> _nodeToOutputNumberMap;
	static const int PwmOutputCount = 16;
	uint16_t _requestedPwmValues[PwmOutputCount];
	uint16_t _actualPwmValues[PwmOutputCount];

	float _pwmFrequency;
public:
	const char* GetName() override;
	bool Tick() override;
	bool Init() override;

	Adafruit_PWMServoDriver _pca9685;

	PwmModule(Conti &device, TwoWire& wire, uint8_t i2cAddress = 0x40, float pwmFrequency = 10000);
	bool OnValueChanged(Node *node) override;
	Node* AddPwmOutput(const char *name, int outputNumber);
	Node* AddOnOffOutput(const char *name, int outputNumber);

	bool Set(int outputNumber, bool value);
};

