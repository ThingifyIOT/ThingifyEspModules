#pragma once

#include <IModule.h>
#include "Libraries\LedMatrix.h"

class LedMatrixUi: public IModule
{
	static const int DeviceCount = 4;
	static const int SpiralPointsCount = 8 * 8 * DeviceCount;
	bool offStart = false;
	int n = 0;
	int n2 = 0;

	/* number of displays, cs pin*/
	struct point_t
	{
		int x;
		int y;
	};
	point_t *_spiralPoints;
	Thingify& _thing;
	LedMatrix ledMatrix;
	Node *_textNode;
public:
	LedMatrixUi(Thingify& thing, uint8_t txPin);
	float temperature;
	const char* GetName() override;
	bool Init() override;
	bool OnValueChanged(Node* node) override;
	bool Tick() override;
	void UpdateLedMatrixPattern();
	void UpdateLedMatrix();
	void PreInit();
};

