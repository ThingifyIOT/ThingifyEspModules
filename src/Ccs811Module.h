
#ifndef _CCS811_MODULE_H
#define _CCS811_MODULE_H

#include <Conti.h>
#include <IModule.h>
#include "Libraries/Ccs811/SparkFunCCS811.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>

class Ccs811Module : public IModule
{
	uint8_t _i2cAddress;
	CCS811 _ccs811;
	Conti& _thing;
	TwoWire& _wire;
	ContiLogger& _logger;
	Node* _co2PpmNode;
	Node* _vocPpbNode;
	bool _isSensorPresent;
	uint64_t _connectTime;
	bool ApplyCompensation();
public:
	Ccs811Module(Conti& thing, TwoWire& wire, uint8_t _i2cAddress = 0x5A);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};


#endif