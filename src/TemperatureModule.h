#pragma once

#include <Arduino.h>
#include "Libraries/DallasTemperature/DallasTemperature.h"
#include "Libraries/OneWire/OneWire.h"

#include <Thingify.h>
#include <IModule.h>
#include <Logging/Logger.h>

class TemperatureModule: public IModule
{
	OneWire* _oneWire;
	DallasTemperature* _ds1820;
	uint64_t lastTemperatureCheck;
	Thingify& _thing;
	int _pin;
	float _temperature;
	Logger& _logger;
	std::vector<Node*> _nodes;
	Node* FindNodeByName(const char* name);
public:
	const char* GetName() override;
	float GetTemperature(int sensorIndex) const;
	TemperatureModule(Thingify& thing, int pin);
	bool Init() override;
	bool Tick() override;	
};

