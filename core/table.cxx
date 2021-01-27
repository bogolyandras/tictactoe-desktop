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
