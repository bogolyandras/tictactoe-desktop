#pragma once

#include "basetable.h"

enum class FieldState { Empty, Cross, Naught };

enum class TableState { AnybodyToPlace, CrossToPlace, NaughtToPlace, CrossWins, NaughtWins };

struct FieldStateWithProperties {
	
	FieldState fieldState = FieldState::Empty;

	bool lastMove = false;
	bool highlight = false;

};

class Table: public TableLayoutBase<FieldStateWithProperties> {

private:

	Position lastMarkPosition;
	TableState tableState;

	void checkWinSituation(FieldState playerToBeChecked);

public:
	
	Table(const int fieldsX, const int fieldsY);

	FieldState get(Position position);
	TableState getTableState();
	bool mark(Position position, FieldState fieldState);
	void reset();

};
