#include "table.h"

Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY)
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

FieldState Table::get(Position position)
{
	return fields[positionToIndex(position)].fieldState;
}

void Table::mark(Position position, FieldState fieldState)
{
	fields[positionToIndex(lastMarkPosition)].lastMove = false;
	fields[positionToIndex(position)].fieldState = fieldState;
	fields[positionToIndex(position)].lastMove = true;
	lastMarkPosition = position;
}

void Table::reset()
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = {};
	}
}
