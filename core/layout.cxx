#include "layout.h"
#include <algorithm>

Layout::Layout(const int fieldsX, const int fieldsY):
	sizeX{ 100 },
	sizeY{ 100 },
	TableLayoutBase(fieldsX, fieldsY)
{
	RecalculateLayout();
}

void Layout::ChangeSize(float sizeX, float sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	RecalculateLayout();
}

void Layout::RecalculateLayout()
{
	const float oneFieldSizeX = (float)sizeX / fieldsX;
	const float oneFieldSizeY = (float)sizeY / fieldsY;

	const float fieldSize = fmin(oneFieldSizeX, oneFieldSizeY);
	
	float offsetX;
	if (fieldSize < oneFieldSizeX) {
		offsetX = (oneFieldSizeX - fieldSize) * fieldsX / 2;
	}
	else {
		offsetX = 0;
	}

	float offsetY;
	if (fieldSize < oneFieldSizeY) {
		offsetY = (oneFieldSizeY - fieldSize) * fieldsY / 2;
	}
	else {
		offsetY = 0;
	}

	for (int i = 0; i < fieldsY; i++) {
		for (int j = 0; j < fieldsX; j++) {
			const size_t index = positionToIndex(j, i);
			fields[index].sizeX = fieldSize;
			fields[index].sizeY = fieldSize;
			fields[index].positionX = j * fieldSize + offsetX;
			fields[index].positionY = i * fieldSize + offsetY;
		}
	}
}
