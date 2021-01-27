#include "layout.h"
#include <algorithm>

Layout::Layout(const int fieldsX, const int fieldsY):
	sizeX{ 100 },
	sizeY{ 100 },
	TableLayoutBase(fieldsX, fieldsY),
	cursorPositionX(fieldsX / 2),
	cursorPositionY(fieldsY / 2)
{
	RecalculateLayout();
}

void Layout::ChangeSize(float sizeX, float sizeY)
{
	this->sizeX = sizeX;
	this->sizeY = sizeY;

	RecalculateLayout();
}

void Layout::RecalculateCommonFieldStructure() {
	const float oneFieldSizeX = (float)sizeX / fieldsX;
	const float oneFieldSizeY = (float)sizeY / fieldsY;

	commonFieldStructure.size = std::min(oneFieldSizeX, oneFieldSizeY);

	if (commonFieldStructure.size < oneFieldSizeX) {
		commonFieldStructure.offsetX = (oneFieldSizeX - commonFieldStructure.size) * fieldsX / 2;
	}
	else {
		commonFieldStructure.offsetX = 0;
	}

	float offsetY;
	if (commonFieldStructure.size < oneFieldSizeY) {
		commonFieldStructure.offsetY = (oneFieldSizeY - commonFieldStructure.size) * fieldsY / 2;
	}
	else {
		commonFieldStructure.offsetY = 0;
	}
}

void Layout::RecalculateLayout()
{
	RecalculateCommonFieldStructure();

	for (int i = 0; i < fieldsY; i++) {
		for (int j = 0; j < fieldsX; j++) {
			const size_t index = positionToIndex(j, i);
			fields[index].positionX = j * commonFieldStructure.size + commonFieldStructure.offsetX;
			fields[index].positionY = i * commonFieldStructure.size + commonFieldStructure.offsetY;
			fields[index].cursorOver = cursorPositionAsIndex() == index;
			fields[index].mouseOver = mousePositionAsIndex() == index;
		}
	}
}

size_t Layout::cursorPositionAsIndex() {
	return positionToIndex(cursorPositionX, cursorPositionY);
}

size_t Layout::mousePositionAsIndex()
{
	const bool mouseOutOfWindow = mousePositionX < 0 || mousePositionY < 0;
	if (mouseOutOfWindow) {
		return -1;
	}

	const bool leftOutRange = (float)mousePositionX < commonFieldStructure.offsetX;
	const bool rightOutRange = (float)mousePositionX > sizeX - commonFieldStructure.offsetX;
	if (leftOutRange || rightOutRange) {
		return -1;
	}

	const bool topOutRange = (float)mousePositionY < commonFieldStructure.offsetY;
	const bool bottomOutRange = (float)mousePositionY > sizeY - commonFieldStructure.offsetY;
	if (topOutRange || bottomOutRange) {
		return -1;
	}

	 
	const int mouseFieldX = (mousePositionX - commonFieldStructure.offsetX) / commonFieldStructure.size;
	const int mouseFieldY = (mousePositionY - commonFieldStructure.offsetY) / commonFieldStructure.size;

	return positionToIndex(mouseFieldX, mouseFieldY);
}

void Layout::OnKeyPressUp() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPositionY = std::max(cursorPositionY - 1, 0);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressDown() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPositionY = std::min(cursorPositionY + 1, fieldsY - 1);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressLeft() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPositionX = std::max(cursorPositionX - 1, 0);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressRight() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPositionX = std::min(cursorPositionX + 1, fieldsX - 1);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnMouseMove(int positionX, int positionY)
{
	const size_t prePos = mousePositionAsIndex();
	if (prePos >= 0) {
		fields[prePos].mouseOver = false;
	}
	mousePositionX = positionX;
	mousePositionY = positionY;
	const size_t postPos = mousePositionAsIndex();
	if (postPos >= 0) {
		fields[postPos].mouseOver = true;
	}
}

void Layout::OnMouseClick(int positionX, int positionY)
{
	mousePositionX = positionX;
	mousePositionY = positionY;
}
