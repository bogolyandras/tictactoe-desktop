#pragma once

#include "basetable.h"

enum class FieldState { Empty, Cross, Naught };

enum class TableState { AnybodyToPlace, CrossToPlace, NaughtToPlace, CrossWins, NaughtWins };

struct FieldStateWithProperties {
	
	FieldState fieldState = FieldState::Empty;

	bool lastMove = false;
	bool highlight = false;

};

constexpr int getNumberOfPositionsToCheck() {
	return 5;
};

struct PositionContainer {
	Position positions[getNumberOfPositionsToCheck()];
};

struct IndexContainer {
	size_t indexes[getNumberOfPositionsToCheck()];
};

class Table: public TableLayoutBase<FieldStateWithProperties> {

private:

	Position lastMarkPosition;
	TableState tableState;

	size_t all_positions_and_indexes_size;
	std::shared_ptr<PositionContainer> all_positions;
	std::shared_ptr<IndexContainer> all_indexes;

	void checkWinSituation(FieldState playerToBeChecked);
	void calculatePositionCombinations();

public:
	
	Table(const int fieldsX, const int fieldsY);

	FieldState get(Position position);
	TableState getTableState();
	bool mark(Position position, FieldState fieldState);
	void reset();

};
