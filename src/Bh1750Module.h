#ifndef _BH1750_MODULE_H
#define _BH1750_MODULE_H

#include <Conti.h>
#include <IModule.h>
#include "Libraries/BH1750/ErriezBH1750.h"
#include <Wire.h>
#include <Logging/ContiLogger.h>



class Bh1750Module : public IModule
{
	Conti& _thing;
	bool _isInitSuccessfull;
	TwoWire& _wire;
	uint8_t _i2cAddress;
	bool _isSensorPresent;
	Node* _lightNode;
	BH1750 _bh1750;
	ContiLogger& _logger;
public:
	Bh1750Module(Conti &thing, TwoWire &wire, uint8_t i2cAddress = 0x23);
	const char* GetName() override;
	bool Init() override;
	bool Tick() override;
};

#endif

