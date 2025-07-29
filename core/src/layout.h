#ifndef LAYOUT_H
#define LAYOUT_H

#include "basetable.h"
#include <optional>

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
	float sizeX;
	float sizeY;

	int mousePositionX = -1;
	int mousePositionY = -1;

	void RecalculateCommonFieldStructure();
	void RecalculateLayout();

	size_t cursorPositionAsIndex();
	auto mousePositionAsIndex() -> std::optional<size_t>;

public:
	CommonFieldStructure commonFieldStructure;

	Layout(const int fieldsX, const int fieldsY);

	Position cursorPosition;
	auto mousePosition() -> std::optional<Position>;

	void ChangeSize(float sizeX, float sizeY);

	void OnKeyPressUp();
	void OnKeyPressDown();
	void OnKeyPressLeft();
	void OnKeyPressRight();
	void OnMouseMove(int positionX, int positionY);

};

#endif // LAYOUT_H
