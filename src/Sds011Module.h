#ifndef _SDS011_MODULE_H
#define _SDS011_MODULE_H

#include <Conti.h>
#include <IModule.h>
#include "Libraries/Sds011/SDS011.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>



class Sds011Module : public IModule
{
	Conti& _thing;
	Node* _pm25Node;
	Node* _pm10Node;
	SDS011 _sds011;
	HardwareSerial& _serial;
	uint8_t _rxPin;
	uint8_t _txPin;
	ContiLogger& _logger;
public:
	Sds011Module(Conti& thing, HardwareSerial& serial, uint8_t rxPin, uint8_t txPin);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif