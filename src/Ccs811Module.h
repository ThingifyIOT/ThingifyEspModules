
#ifndef _CCS811_MODULE_H
#define _CCS811_MODULE_H

#include <Thingify.h>
#include <IModule.h>
#include "Libraries/Ccs811/SparkFunCCS811.h"
#include <Wire.h>
#include <Logging/Logger.h>

class Ccs811Module : public IModule
{
	uint8_t _i2cAddress;
	CCS811 _ccs811;
	Thingify& _thing;
	TwoWire& _wire;
	Logger& _logger;
	Node* _co2PpmNode;
	Node* _vocPpbNode;
	bool _isSensorPresent;
	uint64_t _connectTime;
	bool ApplyCompensation();
public:
	Ccs811Module(Thingify& thing, TwoWire& wire, uint8_t _i2cAddress = 0x5A);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};


#endif