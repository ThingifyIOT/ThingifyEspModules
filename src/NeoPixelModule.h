#pragma once
#include  "Libraries/AdafruitNeoPixel/Adafruit_NeoPixel.h"
#include <Conti.h>
#include <IModule.h>


enum class NeoPixelMode
{
	Disabled,
	BlinkingRed,
	BlinkingBlue,
	BlinkingGreen,
	Theater,
	Manual
};


class NeoPixelModule : public IModule
{
public:
	const char* GetName() override;
	bool Tick() override;
private:
	NeoPixelMode _mode;
	Adafruit_NeoPixel *pixels;

	Node *_colorNode, *_animationPausedNode;
	Conti &_device;
	uint8_t _pin;
public:
	void SetMode(NeoPixelMode mode);
	void SetManualColor(ContiColor color);
	NeoPixelModule(Conti &device, uint8_t pin);
	bool OnValueChanged(Node *node) override;
	bool Init() override;
	void SetColor(int r, int g, int b);
	void Clear();
	bool Set(int outputNumber, bool value);
};

