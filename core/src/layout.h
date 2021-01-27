#pragma once

#include "basetable.h"

struct FieldShapeAndPosition {

	//Position relative to the client area
	float positionX = 0;
	float positionY = 0;

	bool mouseOver = false;
	bool cursorOver = false;

};

struct CommonFieldStructure {

	float offsetX = 0;
	float offsetY = 0;
	float size = 10;

};

class Layout: public TableLayoutBase<FieldShapeAndPosition> {

private:
	float sizeX;
	float sizeY;

	int cursorPositionX;
	int cursorPositionY;

	int mousePositionX = -1;
	int mousePositionY = -1;

	void RecalculateCommonFieldStructure();
	void RecalculateLayout();
	size_t cursorPositionAsIndex();
	size_t mousePositionAsIndex();

public:
	CommonFieldStructure commonFieldStructure;

	Layout(const int fieldsX, const int fieldsY);

	void ChangeSize(float sizeX, float sizeY);

	void OnKeyPressUp();
	void OnKeyPressDown();
	void OnKeyPressLeft();
	void OnKeyPressRight();
	void OnMouseMove(int positionX, int positionY);
	void OnMouseClick(int positionX, int positionY);

};
