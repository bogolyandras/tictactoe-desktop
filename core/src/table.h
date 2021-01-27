#pragma once

#include "basetable.h"

enum class FieldState { Empty, Cross, Naught };

struct FieldStateWithProperties {
	
	FieldState fieldState = FieldState::Empty;

	bool lastMove = false;
	bool highlight = false;

};

class Table: public TableLayoutBase<FieldStateWithProperties> {
public:
	

	Table(const int fieldsX, const int fieldsY);

};
