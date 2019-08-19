#include "LedMatrixUi.h"
#include <ContiUtils.h>

LedMatrixUi::LedMatrixUi(Conti& thing, uint8_t txPin): 
	_thing(thing), 
	ledMatrix(DeviceCount, txPin)
{
}

const char* LedMatrixUi::GetName()
{
	return "Led matrix ui";
}

bool LedMatrixUi::Init()
{	
	_spiralPoints = new point_t[SpiralPointsCount];
	
	_textNode = _thing.AddString("led.text");
	_textNode->OnChanged(this, ValueChangedHandler);

	static int totalWidth = 8 * 4;
	static int totalHeight = 8;

	static int dir = 0;
	static int x = 0;
	static int y = 0;
	static int left = 0;
	static int top = 0;


	for (int n = 0; n < SpiralPointsCount; n++)
	{
		_spiralPoints[n].x = x;
		_spiralPoints[n].y = y;
		
		if (dir == 0)
		{
			x++;
			if (x+1 == totalWidth)
			{
				dir = 1;
			}			
		}
		else if (dir == 1)
		{
			y++;
			if (y+1 == totalHeight)
			{
				dir = 2;
			}
			
		}
		else if (dir == 2)
		{
			x--;
			if (x == left)
			{
				dir = 3;
			}
		}
		else if (dir == 3)
		{
			y--;

			if (y == top)
			{
				dir = 0;
				top++;
				left++;
				x = left;
				y = top;
				totalWidth--;
				totalHeight--;
			}
			if (left > 3)
			{
				totalWidth = 8 * 4;
				totalHeight = 8;
				ledMatrix.clear();
				dir = 0;
				x = 0;
				y = 0;
				left = 0;
				top = 0;
				dir = 0;
			}
		}
	}
	return true;
}


bool LedMatrixUi::OnValueChanged(Node *node)
{
	Serial.printf("Set text: %s\n", node->Value.stringValue.c_str());
	ledMatrix.clear();
	ledMatrix.setText(node->Value.stringValue.c_str());
	ledMatrix.drawText();
	ledMatrix.commit();
	return true;
}



bool LedMatrixUi::Tick()
{
	if(_textNode->Value.stringValue.length() > 0)
	{
		static SoftTimer st(25);
		if (!st.IsElapsed())
		{
			return true;
		}
		ledMatrix.scrollTextLeft();
		ledMatrix.drawText();
		ledMatrix.commit();
		return false;
	}
	static int displayState = 1;
	int prevDisplayState = displayState;
	static SoftTimer stMode(9000);
	if(stMode.IsElapsed())
	{
		if(displayState == 0)
		{
			displayState = 1;
		}
		else if(displayState == 1)
		{
			displayState = 0;
		}
	}
	if(prevDisplayState != displayState)
	{
		ledMatrix.clear();
		offStart = false;
		n = 0;
		n2 = 0;
	}
	if(displayState == 0)
	{
		UpdateLedMatrixPattern();
	}
	else if(displayState == 1)
	{
		UpdateLedMatrix();
	}
	return true;
}

void LedMatrixUi::UpdateLedMatrixPattern()
{
	static SoftTimer st(13);
	if (!st.IsElapsed())
	{
		return;
	}
	

	if (!offStart)
	{
		ledMatrix.setPixel(_spiralPoints[n].x, _spiralPoints[n].y, true);
		if (n == SpiralPointsCount)
		{
			n = 0;			
			offStart = true;			
		}
		else
		{
			n++;
		}
	}	
	else if (offStart)
	{
		ledMatrix.setPixel(_spiralPoints[n2].x, _spiralPoints[n2].y, false);
		if (n2 == SpiralPointsCount)
		{
			n2 = 0;
			offStart = false;
		}
		else
		{
			n2++;
		}
	}

	ledMatrix.commit();

}

void LedMatrixUi::UpdateLedMatrix()
{
	static uint64_t lastLedMatrixCommit = 0;
	if (millis() - lastLedMatrixCommit < 25)
	{
		return;
	}
	lastLedMatrixCommit = millis();

	static SoftTimer st(4000);
	if (st.IsElapsed())
	{
		char text[200];
		memset(text, 0, 200);
		strcpy(text, "");
		dtostrf(temperature, 2, 1, text);
		//strcat(text, " C");

		ledMatrix.setText(text);
		for (int i = 0; i < 32; i++)
		{
			ledMatrix.scrollTextLeft();
		}

		ledMatrix.drawText();
	}
	
	ledMatrix.commit();

}

void LedMatrixUi::PreInit()
{
	
	ledMatrix.init();
	ledMatrix.clear();
	ledMatrix.setIntensity(1); // range is 0-15
}
