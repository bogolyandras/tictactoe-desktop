#pragma once

#include "basetable.h"

struct FieldShapeAndPosition {

	float sizeX = 10;
	float sizeY = 10;

	float positionX = 0;
	float positionY = 0;

};

class Layout: public TableLayoutBase<FieldShapeAndPosition> {

private:
	float sizeX;
	float sizeY;

	void RecalculateLayout();

public:
	Layout(const int fieldsX, const int fieldsY);

	void ChangeSize(float sizeX, float sizeY);

};
