#include "table.h"

Table::Table(const int fieldsX, const int fieldsY) :
	TableLayoutBase(fieldsX, fieldsY)
{
	for (size_t i = 0; i < numberOfFields; i++) {
		fields[i] = FieldStateWithProperties();
	}
	tableState = TableState::AnybodyToPlace;
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
	const int inLineToCheck = 5;
	bool foundWinningCombination = false;
	
	for (int x = 0; x < fieldsX; x++) {
		//For each column
		for (int y = 0; y < fieldsY; y++) {
			//For each field in the columns
			bool canWin;

			//Check Horizontally
			canWin = true;
			for (int k = 0; k < inLineToCheck; k++) {
				if (fields[positionToIndex(Position(x, y + k))].fieldState != playerToBeChecked) {
					canWin = false;
					break;
				}
			}
			if (canWin) {
				foundWinningCombination = true;
				for (int k = 0; k < inLineToCheck; k++) {
					fields[positionToIndex(Position(x, y + k))].highlight = true;
				}
			}

			//Check Veritically
			canWin = true;
			for (int k = 0; k < inLineToCheck; k++) {
				if (fields[positionToIndex(Position(x + k, y))].fieldState != playerToBeChecked) {
					canWin = false;
					break;
				}
			}
			if (canWin) {
				foundWinningCombination = true;
				for (int k = 0; k < inLineToCheck; k++) {
					fields[positionToIndex(Position(x + k, y))].highlight = true;
				}
			}

			//Check diagonally downwards-right
			canWin = true;
			for (int k = 0; k < inLineToCheck; k++) {
				if (fields[positionToIndex(Position(x + k, y + k))].fieldState != playerToBeChecked) {
					canWin = false;
					break;
				}
			}
			if (canWin) {
				foundWinningCombination = true;
				for (int k = 0; k < inLineToCheck; k++) {
					fields[positionToIndex(Position(x + k, y + k))].highlight = true;
				}
			}

			//Check diagonally downwards-left
			canWin = true;
			for (int k = 0; k < inLineToCheck; k++) {
				if (fields[positionToIndex(Position(x + inLineToCheck - k, y + k))].fieldState != playerToBeChecked) {
					canWin = false;
					break;
				}
			}
			if (canWin) {
				foundWinningCombination = true;
				for (int k = 0; k < inLineToCheck; k++) {
					fields[positionToIndex(Position(x + inLineToCheck - k, y + k))].highlight = true;
				}
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
