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

	size_t positionAndIndexCombinationsSize;
	std::shared_ptr<PositionCombinationContainer[]> positionCombinations;
	std::shared_ptr<IndexCombinationContainer[]> indexCombinations;

	void checkWinSituation(FieldState playerToBeChecked);
	void calculatePositionCombinations();

public:
	
	Table(const int fieldsX, const int fieldsY);

	FieldState get(Position position);
	TableState getTableState();
	bool mark(Position position, FieldState fieldState);
	void reset();
	PositionIndexCombinedContainer getComboContainer();

};
