#include "table.h"
#include <algorithm>

Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY),
	positionX(fieldsX / 2),
	positionY(fieldsY / 2)
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = FieldStateWithProperties();
		if (i % 20 == 0) {
			fields[i].fieldState = FieldState::Naught;
		}
		else if (i % 20 == 1) {
			fields[i].fieldState = FieldState::Cross;
		}
	}
}

int Table::positionAsIndex() {
	return positionToIndex(positionX, positionY);
}

void Table::keyPressUp() {
	positionY = std::max(positionY - 1, 0);
}

void Table::keyPressDown() {
	positionY = std::min(positionY + 1, fieldsY - 1);
}

void Table::keyPressLeft() {
	positionX = std::max(positionX - 1, 0);
}

void Table::keyPressRight() {
	positionX = std::min(positionX + 1, fieldsX - 1);
}
