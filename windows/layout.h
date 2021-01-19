#pragma once

#include <iostream>

struct FieldPosition {

	float sizeX = 10;
	float sizeY = 10;

	float positionX = 0;
	float positionY = 0;

};

class Layout {


private:
	float sizeX;
	float sizeY;

	void RecalculateLayout();
	size_t positionToIndex(int x, int y);

public:
	const int fieldsX;
	const int fieldsY;

	const size_t numberOfFields;
	std::unique_ptr<FieldPosition[]> fields;

	Layout(const int fieldsX, const int fieldsY);

	void ChangeSize(float sizeX, float sizeY);

};
