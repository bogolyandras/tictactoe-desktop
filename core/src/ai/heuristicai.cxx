
#include "heuristicai.h"

Position HeuristicAi::calculateAnswer(TableView* tableView) {

	const PositionIndexCombinedContainer c = tableView->getPositionIndexCombinedContainer();

	//Make a copy from the original table
	const int fieldsX = tableView->fieldsX();
	const int fieldsY = tableView->fieldsY();
	const std::shared_ptr<FieldView[]> originalTable(new FieldView[(size_t)fieldsX * fieldsY]);
	const std::shared_ptr<FieldView[]> demonstrativeTable(new FieldView[(size_t)fieldsX * fieldsY]);

	std::list<std::tuple<size_t, Position>> possibleMoves;
	for (int i = 0; i < fieldsX; i++) {
		for (int j = 0; j < fieldsY; j++) {
			const Position p(i, j);
			const size_t index = positionToIndex(fieldsX, fieldsY, p);
			const FieldView fw = tableView->get(p);
			originalTable[index] = fw;
			demonstrativeTable[index] = fw;
			if (fw == FieldView::Empty) {
				possibleMoves.push_back({ index, p });
			}
		}
	}

	const size_t possibleMovesSize = possibleMoves.size();
	std::list<ThreatComboWithPosition> threatList;

	const ThreatCombo originalThreatCombo = calculateThreats(c, fieldsX, fieldsY, originalTable);
	{
		std::list<std::tuple<size_t, Position>>::iterator it;
		int i;

		for (i = 0, it = possibleMoves.begin(); it != possibleMoves.end(); it++, i++)
		{
			const size_t index = std::get<0>(*it);
			const Position position = std::get<1>(*it);

			demonstrativeTable[index] = FieldView::Mine;
			const ThreatCombo diffThreadCombo = calculateThreats(c, fieldsX, fieldsY, demonstrativeTable);
			demonstrativeTable[index] = FieldView::Empty;

			threatList.push_back(ThreatComboWithPosition((diffThreadCombo - originalThreatCombo), position));
		}

	}

	threatList.sort();

	return threatList.back().position;
}

inline ThreatCombo HeuristicAi::calculateThreats(
	const PositionIndexCombinedContainer c,
	const size_t fieldsX, const size_t fieldsY,
	std::shared_ptr<const FieldView[]> table) {

	ThreatCombo aggregatedThreatCombo;

	for (int i = 0; i < c.positionAndIndexCombinationsSize; i++) {
		ThreatCombo tc;
		bool canPoseThreatToOpponent = true;
		bool canOpponentPoseThreat = true;
		int threatToOpponent = 0;
		int threatFromOpponent = 0;
		for (int j = 0; j < getNumberOfPositionsToCheck(); j++) {
			const size_t index = positionToIndex(fieldsX, fieldsY, c.positionCombinations[i].positions[j]);
			if (table[index] == FieldView::Mine) {
				canOpponentPoseThreat = false;
				threatToOpponent++;
			}
			else if (table[index] == FieldView::Opponent) {
				canPoseThreatToOpponent = false;
				threatFromOpponent++;
			}
		}

		if (canPoseThreatToOpponent) {
			tc.threatPosedToOpponent.values[0]++;
		}
		else {
			threatToOpponent = 0;
		}
		if (canOpponentPoseThreat) {
			tc.threatPosedFromOpponent.values[0]++;
		}
		else {
			threatFromOpponent = 0;
		}
		for (int j = 0; j < getNumberOfPositionsToCheck(); j++) {
			if (canPoseThreatToOpponent && j < threatToOpponent) {
				tc.threatPosedToOpponent.values[j + 1]++;
			}
			if (canOpponentPoseThreat && j < threatFromOpponent) {
				tc.threatPosedFromOpponent.values[j + 1]++;
			}
		}
		aggregatedThreatCombo = aggregatedThreatCombo + tc;
	}

	return aggregatedThreatCombo;
}
