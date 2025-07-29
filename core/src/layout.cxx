#include "layout.h"
#include <algorithm>

Layout::Layout(const int fieldsX, const int fieldsY):
	sizeX{ 100 },
	sizeY{ 100 },
	TableLayoutBase(fieldsX, fieldsY),
	cursorPosition(fieldsX / 2, fieldsY / 2)
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
			const size_t index = positionToIndex(Position(j, i));
			fields[index].positionX = j * commonFieldStructure.size + commonFieldStructure.offsetX;
			fields[index].positionY = i * commonFieldStructure.size + commonFieldStructure.offsetY;
			fields[index].cursorOver = cursorPositionAsIndex() == index;
			fields[index].mouseOver = mousePositionAsIndex() == index;
		}
	}
}

size_t Layout::cursorPositionAsIndex() {
	return positionToIndex(cursorPosition);
}



auto Layout::mousePosition() -> std::optional<Position>
{
	const bool mouseOutOfWindow = mousePositionX < 0 || mousePositionY < 0;
	if (mouseOutOfWindow) {
		return std::nullopt;
	}

	const bool leftOutRange = static_cast<float>(mousePositionX) < commonFieldStructure.offsetX;
	const bool rightOutRange = static_cast<float>(mousePositionX) > sizeX - commonFieldStructure.offsetX;
	if (leftOutRange || rightOutRange) {
		return std::nullopt;
	}

	const bool topOutRange = static_cast<float>(mousePositionY) < commonFieldStructure.offsetY;
	const bool bottomOutRange = static_cast<float>(mousePositionY) > sizeY - commonFieldStructure.offsetY;
	if (topOutRange || bottomOutRange) {
		return std::nullopt;
	}

	Position mouseField;
	mouseField.X = static_cast<int>((mousePositionX - commonFieldStructure.offsetX) / commonFieldStructure.size);
	mouseField.Y = static_cast<int>((mousePositionY - commonFieldStructure.offsetY) / commonFieldStructure.size);

	return mouseField;
}

auto Layout::mousePositionAsIndex() -> std::optional<size_t>
{
	const auto mouse_position = mousePosition();
	if (!mouse_position.has_value())
	{
		return std::nullopt;
	}
	const Position position = mouse_position.value();
	return positionToIndex(position);
}

void Layout::OnKeyPressUp() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPosition.Y = std::max(cursorPosition.Y - 1, 0);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressDown() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPosition.Y = std::min(cursorPosition.Y + 1, fieldsY - 1);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressLeft() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPosition.X = std::max(cursorPosition.X - 1, 0);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnKeyPressRight() {
	fields[cursorPositionAsIndex()].cursorOver = false;
	cursorPosition.X = std::min(cursorPosition.X + 1, fieldsX - 1);
	fields[cursorPositionAsIndex()].cursorOver = true;
}

void Layout::OnMouseMove(int positionX, int positionY)
{
	const auto previous_position= mousePositionAsIndex();
	if (previous_position.has_value()) {
		fields[previous_position.value()].mouseOver = false;
	}

	mousePositionX = positionX;
	mousePositionY = positionY;
	const auto current_position = mousePositionAsIndex();
	if (current_position.has_value()) {
		fields[current_position.value()].mouseOver = true;
	}
}

