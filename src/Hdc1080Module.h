#ifndef _HDC1080_MODULE_H
#define _HDC1080_MODULE_H

#include <Conti.h>
#include <IModule.h>
#include "Libraries/HDC1080/ClosedCube_HDC1080.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>


class Hdc1080Module : public IModule
{
	ContiLogger& _logger;
	uint8_t _i2cAddress;
	Conti& _thing;
	ClosedCube_HDC1080 _hdc;
	TwoWire& _wire;
	bool _isSensorPresent;
	Node* _temperatureNode;
	Node* _humidityNode;
	double Round(double x);
	uint64_t _connectTime;
public:
	Hdc1080Module(Conti& thing, TwoWire& wire, uint8_t i2caddress = 0x40);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif

