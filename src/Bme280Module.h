#ifndef _BME_280_MODULE_H
#define _BME_280_MODULE_H

#include <Conti.h>
#include <IModule.h>
#include "Libraries/BME280/BME280I2C.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>


class Bme280Module : public IModule
{	
	ContiLogger& _logger;
	Conti& _thing;
	uint8_t _bmeAddress;
	BME280I2C::Settings _bmeSettings;
	BME280I2C _bme;
	TwoWire& _wire;
	bool _isSensorPresent;
	Node* _temperatureNode;
	Node* _pressureNode;
	Node* _humidityNode;
	double Round(double x);
	uint64_t _connectTime;
public:
	Bme280Module(Conti& thing, TwoWire& wire, uint8_t bmeAddress = 0x76);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif