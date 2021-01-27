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
	if (mousePositionX < 0 || mousePositionY < 0) {
		return -1;
	}



	return -1;
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

}

void Layout::OnMouseClick(int positionX, int positionY)
{

}