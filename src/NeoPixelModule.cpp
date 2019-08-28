#include "NeoPixelModule.h"

NeoPixelModule::NeoPixelModule(Conti &device, uint8_t pin)
	:_device(device), _pin(pin)
{
}

const char* NeoPixelModule::GetName()
{
	return "Neo Pixel";
}

bool NeoPixelModule::Init()
{
	pixels = new Adafruit_NeoPixel(8, _pin, NEO_GRB + NEO_KHZ800);
	pixels->begin();
	pixels->setBrightness(255);


	_colorNode = _device.AddColor("color")->OnChanged(this, ValueChangedHandler);
	_animationPausedNode = _device.AddBoolean("Pauza");	

	_animationPausedNode->OnChanged(this, ValueChangedHandler);

	return true;
}


uint32_t Wheel(Adafruit_NeoPixel* np, int WheelPos) 
{
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) 
	{
		return np->Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	if (WheelPos < 170) 
	{
		WheelPos -= 85;
		return np->Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	WheelPos -= 170;
	return np->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void theaterChaseRainbow(Adafruit_NeoPixel* np, uint8_t wait)
{
	static int j = 0;
	static int q = 0;
	
	static uint64_t ms = 0;
	if ((millis()-ms) < wait)
	{
		return;
	}
	ms = millis();

	for (uint16_t i = 0; i < np->numPixels(); i = i + 3)
	{
		np->setPixelColor(i + q, 0);
	}
	

	j = j < 256 ? j + 1 : 0;
	q = q < 3 ? q + 1 : 0;


	for (uint16_t i = 0; i < 8; i = i + 3)
	{
		np->setPixelColor(i + q, Wheel(np, (i + j) % 255));
	}

	np->show();
}

void DrawFadingPattern(Adafruit_NeoPixel* np, int r, int g, int b)
{
	static uint64_t lastUpdate = 0;
	static float x = 0;
	if ((millis() - lastUpdate) < 10)
	{
		return;
	}

	lastUpdate = millis();

	x += 2.0f*PI / 100.0f;
	for (int i = 0; i< 8; i++)
	{
		float cosValue = (1 - (cos(x) / 2.0f)) / 2.0f;
		int pixelValue = cosValue*255.0;
		np->setPixelColor(i, pixelValue*r, pixelValue*g, pixelValue*b);
		np->show();
	}
}

bool NeoPixelModule::Tick()
{
	if (_animationPausedNode->Value.AsBool())
	{
		return true;
	}
	switch (_mode)
	{
		case NeoPixelMode::Disabled: 
			break;
		case NeoPixelMode::BlinkingRed: 
			DrawFadingPattern(pixels, 1,0,0);
			break;
		case NeoPixelMode::BlinkingGreen: 
			DrawFadingPattern(pixels, 0, 1, 0);
			break;
		case NeoPixelMode::BlinkingBlue:
			DrawFadingPattern(pixels, 0, 0, 1);
			break;
		case NeoPixelMode::Theater:
			theaterChaseRainbow(pixels, 50);
			break;
		case NeoPixelMode::Manual: 
			
			break;
		default: break;
	}
	return true;
}

void NeoPixelModule::SetColor(int r, int g, int b)
{
	for (int i = 0; i < 8; i++)
	{
		pixels->setPixelColor(i, r, g, b);
	}
	pixels->show();
}

void NeoPixelModule::Clear()
{
	pixels->clear();
}

void NeoPixelModule::SetMode(NeoPixelMode mode)
{
	_mode = mode;
	if (_mode == NeoPixelMode::Disabled)
	{
		pixels->clear();
	}
	if (_mode == NeoPixelMode::Manual)
	{
	}
}


void NeoPixelModule::SetManualColor(ContiColor color)
{
	for (int i = 0; i < 8; i++)
	{
		pixels->setPixelColor(i, color.R, color.G, color.B);
		pixels->setBrightness(color.A);
		pixels->show();
	}
}


bool NeoPixelModule::OnValueChanged(Node *node)
{
	if (node == _animationPausedNode)
	{
		return true;
	}

	if (_mode != NeoPixelMode::Manual)
	{
		return true;
	}

	if (node == _colorNode)
	{
		SetManualColor(node->Value.AsColor());
	}
	return true;
}
