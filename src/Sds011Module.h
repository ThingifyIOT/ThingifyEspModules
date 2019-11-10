#ifndef _SDS011_MODULE_H
#define _SDS011_MODULE_H

#include <Thingify.h>
#include <IModule.h>
#include "Libraries/Sds011/SDS011.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>



class Sds011Module : public IModule
{
	Thingify& _thing;
	Node* _pm25Node;
	Node* _pm10Node;
	SDS011 _sds011;	
	ContiLogger& _logger;
	HardwareSerial& _serial;
	uint8_t _rxPin;
	uint8_t _txPin;
public:
	Sds011Module(Thingify& thing, HardwareSerial& serial, uint8_t rxPin, uint8_t txPin);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif
