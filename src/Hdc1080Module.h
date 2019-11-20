#ifndef _HDC1080_MODULE_H
#define _HDC1080_MODULE_H

#include <Thingify.h>
#include <IModule.h>
#include "Libraries/HDC1080/ClosedCube_HDC1080.h"
#include <Wire.h>
#include <Logging/Logger.h>


class Hdc1080Module : public IModule
{
	Logger& _logger;
	uint8_t _i2cAddress;
	Thingify& _thing;
	ClosedCube_HDC1080 _hdc;
	TwoWire& _wire;
	bool _isSensorPresent;
	Node* _temperatureNode;
	Node* _humidityNode;
	double Round(double x);
	uint64_t _connectTime;
public:
	Hdc1080Module(Thingify& thing, TwoWire& wire, uint8_t i2caddress = 0x40);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif

