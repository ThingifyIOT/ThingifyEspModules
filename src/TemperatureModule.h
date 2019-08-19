#pragma once

#include <Arduino.h>
#include "Libraries/DallasTemperature/DallasTemperature.h"
#include "Libraries/OneWire/OneWire.h"

#include <Conti.h>
#include <IModule.h>

class TemperatureModule: public IModule
{
	OneWire* _oneWire;
	int _pin;
	DallasTemperature* _ds1820;
	uint64_t lastTemperatureCheck;
	Conti* _device;
	float _temperature;
	std::vector<Node*> _nodes;
	Node* FindNodeByName(const char* name);
public:
	const char* GetName() override;
	float GetTemperature(int sensorIndex) const;
	TemperatureModule(int pin, Conti* device);
	bool Init() override;
	bool Tick() override;	
};

