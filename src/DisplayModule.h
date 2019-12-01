#pragma once

#include <IModule.h>
#include <Thingify.h>
#include "Libraries/SSD1306/SSD1306.h"
#include <functional>

class DisplayModule : public IModule
{
	SSD1306 display;
	int _scl_pin;
	int _sda_pin;
	Thingify& _thing;
	int _ssd1306_address;
	SoftTimer _displayTimer;
	bool _isInverted;
public:
	DisplayModule(Thingify &thing, int ssd1306Address, int sdaPin, int sclPin, bool isInverted = false, int displayUpdateInterval = 1000);
	void OnStateChanged(ThingState state);
	bool Init() override;
	bool Tick() override;
	std::function<void(SSD1306&)> BeforeDisplay;
	void DisplayInfo();
	void Display();
	const char* GetName() override;
};

