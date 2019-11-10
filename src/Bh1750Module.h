#ifndef _BH1750_MODULE_H
#define _BH1750_MODULE_H

#include <Thingify.h>
#include <IModule.h>
#include "Libraries/BH1750/ErriezBH1750.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>



class Bh1750Module : public IModule
{
	ContiLogger& _logger;
	uint8_t _i2cAddress;
	BH1750 _bh1750;
	Thingify& _thing;
	bool _isInitSuccessfull;
	TwoWire& _wire;
	bool _isSensorPresent;
	Node* _lightNode;
public:
	Bh1750Module(Thingify &thing, TwoWire &wire, uint8_t i2cAddress = 0x23);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif

