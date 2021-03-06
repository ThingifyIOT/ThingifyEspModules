#ifndef _HUMIDITY_MODULE_H
#define _HUMIDITY_MODULE_H

#include <IModule.h>
#include <Thingify.h>
#include "Libraries/DHT/DHT.h"

#define REPORT_INTERVAL 5000 // in msec must > 2000


class HumidityModule : public IModule
{	
	DHT * _dht;
	Thingify* _thing;
	float t, h, d;
	uint8_t _pin;
	Node *temperatureNode;
	Node *humidityNode;
public:
	HumidityModule(Thingify* thing, uint8_t pin);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif
