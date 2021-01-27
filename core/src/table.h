#pragma once

#include "basetable.h"

enum class FieldState { Empty, Cross, Naught };

struct FieldStateWithProperties {
	
	FieldState fieldState = FieldState::Empty;

	bool lastMove = false;
	bool highlight = false;

};

class Table: public TableLayoutBase<FieldStateWithProperties> {
private:
	Position lastMarkPosition;
public:
	
	Table(const int fieldsX, const int fieldsY);

	FieldState get(Position position);
	void mark(Position position, FieldState fieldState);
	void reset();

};
