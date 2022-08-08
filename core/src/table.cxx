#include "table.h"

Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY)
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = FieldStateWithProperties();
	}
	tableState = TableState::AnybodyToPlace;
	calculatePositionCombinations();
}

void Table::calculatePositionCombinations() {

	constexpr int inLineToCheck = getNumberOfPositionsToCheck();

	const size_t horizontal_possibilities = ((size_t)fieldsX - inLineToCheck + 1) * fieldsY;
	const size_t vertical_possibilities = fieldsX * ((size_t)fieldsY - inLineToCheck + 1);
	const size_t downwards_right = ((size_t)fieldsX - inLineToCheck + 1) * ((size_t)fieldsY - inLineToCheck + 1);
	const size_t downwards_left = ((size_t)fieldsX - inLineToCheck + 1) * ((size_t)fieldsY - inLineToCheck + 1);

	const size_t all_possibilities = horizontal_possibilities + vertical_possibilities + downwards_right + downwards_left;

	all_positions_and_indexes_size = all_possibilities;
	all_positions = std::shared_ptr<PositionContainer>(new PositionContainer[all_positions_and_indexes_size]);
	all_indexes = std::shared_ptr<IndexContainer>(new IndexContainer[all_positions_and_indexes_size]);

	size_t z = 0;
	for (int x = 0; x < fieldsX; x++) {
		//For each column
		for (int y = 0; y < fieldsY; y++) {
			//For each field in the columns

			//Check Horizontally
			if (y < fieldsY - inLineToCheck + 1) {

				PositionContainer pc;
				IndexContainer ic;

				for (int k = 0; k < inLineToCheck; k++) {
					pc.positions[k] = Position(x, y + k);
					ic.indexes[k] = positionToIndex(pc.positions[k]);
				}

				all_positions.get()[z] = pc;
				all_indexes.get()[z] = ic;

				z++;
			}

			//Check Veritically
			if (x < fieldsX - inLineToCheck + 1) {

				PositionContainer pc;
				IndexContainer ic;

				for (int k = 0; k < inLineToCheck; k++) {
					pc.positions[k] = Position(x + k, y);
					ic.indexes[k] = positionToIndex(pc.positions[k]);
				}

				all_positions.get()[z] = pc;
				all_indexes.get()[z] = ic;

				z++;

			}

			//Check diagonally downwards-right
			if (y < fieldsY - inLineToCheck + 1 && x < fieldsX - inLineToCheck + 1) {

				PositionContainer pc;
				IndexContainer ic;

				for (int k = 0; k < inLineToCheck; k++) {
					pc.positions[k] = Position(x + k, y + k);
					ic.indexes[k] = positionToIndex(pc.positions[k]);
				}

				all_positions.get()[z] = pc;
				all_indexes.get()[z] = ic;

				z++;
			}

			//Check diagonally downwards-left
			if (y < fieldsY - inLineToCheck + 1 && x < fieldsX - inLineToCheck + 1) {

				PositionContainer pc;
				IndexContainer ic;

				for (int k = 0; k < inLineToCheck; k++) {
					pc.positions[k] = Position(x + inLineToCheck - k, y + k);
					ic.indexes[k] = positionToIndex(pc.positions[k]);
				}

				all_positions.get()[z] = pc;
				all_indexes.get()[z] = ic;

				z++;
			}
			
		}
	}

	if (z != all_possibilities) {
		throw std::invalid_argument("not expected number of positions!");
	}

}

FieldState Table::get(Position position)
{
	return fields[positionToIndex(position)].fieldState;
}

TableState Table::getTableState()
{
	return tableState;
}

bool Table::mark(Position position, FieldState fieldState)
{
	const bool anybodyToPlace = tableState == TableState::AnybodyToPlace;
	const bool crossCanPlace = fieldState == FieldState::Cross && tableState == TableState::CrossToPlace;
	const bool naughtCanPlace = fieldState == FieldState::Naught && tableState == TableState::NaughtToPlace;

	if (!anybodyToPlace && !crossCanPlace && !naughtCanPlace) {
		return false;
	}

	if (fields[positionToIndex(position)].fieldState != FieldState::Empty) {
		return false;
	}

	fields[positionToIndex(lastMarkPosition)].lastMove = false;
	fields[positionToIndex(position)].fieldState = fieldState;
	fields[positionToIndex(position)].lastMove = true;
	lastMarkPosition = position;

	if (fieldState == FieldState::Cross) {
		tableState = TableState::NaughtToPlace;
		checkWinSituation(FieldState::Cross);
	}
	else if (fieldState == FieldState::Naught) {
		tableState = TableState::CrossToPlace;
		checkWinSituation(FieldState::Naught);
	}

	return true;
}

void Table::reset()
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = {};
	}
	tableState = TableState::AnybodyToPlace;
}

void Table::checkWinSituation(FieldState playerToBeChecked)
{
	constexpr int inLineToCheck = getNumberOfPositionsToCheck();
	bool foundWinningCombination = false;

	bool canWin;
	for (int i = 0; i < all_positions_and_indexes_size; i++) {
		canWin = true;
		for (int j = 0; j < inLineToCheck; j++) {
			if (fields[all_indexes.get()[i].indexes[j]].fieldState != playerToBeChecked) {
				canWin = false;
				break;
			}
			
		}
		if (canWin) {
			foundWinningCombination = true;
			for (int j = 0; j < inLineToCheck; j++) {
				fields[all_indexes.get()[i].indexes[j]].highlight = true;
			}
		}
	}

	if (foundWinningCombination) {
		if (playerToBeChecked == FieldState::Cross) {
			tableState = TableState::CrossWins;
		}
		else if (playerToBeChecked == FieldState::Naught) {
			tableState = TableState::NaughtWins;
		}
	}

}
