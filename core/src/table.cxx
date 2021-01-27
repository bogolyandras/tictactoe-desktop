#include "table.h"

Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY)
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = FieldStateWithProperties();
	}
}

FieldState Table::get(Position position)
{
	return fields[positionToIndex(position)].fieldState;
}

bool Table::mark(Position position, FieldState fieldState)
{
	if (fields[positionToIndex(position)].fieldState != FieldState::Empty) {
		return false;
	}
	fields[positionToIndex(lastMarkPosition)].lastMove = false;
	fields[positionToIndex(position)].fieldState = fieldState;
	fields[positionToIndex(position)].lastMove = true;
	lastMarkPosition = position;
	return true;
}

void Table::reset()
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = {};
	}
}
