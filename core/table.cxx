#include "table.h"


Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY)
{
	for (size_t i = 0; i < numberOfFields; i++) {
		if (i % 20 == 0) {
			fields[i] = FieldState::Naught;
		}
		else if (i % 20 == 1) {
			fields[i] = FieldState::Cross;
		}
		else {
			fields[i] = FieldState::Empty;
		}
	}
}
